#include "TFW_message_loop.h"

#include <string.h>
#include <unistd.h>

#include "TFW_list.h"
#include "TFW_mem.h"
#include "TFW_thread.h"
#include "TFW_timer.h"
#include "TFW_utils_log.h"

#define LOOP_NAME_LEN 32
#define TIME_THOUSANDS_MULTIPLIER 1000LL
#define MAX_LOOPER_CNT 30U
#define MAX_LOOPER_PRINT_CNT 64

typedef struct {
    TFW_Message *msg;
    TFW_ListNode node;
} TFW_MessageNode;

struct TFW_LooperContext {
    TFW_ListNode msgHead;
    char name[LOOP_NAME_LEN];
    volatile unsigned char stop; // destroys looper, stop =1, and running =0
    volatile unsigned char running;
    TFW_Message *currentMsg;
    uint32_t msgSize;
    TFW_Mutex_t lock;
    TFW_MutexAttr_t attr;
    TFW_Cond_t cond;          // 用于通知有新消息
    TFW_Cond_t condRunning;   // 用于通知looper状态变化
};

// Looper配置项结构体
struct LooperConfigItem {
    TFW_LooperType type;
    TFW_Looper *looper;
};

// 全局Looper配置数组
static struct LooperConfigItem g_looperConfig[TFW_LOOP_TYPE_MAX] = {0}; // 只为有效枚举值分配空间

static uint32_t g_looperCnt = 0;
static int8_t g_isNeedDestroy = 0;
static int8_t g_isThreadStarted = 0;

TFW_Looper *TFW_GetLooper(TFW_LooperType type)
{
    // 检查类型是否有效
    if (type < TFW_LOOP_TYPE_DEFAULT || type >= TFW_LOOP_TYPE_MAX) {
        return NULL;
    }

    return g_looperConfig[type].looper;
}

void TFW_SetLooper(TFW_LooperType type, TFW_Looper *looper)
{
    // 检查类型是否有效
    if (type < TFW_LOOP_TYPE_DEFAULT || type >= TFW_LOOP_TYPE_MAX) {
        return;
    }

    g_looperConfig[type].type = type;
    g_looperConfig[type].looper = looper;
}

static void ReleaseLooper(const TFW_Looper *looper)
{
    for (int32_t i = 0; i < (TFW_LOOP_TYPE_MAX); i++) {
        if (g_looperConfig[i].looper == looper) {
            g_looperConfig[i].looper = NULL;
            return;
        }
    }
}

static int64_t UptimeMicros(void)
{
    return (int64_t)TFW_GetTimestampUs();
}

static void FreeTFWMsg(TFW_Message *msg)
{
    if (msg->FreeMessage == NULL) {
        TFW_Free(msg);
    } else {
        msg->FreeMessage(msg);
    }
}

TFW_Message *TFW_MallocMessage(void)
{
    TFW_Message *msg = (TFW_Message *)TFW_Calloc(sizeof(TFW_Message));
    if (msg == NULL) {
        TFW_LOGE_UTILS("malloc TFW_Message failed");
        return NULL;
    }
    return msg;
}

void TFW_FreeMessage(TFW_Message *msg)
{
    if (msg != NULL) {
        FreeTFWMsg(msg);
    }
}

static void DumpLooperLocked(const TFW_Looper *looper)
{
    if (looper == NULL || looper->context == NULL) {
        TFW_LOGE_UTILS("Invalid looper or context");
        return;
    }

    const TFW_LooperContext *context = looper->context;
    if (context->currentMsg != NULL) {
        TFW_LOGD_UTILS("Current Message - What: %d, Time: %lld",
                        context->currentMsg->what, context->currentMsg->time);
    }

    // 遍历消息队列并打印信息
    if (!TFW_IsListEmpty(&context->msgHead)) {
        TFW_ListNode *item = NULL;
        TFW_ListNode *nextItem = NULL;
        uint32_t count = 0;

        TFW_LIST_FOR_EACH_SAFE(item, nextItem, &context->msgHead) {
            TFW_MessageNode *itemNode = TFW_LIST_ENTRY(item, TFW_MessageNode, node);
            if (itemNode->msg != NULL) {
                TFW_LOGD_UTILS("Message[%u] - What: %d, Time: %lld, Handler: %s",
                                count, itemNode->msg->what, itemNode->msg->time,
                                (itemNode->msg->handler && itemNode->msg->handler->name) ?
                                itemNode->msg->handler->name : "null");
                count++;

                // 避免打印过多信息
                if (count >= MAX_LOOPER_PRINT_CNT) {
                    TFW_LOGD_UTILS("... and more messages");
                    break;
                }
            }
        }
    }
}

