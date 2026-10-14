/* Body from Petari (src/RVL_SDK/os/OSExec.c, the RMGK01 build), with our headers */
#include <decomp.h>
#include <revolution/dvd/dvd.h>
#include <revolution/esp/esp.h>
#include <revolution/gx/GXTypes.h>
#include <revolution/ipc.h>
#include <revolution/os.h>
#include <revolution/os/OSPlayTime.h>
#include <stdio.h>
#include <string.h>
#include <wstring.h>

#define OS_INTERRUPTMASK_PI_ACR OS_INTR_MASK(OS_INTR_PI_ACR)
/* MEM | DSP | AI | EXI | PI */
#define OS_INTERRUPTMASK_ALL_BUT_PI_ACR 0xFFFFFFF0

/* DVDLow functions as dvd_broadway.c defines them in this SDK */
typedef void (*DVDLowCallback)(u32 intType);
u8 DVDLowInit(void);
u8 DVDLowFinalize(void);
u8 DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback);
u8 DVDLowOpenPartition(u32 offset, const ESPTicket* ticket, u32 numCertBytes,
                       const u8* certs, ESPTmd* tmd, DVDLowCallback callback);
u8 DVDLowOpenPartitionWithTmdAndTicketView(u32 offset,
                                           const ESTicketView* ticketView,
                                           u32 numTmdBytes, const ESPTmd* tmd,
                                           u32 numCertBytes, const u8* certs,
                                           DVDLowCallback callback);
u8 DVDLowClosePartition(DVDLowCallback callback);
u8 DVDLowUnencryptedRead(void* dst, u32 size, u32 offset,
                         DVDLowCallback callback);

void __DVDPrepareResetAsync(DVDCommandCallback callback);
void __OSHotResetForError(void);
void IPCReInit(void);
s32 IPCCltReInit(void);

typedef struct DVDGameTOC {
    u32 numGamePartitions;
    u32 partitionInfos;
} DVDGameTOC;

typedef struct DVDPartitionInfo {
    u32 gamePartition;
    u32 type;
} DVDPartitionInfo;

/* the start of Petari's ESTitleMeta, up to head.sysVersion */
#pragma pack(push, 4)
typedef struct ESTitleMetaStart {
    u8 sig[0x180];
    u8 version;
    u8 caCrlVersion;
    u8 signerCrlVersion;
    u8 padding;
    ESSysVersion sysVersion;
} ESTitleMetaStart;
#pragma pack(pop)

static volatile BOOL Prepared;
BOOL __OSInReboot;
static volatile u32 DVDLowIntType;
static DVDDiskID id ALIGN(32);
u32 __OSNextPartitionType = 0;

OSExecParams* __OSExecParamsAddr AT_ADDRESS(0x800030F0);
s32 __OSCurrentTGCOffset AT_ADDRESS(0x800030F4);
vu32 __OSLaunchPartitionType AT_ADDRESS(0x80003194);
vu8 __OSLockedFlag AT_ADDRESS(0x80003187);

static BOOL PackArgs(void* addr, s32 argc, char* argv[]) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)&bootInfo2[8] = 0;
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;
        while (--argc >= 0) {
            ptr -= (strlen(argv[argc]) + 1);
            strcpy(ptr, argv[argc]);
            argv[argc] = (char*)(ptr - bootInfo2);
        }

        list = (char**)(bootInfo2 + ((ptr - bootInfo2) & ~3));
        list -= numArgs + 1;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        *(s32*)(list - 1) = numArgs;
        *(u32*)&bootInfo2[8] = (u32)((char*)(list - 1) - bootInfo2);
    }

    return TRUE;
}

static BOOL Utf16ToArg(char* dstArg, u16* srcName) {
    char *srcPtr, *dstPtr;
    u8 i, mask, shift;

    if (srcName != 0) {
        srcPtr = (char*)srcName;
        dstPtr = dstArg;

        while (*srcPtr || *(srcPtr + 1)) {
            for (i = 0; i < 4; i++) {
                mask = (u8)((i & 0x1) ? 0xF : 0xF0);
                shift = (u8)((i & 0x1) ? 0 : 4);

                if (0 <= ((*srcPtr & mask) >> shift) &&
                    ((*srcPtr & mask) >> shift) < 0xA) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x30);
                } else if (0xA <= ((*srcPtr & mask) >> shift) &&
                           ((*srcPtr & mask) >> shift) < 0x10) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x57);
                } else {
                    return FALSE;
                }

                dstPtr++;

                if (i & 1) {
                    srcPtr++;
                }
            }
        }

        *dstPtr++ = 0;
        return TRUE;
    }

    return FALSE;
}

