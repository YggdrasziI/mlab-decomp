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

#pragma pack(pop)

s32 ESP_InitLib(void);
s32 ESP_CloseLib(void);
s32 ESP_DiGetTicketView(const void* ticket, ESTicketView* view);
s32 ESP_DiGetTmd(ESPTmd* tmd, u32* tmdSize);
s32 ESP_GetTitleId(ESTitleId* titleId);
s32 ESP_GetDataDir(ESTitleId titleId, char* dataDir);
s32 ESP_GetConsumption(ESTicketId ticketId, ESLpEntry* entries, u32* count);

s32 ESP_ReadContentFile(s32 fd, void* dst, u32 len);
s32 ESP_SeekContentFile(s32 fd, s32 offset, s32 origin);

#ifdef __cplusplus
}
#endif
#endif