void DumpLooper(const TFW_Looper *looper)
{
    if (looper == NULL || looper->context == NULL) {
        TFW_LOGE_UTILS("Invalid looper or context");
        return;
    }

    TFW_LooperContext *context = looper->context;
    if (TFW_Mutex_Lock(&context->lock) != 0) {
        TFW_LOGE_UTILS("Failed to lock looper mutex");
        return;
    }

    DumpLooperLocked(looper);

    (void)TFW_Mutex_Unlock(&context->lock);
}

// ... existing code ...

static void *LoopTask(void *arg)
{
    TFW_Looper *looper = (TFW_Looper *)arg;
    TFW_LooperContext *context = looper->context;
    if (context == NULL) {
        TFW_LOGE_UTILS("loop context is NULL");
        return NULL;
    }

    TFW_LOGD_UTILS("LoopTask running. name=%s", context->name);

    if (TFW_Mutex_Lock(&context->lock) != 0) {
        TFW_LOGE_UTILS("lock failed");
        return NULL;
    }
    context->running = 1;
    (void)TFW_Mutex_Unlock(&context->lock);

    for (;;) {
        if (TFW_Mutex_Lock(&context->lock) != 0) {
            return NULL;
        }
        // wait
        if (context->stop == 1) {
            TFW_LOGI_UTILS("LoopTask stop is 1. name=%s", context->name);
            (void)TFW_Mutex_Unlock(&context->lock);
            break;
        }

        if (TFW_IsListEmpty(&context->msgHead)) {
            TFW_LOGD_UTILS("LoopTask wait msg list empty. name=%s", context->name);
            // 使用条件变量等待新消息，替代轮询等待
            TFW_Cond_Wait(&context->cond, &context->lock, NULL);
            (void)TFW_Mutex_Unlock(&context->lock);
            continue;
        }

        int64_t now = UptimeMicros();
        TFW_ListNode *item = context->msgHead.next;
        TFW_Message *msg = NULL;
        TFW_MessageNode *itemNode = TFW_LIST_ENTRY(item, TFW_MessageNode, node);
        int64_t time = itemNode->msg->time;
        if (now >= time) {
            msg = itemNode->msg;
            TFW_ListDelete(item);
            TFW_Free(itemNode);
            context->msgSize--;
            if (looper->dumpable) {
                TFW_LOGD_UTILS(
                    "LoopTask get message. name=%s, handle=%s, what=%d, arg1=%llu, msgSize=%u, time=%lld",
                    context->name, msg->handler ? msg->handler->name : "null", msg->what, msg->arg1, context->msgSize,
                    msg->time);
            }
        } else {
            // 使用条件变量的定时等待功能，在指定时间点自动唤醒
            TFW_SysTime tv;
            tv.sec = time / TIME_THOUSANDS_MULTIPLIER / TIME_THOUSANDS_MULTIPLIER;
            tv.nsec = (time % (TIME_THOUSANDS_MULTIPLIER * TIME_THOUSANDS_MULTIPLIER)) * 1000; // 转换为纳秒
            TFW_Cond_Wait(&context->cond, &context->lock, &tv);
            (void)TFW_Mutex_Unlock(&context->lock);
            continue;
        }

        if (msg == NULL) {
            (void)TFW_Mutex_Unlock(&context->lock);
            continue;
        }
        context->currentMsg = msg;
        (void)TFW_Mutex_Unlock(&context->lock);
        if (looper->dumpable) {
            TFW_LOGD_UTILS(
                "LoopTask HandleMessage message. name=%s, handle=%s, what=%d",
                context->name, msg->handler ? msg->handler->name : "null", msg->what);
        }

        if (msg->handler != NULL && msg->handler->HandleMessage != NULL) {
            msg->handler->HandleMessage(msg);
        }
        if (looper->dumpable) {
            TFW_LOGD_UTILS(
                "LoopTask after HandleMessage message. "
                "name=%s, what=%d, arg1=%llu",
                context->name, msg->what, msg->arg1);
        }
        (void)TFW_Mutex_Lock(&context->lock);
        FreeTFWMsg(msg);
        context->currentMsg = NULL;
        (void)TFW_Mutex_Unlock(&context->lock);
    }
    (void)TFW_Mutex_Lock(&context->lock);
    context->running = 0;
    TFW_LOGI_UTILS("LoopTask running is 0. name=%s", context->name);
    // 使用条件变量广播通知，优化TFW_DestroyLooper中的等待逻辑
    TFW_Cond_Broadcast(&context->condRunning);
    (void)TFW_Mutex_Unlock(&context->lock);
    return NULL;
}