static BOOL PackInstallerArgs(void* addr, s32 argc, char* argv[]) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)&bootInfo2[8] = 0;
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;

        while (--argc >= 0) {
            if (argc < 2 || (argc % 2)) {
                char* val = argv[argc];
                ptr -= (strlen(argv[argc]) + 1);
                strcpy(ptr, val);
                argv[argc] = (char*)(ptr - bootInfo2);
            } else {
                u16* val = (u16*)argv[argc];
                ptr -= (wcslen(val) * 4 + 1);
                Utf16ToArg(ptr, val);
                argv[argc] = (char*)(ptr - bootInfo2);
            }
        }

        list = (char**)(bootInfo2 + ((ptr - bootInfo2) & ~3));
        list -= numArgs + 1;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        *(s32*)(list - 1) = numArgs;

        *(u32*)&bootInfo2[8] = (u32)((char*)(list - 1) - bootInfo2);
    }

    return TRUE;
}

// clang-format off
static asm void Run(register void* entryPoint) {
    fralloc
    bl      ICFlashInvalidate
    sync
    isync
    mtctr    entryPoint
    bctr
    frfree
    blr
}
// clang-format on

static void Callback(s32 result, DVDCommandBlock* block) {
    Prepared = TRUE;
}

void __OSGetExecParams(OSExecParams* params) {
    if (0x80000000 <= (u32)__OSExecParamsAddr) {
        memcpy(params, __OSExecParamsAddr, 0x1C);
    } else {
        params->valid = FALSE;
    }
}

static void __OSSetExecParams(const OSExecParams* params, OSExecParams* addr) {
    memcpy(addr, params, sizeof(OSExecParams));
    __OSExecParamsAddr = addr;
}

static void callback(u32 intType) {
    DVDLowIntType = intType;
}

