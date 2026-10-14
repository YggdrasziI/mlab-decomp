#ifndef RVL_SDK_ESP_H
#define RVL_SDK_ESP_H
#include <revolution/types.h>

#include <revolution/arc.h>
#include <revolution/ipc.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ES_IOCTLV_LAUNCH_TITLE = 8,
    ES_IOCTLV_READ_CONTENT_FILE = 10,
    ES_IOCTLV_GET_NUM_TICKET_VIEWS = 18,
    ES_IOCTLV_GET_TICKET_VIEWS = 19,
    ES_IOCTLV_GET_DATA_DIR = 29,
    ES_IOCTLV_GET_TITLE_ID = 32,
    ES_IOCTLV_SEEK_CONTENT_FILE = 35,
} ESIoctl;

typedef struct ESPTicket {
    u8 dummy[0x2A4];
} ESPTicket;

typedef struct ESPTmd {
    u8 dummy[0x49E4];
} ESPTmd;

// Ticket types and functions below are from Petari's revolution/esp.h (CC0).
typedef u32 ESId;
typedef u64 ESTitleId;
typedef u64 ESTicketId;
typedef u8 ESVersion;
typedef u8 ESTicketReserved[48];
typedef u8 ESSysAccessMask[2];
typedef u8 ESCidxMask[64];
typedef u32 ESContentId;
typedef u16 ESTitleVersion;
typedef ESTitleId ESSysVersion;
typedef u32 ESTitleType;
typedef u16 ESContentType;
typedef u8 ESTmdReserved[62];

#pragma pack(push, 4)

typedef struct ESLpEntry {
    u32 code;
    u32 limit;
} ESLpEntry;

typedef struct ESTicketView {
    ESVersion version;
    ESTicketId ticketID;
    ESId devID;
    ESTitleId titleID;
    ESSysAccessMask sysAccessMask;
    u16 ticketVer;
    u32 accTitleID;
    u32 accTitleMask;
    u8 license;
    ESTicketReserved reserved;
    u8 audit;
    ESCidxMask cidxMask;
    ESLpEntry limits[8];
} ESTicketView;

typedef struct ESTmdViewHeader {
    ESVersion version;
    ESSysVersion sysVersion;
    ESTitleId titleId;
    ESTitleType type;
    u16 groupId;
    ESTmdReserved reserved;
    ESTitleVersion titleVersion;
    u16 numContents;
} ESTmdViewHeader;

typedef struct ESCmdView {
    ESContentId cid;
    u16 index;
    ESContentType type;
    u64 size;
} ESCmdView;

typedef struct ESTmdView {
    ESTmdViewHeader head;
    ESCmdView contents[512];
} ESTmdView;

#pragma pack(pop)

s32 ESP_InitLib(void);
s32 ESP_CloseLib(void);
s32 ESP_DiGetTicketView(const void* ticket, ESTicketView* view);
s32 ESP_DiGetTmd(ESPTmd* tmd, u32* tmdSize);
s32 ESP_GetTmdView(ESTitleId titleId, ESTmdView* tmdView, u32* size);
s32 ESP_LaunchTitle(ESTitleId titleId, ESTicketView* ticketView);
s32 ESP_GetTicketViews(ESTitleId titleId, ESTicketView* ticketViews,
                       u32* count);
s32 ESP_GetTitleId(ESTitleId* titleId);
s32 ESP_GetDataDir(ESTitleId titleId, char* dataDir);
s32 ESP_GetConsumption(ESTicketId ticketId, ESLpEntry* entries, u32* count);

s32 ESP_ReadContentFile(s32 fd, void* dst, u32 len);
s32 ESP_SeekContentFile(s32 fd, s32 offset, s32 origin);

#ifdef __cplusplus
}
#endif
#endif