static int32_t StartNewLooperThread(TFW_Looper *looper)
{
    TFW_ThreadAttr attr;
    TFW_ThreadAttr_Init(&attr);
    attr.name = looper->context->name;

    TFW_Thread_t tid;
    int32_t ret = TFW_Thread_Create(&tid, &attr, LoopTask, looper);
    if (ret != TFW_SUCCESS) {
        TFW_LOGE_UTILS("Init LoopTask Thread failed");
        return ret;
    }

    TFW_LOGI_UTILS("loop thread creating. name=%s", looper->context->name);
    return TFW_SUCCESS;
}

static int32_t PostMessageAtTimeParamVerify(const TFW_Looper *looper, TFW_Message *msgPost)
{
    if (msgPost == NULL) {
        TFW_LOGE_UTILS("the msgPost param is null.");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (looper == NULL) {
        TFW_LOGE_UTILS("the looper param is null.");
        return TFW_ERROR_INVALID_PARAM;
    }

    if (looper->dumpable) {
        TFW_LOGD_UTILS("PostMessageAtTime name=%s, what=%d, time=%lldus",
            looper->context->name, msgPost->what, msgPost->time);
    }

    if (msgPost->handler == NULL) {
        TFW_LOGE_UTILS("[%s] msg handler is null", looper->context->name);
        return TFW_ERROR_LOOPER_ERROR;
    }

    return TFW_SUCCESS;
}

static void PostMessageAtTime(const TFW_Looper *looper, TFW_Message *msgPost)
{
    if (PostMessageAtTimeParamVerify(looper, msgPost) != 0) {
        FreeTFWMsg(msgPost);
        return;
    }

    TFW_MessageNode *newNode = (TFW_MessageNode *)TFW_Calloc(sizeof(TFW_MessageNode));
    if (newNode == NULL) {
        TFW_LOGE_UTILS("message node malloc failed.");
        FreeTFWMsg(msgPost);
        return;
    }
    TFW_ListInit(&newNode->node);
    newNode->msg = msgPost;
    TFW_LooperContext *context = looper->context;
    if (TFW_Mutex_Lock(&context->lock) != 0) {
        TFW_Free(newNode);
        FreeTFWMsg(msgPost);
        return;
    }
    if (context->stop == 1) {
        TFW_Free(newNode);
        FreeTFWMsg(msgPost);
        (void)TFW_Mutex_Unlock(&context->lock);
        TFW_LOGE_UTILS("PostMessageAtTime stop is 1. name=%s, running=%d",
            context->name, context->running);
        return;
    }
    TFW_ListNode *item = NULL;
    TFW_ListNode *nextItem = NULL;
    bool insert = false;
    TFW_LIST_FOR_EACH_SAFE(item, nextItem, &context->msgHead) {
        TFW_MessageNode *itemNode = TFW_LIST_ENTRY(item, TFW_MessageNode, node);
        TFW_Message *msg = itemNode->msg;
        if (msg->time > msgPost->time) {
            TFW_ListTailInsert(item, &(newNode->node));
            insert = true;
            break;
        }
    }
    if (!insert) {
        TFW_ListTailInsert(&(context->msgHead), &(newNode->node));
    }
    context->msgSize++;
    if (looper->dumpable) {
        TFW_LOGD_UTILS("PostMessageAtTime insert. name=%s", context->name);
        DumpLooperLocked(looper);
    }
    TFW_Cond_Broadcast(&context->cond);
    (void)TFW_Mutex_Unlock(&context->lock);
}

static void LooperPostMessage(const TFW_Looper *looper, TFW_Message *msg)
{
    if (msg == NULL) {
        TFW_LOGE_UTILS("LooperPostMessage with nullmsg");
        return;
    }
    if (looper == NULL) {
        TFW_LOGE_UTILS("LooperPostMessage with nulllooper");
        return;
    }
    msg->time = UptimeMicros();
    PostMessageAtTime(looper, msg);
}

static void LooperPostMessageDelay(const TFW_Looper *looper, TFW_Message *msg, uint64_t delayMillis)
{
    if (msg == NULL) {
        TFW_LOGE_UTILS("LooperPostMessageDelay with nullmsg");
        return;
    }
    if (looper == NULL) {
        TFW_LOGE_UTILS("LooperPostMessageDelay with nulllooper");
        return;
    }
    msg->time = UptimeMicros() + (int64_t)delayMillis * TIME_THOUSANDS_MULTIPLIER;
    PostMessageAtTime(looper, msg);
}

static int32_t WhatRemoveFunc(const TFW_Message *msg, void *args)
{
    int32_t what = (int32_t)(intptr_t)args;
    if (msg->what == what) {
        return 0;
    }
    return 1;
}

static void LoopRemoveMessageCustom(const TFW_Looper *looper, const TFW_Handler *handler,
    int32_t (*customFunc)(const TFW_Message*, void*), void *args)
{
    TFW_LooperContext *context = looper->context;
    if (TFW_Mutex_Lock(&context->lock) != 0) {
        return;
    }
    if (context->running == 0 || context->stop == 1) {
        (void)TFW_Mutex_Unlock(&context->lock);
        return;
    }
    TFW_ListNode *item = NULL;
    TFW_ListNode *nextItem = NULL;
    TFW_LIST_FOR_EACH_SAFE(item, nextItem, &context->msgHead) {
        TFW_MessageNode *itemNode = TFW_LIST_ENTRY(item, TFW_MessageNode, node);
        TFW_Message *msg = itemNode->msg;
        if (msg->handler == handler && customFunc(msg, args) == 0) {
            TFW_LOGD_UTILS(
                "LooperRemoveMessage. name=%s, handler=%s, what=%d, arg1=%llu, "
                "time=%lld",
                context->name, handler->name, msg->what, msg->arg1, msg->time);
            FreeTFWMsg(msg);
            TFW_ListDelete(&itemNode->node);
            TFW_Free(itemNode);
            context->msgSize--;
        }
    }
    (void)TFW_Mutex_Unlock(&context->lock);
}

static void LooperRemoveMessage(const TFW_Looper *looper, const TFW_Handler *handler, int32_t what)
{
    LoopRemoveMessageCustom(looper, handler, WhatRemoveFunc, (void*)(intptr_t)what);
}

void TFW_SetLooperDumpable(TFW_Looper *loop, bool dumpable)
{
    if (loop == NULL) {
        TFW_LOGE_UTILS("loop is null");
        return;
    }
    loop->dumpable = dumpable;
}

TFW_Looper *TFW_CreateNewLooper(const char *name)
{
    if (g_looperCnt >= (MAX_LOOPER_CNT)) {
        TFW_LOGE_UTILS("Looper exceeds the maximum, count=%u,", g_looperCnt);
        return NULL;
    }

    TFW_Looper *looper = (TFW_Looper *)TFW_Calloc(sizeof(TFW_Looper));
    if (looper == NULL) {
        TFW_LOGE_UTILS("Looper TFW_Calloc fail");
        return NULL;
    }

    TFW_LooperContext *context = (TFW_LooperContext *)TFW_Calloc(sizeof(TFW_LooperContext));
    if (context == NULL) {
        TFW_LOGE_UTILS("Looper TFW_Calloc fail");
        TFW_Free(looper);
        return NULL;
    }

    // 使用项目中的安全字符串复制函数替换securec.h中的strcpy_s
    if (TFW_Strcpy_S(context->name, sizeof(context->name), name) != 0) {
        TFW_LOGE_UTILS("strcpy_s fail");
        TFW_Free(looper);
        TFW_Free(context);
        return NULL;
    }
    TFW_ListInit(&context->msgHead);
    // init context
    TFW_MutexAttr_Init(&context->attr);
    TFW_Mutex_Init(&context->lock, &context->attr);
    // 初始化条件变量
    TFW_Cond_Init(&context->cond);
    TFW_Cond_Init(&context->condRunning);

    // init looper
    context->stop = 0;
    context->running = 0;
    context->currentMsg = NULL;
    context->msgSize = 0;

    looper->context = context;
    looper->dumpable = true;
    looper->PostMessage = LooperPostMessage;
    looper->PostMessageDelay = LooperPostMessageDelay;
    looper->RemoveMessage = LooperRemoveMessage;
    looper->RemoveMessageCustom = LoopRemoveMessageCustom;

    int32_t ret = StartNewLooperThread(looper);
    if (ret != 0) {
        TFW_LOGE_UTILS("start fail");
        TFW_Free(looper);
        TFW_Free(context);
        return NULL;
    }

    g_looperCnt++;

    TFW_LOGD_UTILS("wait looper start ok. name=%s", context->name);
    return looper;
}

void TFW_DestroyLooper(TFW_Looper *looper)
{
    if (looper == NULL) {
        TFW_LOGE_UTILS("looper is null");
        return;
    }

    TFW_LooperContext *context = looper->context;
    if (context != NULL) {
        (void)TFW_Mutex_Lock(&context->lock);

        TFW_LOGI_UTILS("set stop 1. name=%s", context->name);
        context->stop = 1;

        TFW_Cond_Broadcast(&context->cond);
        (void)TFW_Mutex_Unlock(&context->lock);
        // 等待线程结束
        while (1) {
            (void)TFW_Mutex_Lock(&context->lock);
            TFW_LOGI_UTILS("get. name=%s, running=%d", context->name, context->running);
            if (context->running == 0) {
                (void)TFW_Mutex_Unlock(&context->lock);
                break;
            }
            // 使用条件变量等待替换轮询等待
            TFW_Cond_Wait(&context->condRunning, &context->lock, NULL);
            (void)TFW_Mutex_Unlock(&context->lock);
        }
        // release msg
        TFW_ListNode *item = NULL;
        TFW_ListNode *nextItem = NULL;
        TFW_LIST_FOR_EACH_SAFE(item, nextItem, &context->msgHead) {
            TFW_MessageNode *itemNode = TFW_LIST_ENTRY(item, TFW_MessageNode, node);
            TFW_Message *msg = itemNode->msg;
            FreeTFWMsg(msg);
            TFW_ListDelete(&itemNode->node);
            TFW_Free(itemNode);
        }
        TFW_LOGI_UTILS("destroy. name=%s", context->name);
        // destroy looper
        // 销毁条件变量
        TFW_Cond_Destroy(&context->cond);
        TFW_Cond_Destroy(&context->condRunning);
        TFW_Mutex_Destroy(&context->lock);
        TFW_Free(context);
        looper->context = NULL;
    }
    // 清理g_looperConfig数组中的引用
    ReleaseLooper(looper);

    TFW_Free(looper);
    if (g_looperCnt != 0) {
        g_looperCnt--;
    }
}

int32_t TFW_LooperInit(void)
{
    TFW_Looper *defaultLooper = TFW_CreateNewLooper(TFW_DEFAULT_LOOPER_NAME);
    if (!defaultLooper) {
        TFW_LOGE_UTILS("init default looper fail.");
        return TFW_ERROR_LOOPER_ERROR;
    }
    TFW_SetLooper(TFW_LOOP_TYPE_DEFAULT, defaultLooper);

    TFW_Looper *logLooper = TFW_CreateNewLooper(TFW_LOG_LOOPER_NAME);
    if (!logLooper) {
        TFW_LOGE_UTILS("init log looper fail.");
        return TFW_ERROR_LOOPER_ERROR;
    }
    TFW_SetLooper(TFW_LOOP_TYPE_LOG, logLooper);

    TFW_LOGD_UTILS("init looper success.");
    return TFW_SUCCESS;
}

void TFW_LooperDeinit(void)
{
    for (int32_t i = 0; i < (TFW_LOOP_TYPE_MAX); i++) {
        if (g_looperConfig[i].looper == NULL) {
            continue;
        }
        (void)TFW_Mutex_Lock(&(g_looperConfig[i].looper->context->lock));
        if (g_isThreadStarted == 0) {
            g_isNeedDestroy = 1;
            (void)TFW_Mutex_Unlock(&(g_looperConfig[i].looper->context->lock));
            return;
        }
        (void)TFW_Mutex_Unlock(&(g_looperConfig[i].looper->context->lock));
        TFW_DestroyLooper(g_looperConfig[i].looper);
    }
}