void __OSLaunchNextFirmware(void) {
    u8 i;
    s32 rc = -1;
    u32 ticketCnt = 1;
    ESTicketView *t, *t2;
    ESSysVersion version = 0x0000000100000003;
    u32 simGddr3Size;
    u32 gddrProtAddr;

    DVDGameTOC* gameToc;
    DVDPartitionInfo* partInfo;
    DVDPartitionInfo* bootGameInfo;
    ESPTmd* tmd;
    u32 numTmdBytes = 0;
    DVDPartitionInfo* partInfoPtr;

    gameToc = (DVDGameTOC*)OSAllocFromMEM1ArenaLo(
        OSRoundUp32B(sizeof(DVDGameTOC)), 32);
    partInfo = (DVDPartitionInfo*)OSAllocFromMEM1ArenaLo(
        OSRoundUp32B(sizeof(DVDPartitionInfo) * 256), 32);
    tmd = (ESPTmd*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESPTmd)), 64);
    t2 = (ESTicketView*)OSAllocFromMEM1ArenaLo(
        OSRoundUp32B(sizeof(ESTicketView)), 32);

    if (__OSNextPartitionType == __OSLaunchPartitionType &&
        *(u32*)OSPhysicalToCached(0x3198)) {
        rc = ESP_InitLib();
        if (rc == 0) {
            rc = ESP_DiGetTicketView(NULL, t2);
        }
        if (rc == 0) {
            rc = ESP_DiGetTmd(NULL, &numTmdBytes);
        }
        if (rc == 0) {
            rc = ESP_DiGetTmd(tmd, &numTmdBytes);
        }
        ESP_CloseLib();

        if (OSPlayTimeIsLimited()) {
            __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
            u32 remain = 0xffffffff;

            __OSGetPlayTime(t2, &type, &remain);

            if (remain == 0) {
                __OSWriteExpiredFlag();
                __OSReturnToMenuForError();
            }
        }
    }

    if (rc == 0) {
        bootGameInfo = partInfo;
        bootGameInfo->type = *((u32*)OSPhysicalToCached(0x3194));
        bootGameInfo->gamePartition = *((u32*)OSPhysicalToCached(0x3198));
    } else {
        DVDLowIntType = 0;
        DVDLowClosePartition(callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType,
                     0x395);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowUnencryptedRead(gameToc, OSRoundUp32B(sizeof(DVDGameTOC)),
                              0x40000 >> 2, callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType,
                     0x39F);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowUnencryptedRead(partInfo,
                              OSRoundUp32B(sizeof(DVDPartitionInfo) * 256),
                              gameToc->partitionInfos, callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType,
                     0x3AB);
            __OSReturnToMenuForError();
        }

        bootGameInfo = NULL;
        partInfoPtr = partInfo;
        for (i = 0; i < gameToc->numGamePartitions; i++) {
            if (partInfoPtr->type == __OSNextPartitionType) {
                bootGameInfo = partInfoPtr;
            }

            partInfoPtr++;
        }

        if (!bootGameInfo) {
            gameToc++;
            partInfoPtr = partInfo + 4;
            for (i = 0; i < gameToc->numGamePartitions; i++) {
                if (partInfoPtr->type == __OSNextPartitionType) {
                    bootGameInfo = partInfoPtr;
                }

                partInfoPtr++;
            }
        }

        if (!bootGameInfo) {
            OSReport("\nOSExec(): The specified game doesn't exist in the "
                     "disc\n");
            __OSReturnToMenuForError();
        }

        *((u32*)OSPhysicalToCached(0x3194)) = bootGameInfo->type;
        *((u32*)OSPhysicalToCached(0x3198)) = bootGameInfo->gamePartition;

        DVDLowIntType = 0;
        if (__OSLockedFlag == 0x80) {
            DVDLowOpenPartitionWithTmdAndTicketView(bootGameInfo->gamePartition,
                                                    t2, numTmdBytes, tmd, 0,
                                                    NULL, callback);
        } else {
            DVDLowOpenPartition(bootGameInfo->gamePartition, NULL, 0, 0, tmd,
                                callback);
        }

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType,
                     0x3E8);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowClosePartition(callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType,
                     0x3F2);
            __OSReturnToMenuForError();
        }
    }

    version = ((ESTitleMetaStart*)tmd)->sysVersion;

    rc = ESP_InitLib();
    if (rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x3FD);
        __OSHotResetForError();
    }

    rc = ESP_GetTicketViews(version, NULL, &ticketCnt);
    if (ticketCnt != 1 || rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x406);
        __OSHotResetForError();
    }

    t = (ESTicketView*)OSAllocFromMEM1ArenaLo(
        OSRoundUp32B(sizeof(ESTicketView) * ticketCnt), 32);
    rc = ESP_GetTicketViews(version, t, &ticketCnt);
    if (rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x40F);
        __OSHotResetForError();
    }

    DVDLowFinalize();

    simGddr3Size = *(u32*)OSPhysicalToCached(0x311C);
    gddrProtAddr = *(u32*)OSPhysicalToCached(0x3120);
    DCStoreRange(OSPhysicalToCached(0x3100), 0x100);

    rc = ESP_LaunchTitle(version, t);
    if (rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x41F);
        __OSHotResetForError();
    }

    ESP_CloseLib();

    DCInvalidateRange(OSPhysicalToCached(0x3100), 0x100);

    if (simGddr3Size < *(u32*)OSPhysicalToCached(0x311C)) {
        *(u32*)OSPhysicalToCached(0x3120) =
            simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) -
                            *(u32*)OSPhysicalToCached(0x3120));
        *(u32*)OSPhysicalToCached(0x3128) =
            simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) -
                            *(u32*)OSPhysicalToCached(0x3128));
        *(u32*)OSPhysicalToCached(0x3130) =
            simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) -
                            *(u32*)OSPhysicalToCached(0x3130));
        *(u32*)OSPhysicalToCached(0x3134) =
            simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) -
                            *(u32*)OSPhysicalToCached(0x3134));
        *(u32*)OSPhysicalToCached(0x311C) = simGddr3Size;
    }

    if (gddrProtAddr < *(u32*)OSPhysicalToCached(0x3120)) {
        __OSInitMemoryProtection();
    }

    __OSInitIPCBuffer();
    IPCReInit();
    IPCCltReInit();

    DVDLowInit();
    DVDLowIntType = 0;
    DVDLowReadDiskID(&id, callback);

    while (!DVDLowIntType) {
    }

    if (DVDLowIntType != 1) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x44B);
        __OSReturnToMenuForError();
    }

    DVDLowIntType = 0;
    if (__OSLockedFlag == 0x80) {
        DVDLowOpenPartitionWithTmdAndTicketView(bootGameInfo->gamePartition, t2,
                                                numTmdBytes, tmd, 0, NULL,
                                                callback);
    } else {
        DVDLowOpenPartition(bootGameInfo->gamePartition, NULL, 0, 0, tmd,
                            callback);
    }

    while (!DVDLowIntType) {
    }

    if (DVDLowIntType != 1) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x462);
        __OSReturnToMenuForError();
    }
}

