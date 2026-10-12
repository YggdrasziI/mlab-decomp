#include <revolution/ipc.h>
#include <revolution/os.h>

static u8 Initialized = FALSE;
static void* IPCCurrentBufferLo = NULL;
static void* IPCCurrentBufferHi = NULL;
static void* IPCBufferLo = NULL;
static void* IPCBufferHi = NULL;

void IPCInit(void) {
    if (!Initialized) {
        IPCBufferHi = __OSGetIPCBufferHi();
        IPCBufferLo = __OSGetIPCBufferLo();
        IPCCurrentBufferHi = IPCBufferHi;
        IPCCurrentBufferLo = IPCBufferLo;
        Initialized = TRUE;
    }
}

// From zeldaret/tp libs/revolution/src/ipc/ipcMain.c (CC0-1.0, commit c8fa8c9e).
void IPCReInit(void) {
    Initialized = FALSE;
    IPCInit();
}

u32 IPCReadReg(s32 index) {
    return IPC_HW_REGS_PPC[index];
}

void IPCWriteReg(s32 index, u32 value) {
    IPC_HW_REGS_PPC[index] = value;
}

void* IPCGetBufferHi(void) {
    return IPCCurrentBufferHi;
}

void* IPCGetBufferLo(void) {
    return IPCCurrentBufferLo;
}

void IPCSetBufferLo(void* lo) {
    IPCCurrentBufferLo = lo;
}
