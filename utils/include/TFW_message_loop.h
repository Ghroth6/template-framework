#ifndef TFW_MESSAGE_LOOP_H
#define TFW_MESSAGE_LOOP_H

#include <stdbool.h>
#include <stdint.h>

#include "TFW_list.h"
#include "TFW_thread.h"

#ifdef __cplusplus
extern "c" {
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

TFW_Message *TFW_MallocMessage(void);

void TFW_FreeMessage(TFW_Message *msg);

TFW_Looper *TFW_CreateNewLooper(const char *name);

void TFW_DestroyLooper(TFW_Looper *looper);

void TFW_SetLooperDumpable(TFW_Looper *loop, bool dumpable);

#ifdef __cplusplus
}
#endif

#endif /* TFW_MESSAGE_LOOP_H */