#define TITLE_ID ((((u64)1) << 32) | (2))

void __OSLaunchMenu(void) {
    u8 i;
    s32 rc;
    u32 ticketCnt = 1;
    ESTicketView* t;
    ESSysVersion version = 0x0000000100000003;

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);
    rc = ESP_InitLib();

    if (rc != 0) {
        return;
    }

    rc = ESP_GetTicketViews(TITLE_ID, NULL, &ticketCnt);

    if (ticketCnt != 1 || rc != 0) {
        return;
    }

    t = (ESTicketView*)OSAllocFromMEM1ArenaLo(
        OSRoundUp32B(sizeof(ESTicketView) * ticketCnt), 32);
    rc = ESP_GetTicketViews(TITLE_ID, t, &ticketCnt);

    if (rc != 0) {
        return;
    }

    rc = ESP_LaunchTitle(TITLE_ID, t);

    if (rc != 0) {
        return;
    }

    while (1) {
    }
}

typedef struct {
    char date[16];
    u32 entry;
    u32 size;
    u32 rebootSize;
    u32 reserved2;
} ImgHeader;

static inline void ReadDisc(void* addr, s32 length, u32 offset) {
    DVDCommandBlock block;
    GXColor bg = {0, 0, 0, 0};
    GXColor fg = {255, 255, 255, 0};

    DVDReadAbsAsyncPrio(&block, addr, length, offset, NULL, 0);

    while (!(DVDGetCommandBlockStatus(&block) == DVD_STATE_IDLE)) {
        if (DVDGetCommandBlockStatus(&block) > 2 ||
            DVDGetCommandBlockStatus(&block) < 0) {
            __OSReturnToMenuForError();
        }
    };
}

static u32 GetApploaderPosition(void) {
    static u32 apploaderPosition = 0;

    if (apploaderPosition) {
        return apploaderPosition;
    }

    if (__OSCurrentTGCOffset) {
        u32* tgcHeader;
        s32 apploaderOffsetInTGC;

        tgcHeader = OSAllocFromMEM1ArenaLo(64, 32);
        ReadDisc(tgcHeader, 64, (u32)(__OSCurrentTGCOffset >> 2));
        apploaderOffsetInTGC = *(s32*)((u32)tgcHeader + 56);
        apploaderPosition =
            (u32)((__OSCurrentTGCOffset + apploaderOffsetInTGC) >> 2);
    } else {
        apploaderPosition = (0x00002440 >> 2);
    }

    return apploaderPosition;
}

static ImgHeader* LoadApploader(void) {
    ImgHeader* header;

    header = OSAllocFromMEM1ArenaLo(sizeof(ImgHeader), 32);
    ReadDisc(header, sizeof(ImgHeader), GetApploaderPosition());
    ReadDisc((void*)0x81200000, (s32)OSRoundUp32B(header->size),
             GetApploaderPosition() + (u32)(sizeof(ImgHeader) >> 2));
    ICInvalidateRange((void*)0x81200000, OSRoundUp32B(header->size));

    return header;
}

typedef void (*AppInitFunc)(void (*report)(const char*, ...));
typedef BOOL (*AppGetNextFunc)(void**, u32*, u32*);
typedef void* (*AppGetEntryFunc)(void);

typedef void (*AppInterfaceFunc)(AppInitFunc*, AppGetNextFunc*,
                                 AppGetEntryFunc*);


#define NEWAPPLOADER_DATE "2004/02/01"

static BOOL IsNewApploader(ImgHeader* header) {
    if (strncmp((char*)header, NEWAPPLOADER_DATE, 10) > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void* LoadDol(const OSExecParams* params,
                     AppInterfaceFunc getInterface) {
    AppInitFunc appInit;
    AppGetNextFunc appGetNext;
    AppGetEntryFunc appGetEntry;
    void* addr;
    u32 length;
    u32 offset;
    OSExecParams* paramsWork;

    getInterface(&appInit, &appGetNext, &appGetEntry);
    paramsWork = OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);
    __OSSetExecParams(params, paramsWork);
    appInit(OSReport);
    OSSetArenaLo(paramsWork);

    while (appGetNext(&addr, &length, &offset)) {
        ReadDisc(addr, (s32)length, (u32)(offset >> __DVDLayoutFormat));
    }

    return appGetEntry();
}

static void StartDol(const OSExecParams* params, void* entry) {
    OSExecParams* paramsWork;
    paramsWork = OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);
    __OSSetExecParams(params, paramsWork);
    PI_HW_REGS[0x024 / 4] = 0x00000004 | 0x00000001 | 2;
    OSDisableInterrupts();
    Run(entry);
}

