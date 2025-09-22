#ifndef TFW_MESSAGE_LOOP_H
#define TFW_MESSAGE_LOOP_H

#include <stdbool.h>
#include <stdint.h>

#include "TFW_list.h"
#include "TFW_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TFW_Message TFW_Message;
typedef struct TFW_Handler TFW_Handler;
typedef struct TFW_LooperContext TFW_LooperContext;
typedef struct TFW_Looper TFW_Looper;

struct TFW_Looper {
    TFW_LooperContext *context;
    bool dumpable;
    void (*PostMessage)(const TFW_Looper *looper, TFW_Message *msg);
    void (*PostMessageDelay)(const TFW_Looper *looper, TFW_Message *msg, uint64_t delayMillis);
    void (*RemoveMessage)(const TFW_Looper *looper, const TFW_Handler *handler, int32_t what);
    // customFunc, when match, return 0
    void (*RemoveMessageCustom)(const TFW_Looper *looper, const TFW_Handler *handler,
        int32_t (*)(const TFW_Message*, void*), void *args);
};

struct TFW_Handler {
    char *name;
    TFW_Looper *looper;
    void (*HandleMessage)(TFW_Message *msg);
};

struct TFW_Message {
    int32_t what;
    uint64_t arg1;
    uint64_t arg2;
    int64_t time;
    void *obj;
    TFW_Handler *handler;
    void (*FreeMessage)(TFW_Message *msg);
};

// 消息循环类型枚举
typedef enum {
    TFW_LOOP_TYPE_DEFAULT,
    TFW_LOOP_TYPE_LOG,
    TFW_LOOP_TYPE_MAX
} TFW_LooperType;

// 消息循环名称宏定义
#define TFW_DEFAULT_LOOPER_NAME "TFW_Default_Lp"
#define TFW_LOG_LOOPER_NAME "TFW_Log_Lp"

TFW_Message *TFW_MallocMessage(void);

void TFW_FreeMessage(TFW_Message *msg);

TFW_Looper *TFW_CreateNewLooper(const char *name);

void TFW_DestroyLooper(TFW_Looper *looper);

void TFW_SetLooperDumpable(TFW_Looper *loop, bool dumpable);

TFW_Looper *TFW_GetLooper(TFW_LooperType type);

void TFW_SetLooper(TFW_LooperType type, TFW_Looper *looper);

int32_t TFW_LooperInit(void);

void TFW_LooperDeinit(void);

#ifdef __cplusplus
}
#endif

#endif /* TFW_MESSAGE_LOOP_H */
