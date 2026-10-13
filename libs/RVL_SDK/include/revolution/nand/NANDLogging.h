#ifndef RVL_SDK_NAND_LOGGING_H
#define RVL_SDK_NAND_LOGGING_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NANDLoggingCallback)(BOOL);

BOOL NANDLoggingAddMessageAsync(NANDLoggingCallback cb, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