#define BOOT_REGION_START 0x812fdff0
#define BOOT_REGION_END 0x812fdfec

void __OSBootDolSimple(u32 doloffset, u32 restartCode, void* regionStart,
                       void* regionEnd, BOOL argsUseDefault, s32 argc,
                       char* argv[]) {
    OSExecParams* params;
    void* dolEntry;
    ImgHeader* header;
    DVDDiskID* diskId;

    OSDisableInterrupts();

    if (__OSInReboot) {
        __OSNextPartitionType = __OSLaunchPartitionType;
    }

    params = OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);

    params->valid = TRUE;
    params->restartCode = restartCode;
    params->regionStart = regionStart;
    params->regionEnd = regionEnd;

    params->argsUseDefault = argsUseDefault;
    if (!argsUseDefault) {
        params->argsAddr = OSAllocFromMEM1ArenaLo(0x2000, 1);
        if (__OSNextPartitionType == 2 && !__OSInReboot) {
            PackInstallerArgs(params->argsAddr, argc, argv);
        } else {
            PackArgs(params->argsAddr, argc, argv);
        }
    }

    DVDInit();
    DVDSetAutoInvalidation(TRUE);
    DVDResume();

    Prepared = FALSE;
    __DVDPrepareResetAsync(Callback);

    __OSMaskInterrupts(OS_INTERRUPTMASK_ALL_BUT_PI_ACR);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_ACR);
    OSEnableInterrupts();

    while (Prepared != TRUE) {
    }

    __OSLaunchNextFirmware();

    if (0xA0000000 == restartCode && !__OSInReboot) {
        ESTitleId titleId ALIGN(32);
        s32 rc;
        char* ptr;

        rc = ESP_InitLib();
        if (rc != 0) {
            return;
        }

        rc = ESP_GetTitleId(&titleId);
        if (rc != 0) {
            return;
        }

        rc = ESP_CloseLib();
        if (rc != 0) {
            return;
        }

        ptr = (char*)params->argsAddr + (u32)argv[1];
        snprintf(ptr, 16 + 1, "%016llx", titleId);
    }

    header = LoadApploader();

    if (IsNewApploader(header)) {
        if (doloffset == 0xffffffff) {
            doloffset = GetApploaderPosition() +
                        ((sizeof(ImgHeader) + header->size) >> 2);
        }

        params->bootDol = doloffset;
        dolEntry = LoadDol(params, (AppInterfaceFunc)header->entry);
        diskId = (DVDDiskID*)OSPhysicalToCached(0x0000);
        *((u32*)OSPhysicalToCached(0x3180)) = *((u32*)diskId->game);
        *((u8*)OSPhysicalToCached(0x3184)) = 0x80;
        StartDol(params, dolEntry);
    } else {
        *(void**)BOOT_REGION_START = regionStart;
        *(void**)BOOT_REGION_END = regionEnd;
        *(u8*)OSPhysicalToCached(0x30E2) = 1;

        ReadDisc((void*)0x81330000, (s32)OSRoundUp32B(header->rebootSize),
                 (u32)(GetApploaderPosition() +
                       ((sizeof(ImgHeader) + header->size) >> 2)));
        ICInvalidateRange((void*)0x81330000,
                          OSRoundUp32B(header->rebootSize));
        OSDisableInterrupts();
        ICFlashInvalidate();
        Run((void*)0x81330000);
    }
}

void __OSBootDol(u32 doloffset, u32 restartCode, char* const argv[]) {
    char doloffInString[20];
    s32 argvlen;
    char** argvToPass;
    s32 i;
    void* saveStart;
    void* saveEnd;

    OSGetSaveRegion(&saveStart, &saveEnd);
    sprintf(doloffInString, "%d", doloffset);

    argvlen = 0;
    if (argv) {
        while (argv[argvlen]) {
            argvlen++;
        }
    }

    argvlen += 1;
    argvToPass = OSAllocFromMEM1ArenaLo((argvlen + 1) * sizeof(char*), 1);
    argvToPass[0] = doloffInString;

    for (i = 1; i < argvlen; i++) {
        argvToPass[i] = argv[i - 1];
    }

    __OSBootDolSimple(0xffffffff, restartCode, saveStart, saveEnd, FALSE,
                      argvlen, argvToPass);
}
