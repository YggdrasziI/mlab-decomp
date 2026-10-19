// Game unit at 0x801091EC-0x80109C44 (real file name unknown): localized file names,
// the "shared_voice.sir" loader and a queue of up to 2000 texture requests read from
// "shared_texture.sir" into a 0xC0000-byte buffer. The inline constructor of the
// request (0x80109BE0, built by the static initializer with __construct_array) is
// emitted after the static initializer. Bounds come from the static initializers of
// this unit and the previous one.

#include "game/unk_vec3.mch"

#include "game/unk_vec3.h"

// File system object at 0x8048A8F0 (real type unknown).
struct UnkFileSystem;

// Reader built over the loaded buffer (real type unknown).
struct UnkSirReader {
    /* 0x0 */ u8 unk0[0xC];
};

// Loaded texture, reference counted (real type unknown).
struct UnkTexture {
    /* 0x00 */ u8 unk0[0x24];
    /* 0x24 */ int mRefs;
};

extern "C" {
int fn_80142EF8(char* buf, const char* format, ...);
void fn_8016964C(void* owner, UnkTexture* texture);
UnkTexture* fn_8016A08C(const char* name);
void fn_8016B5E0(UnkSirReader* reader, void* buffer, int offset);
UnkTexture* fn_8016B724(UnkSirReader* reader, int arg, const char* name);
const char* fn_801D8ADC(const char* name);
int fn_801E5F48(UnkFileSystem* fs, const char* name, int dir);
u32 fn_801E6138(UnkFileSystem* fs, const char* name, void* buffer, u32 size, int dir);
int fn_801E6BD0(UnkFileSystem* fs, u32* handle);
int fn_801E6C44(UnkFileSystem* fs, u32* handle);
u32 fn_801E6EB0(UnkFileSystem* fs);
int fn_801EB704(const char* name);
void* MEMAllocFromAllocator(void* allocator, u32 size);
void MEMFreeToAllocator(void* allocator, void* block);

int fn_801091EC(char* buf, const char* name, int language);
u32 fn_80109278(const char* name, void* buffer, u32 size);
UnkTexture* fn_80109330(const char* name);
void fn_801094F4(const char* name, int arg);
int fn_80109600(u32* request);
int fn_801098DC(u32 id);
int fn_80109A0C(u32 id);
void fn_80109A20(void);
void fn_80109AE0(void);
}

extern UnkFileSystem lbl_8048A8F0;
extern u8 lbl_803F2870[]; // heap allocator
extern void* lbl_805FB5A8;

// Texture request: the file system handle of the pending read (fn_801E6EB0 returns
// the "none" value), the offset in the archive, the texture name, the directory and a
// user value (real names unknown).
struct UnkTextureRequest {
    UnkTextureRequest() {
        mHandle = fn_801E6EB0(&lbl_8048A8F0);
        mOffset = 0;
        mDir = 0;
        fn_80142EF8(mName, "");
        mArg = 0;
    }

    /* 0x00 */ u32 mHandle;
    /* 0x04 */ int mOffset;
    /* 0x08 */ char mName[0x30];
    /* 0x38 */ int mDir;
    /* 0x3C */ int mArg;
};

static UnkTextureRequest lbl_803C75C8[2000];

static void* lbl_805FB220; // texture buffer
static void* lbl_805FB224; // allocated block of the buffer
static u8 lbl_805FB228; // a read is pending
static u32 lbl_805FB22C; // request being processed
static u32 lbl_805FB230; // next free request
static u32 lbl_805FB234; // number of queued requests

inline UnkTextureRequest& UnkRequest(u32 i) {
    return lbl_803C75C8[i];
}

inline void UnkResetRequest(UnkTextureRequest* request) {
    request->mHandle = fn_801E6EB0(&lbl_8048A8F0);
    request->mOffset = 0;
    request->mDir = 0;
    fn_80142EF8(request->mName, "");
    request->mArg = 0;
}

inline void UnkInitRequests() {
    lbl_805FB230 = 1;
    lbl_805FB22C = 1;
    lbl_805FB234 = 0;
    lbl_805FB228 = 0;
    for (int i = 0; i < 2000; i++) {
        UnkResetRequest(&lbl_803C75C8[i]);
    }
    if (lbl_805FB220 == 0) {
        lbl_805FB220 = lbl_805FB224 = MEMAllocFromAllocator(lbl_803F2870, 0xC0000);
    }
}

// Never called, so not emitted; it only gives the string pools their order in the
// DOL (hypothesis: the original file used these names before the functions below).
static void UnusedArchiveNames(char* buf) {
    fn_80142EF8(buf, "%s.sir", "shared_voice");
    fn_80142EF8(buf, "shared_voice.sir", "shared_texture");
}

// Appends the language suffix to a file name.
int fn_801091EC(char* buf, const char* name, int language) {
    switch (language) {
    case 1:
        return fn_80142EF8(buf, "%s%s", name, "_FR");
    case 3:
        return fn_80142EF8(buf, "%s%s", name, "_IT");
    case 2:
        return fn_80142EF8(buf, "%s%s", name, "_GE");
    case 4:
        return fn_80142EF8(buf, "%s%s", name, "_SP");
    case 0:
    default:
        return fn_80142EF8(buf, "%s", name);
    }
}

u32 fn_80109278(const char* name, void* buffer, u32 size) {
    fn_801E6EB0(&lbl_8048A8F0);
    int dir = fn_801EB704(name);
    if (fn_801E5F48(&lbl_8048A8F0, "shared_voice.sir", dir) == 0) {
        return fn_801E6EB0(&lbl_8048A8F0);
    }
    char path[0x80];
    fn_80142EF8(path, "%s.sir", "shared_voice");
    return fn_801E6138(&lbl_8048A8F0, path, buffer, size, dir);
}

// 96.9 %: register numbering (dir, buffer) and the scheduling of the sprintf
// arguments.
// Loads a texture now (or takes a reference to the loaded one).
UnkTexture* fn_80109330(const char* name) {
    int dir = fn_801EB704(name);
    char archive[0x80];
    fn_80142EF8(archive, "%s.sir", "shared_texture");
    int offset = fn_801E5F48(&lbl_8048A8F0, archive, dir);
    name = fn_801D8ADC(name);
    UnkTexture* texture = fn_8016A08C(name);
    if (texture != 0) {
        texture->mRefs++;
        return texture;
    }
    if (lbl_805FB220 == 0) {
        UnkInitRequests();
    }
    void* buffer = lbl_805FB220;
    char path[0x80];
    fn_80142EF8(path, "%s.sir", "shared_texture");
    u32 handle = fn_801E6138(&lbl_8048A8F0, path, buffer, 0xC0000, dir);
    int loaded;
    if (handle == 0xFFFFFFFF) {
        loaded = 0;
    } else {
        u32 wait = handle;
        fn_801E6C44(&lbl_8048A8F0, &wait);
        loaded = 1;
    }
    if (loaded) {
        UnkSirReader reader;
        fn_8016B5E0(&reader, lbl_805FB220, offset);
        texture = fn_8016B724(&reader, 0, name);
        if (texture != 0) {
            fn_8016964C(lbl_805FB5A8, texture);
        }
    }
    return texture;
}

// 97.2 %: the scheduling of the first sprintf arguments.
// Queues a texture request.
void fn_801094F4(const char* name, int arg) {
    int dir = fn_801EB704(name);
    char archive[0x80];
    fn_80142EF8(archive, "%s.sir", "shared_texture");
    int offset = fn_801E5F48(&lbl_8048A8F0, archive, dir);
    const char* textureName = fn_801D8ADC(name);
    UnkTexture* texture = fn_8016A08C(textureName);
    if (texture != 0) {
        texture->mRefs++;
    } else if (lbl_805FB234 < 1999) {
        u32 none = fn_801E6EB0(&lbl_8048A8F0);
        lbl_803C75C8[lbl_805FB230].mHandle = none;
        fn_80142EF8(lbl_803C75C8[lbl_805FB230].mName, textureName);
        lbl_803C75C8[lbl_805FB230].mDir = dir;
        lbl_803C75C8[lbl_805FB230].mArg = arg;
        lbl_803C75C8[lbl_805FB230].mOffset = offset;
        lbl_805FB234++;
        if (++lbl_805FB230 >= 2000) {
            lbl_805FB230 = 1;
        }
    }
}

inline int UnkCursorFree() {
    int free = 0;
    if (lbl_803C75C8[lbl_805FB22C].mDir == 0 && lbl_803C75C8[lbl_805FB22C].mHandle == fn_801E6EB0(&lbl_8048A8F0)) {
        free = 1;
    }
    return free;
}

// 91.6 %: in the free-slot test the DOL reloads the cursor after fn_801E6EB0 and
// recomputes the entry address; this build keeps the address across the call
// (non-static globals, accessor, operand order tried).
// Processes the queue; *request is a request id (<= 2000) until the texture is
// loaded, then the texture.
int fn_80109600(u32* request) {
    if (lbl_805FB234 != 0) {
        while (UnkCursorFree()) {
            if (++lbl_805FB22C >= 2000) {
                lbl_805FB22C = 1;
            }
        }
        if (!lbl_805FB228) {
            UnkTexture* texture = fn_8016A08C(lbl_803C75C8[lbl_805FB22C].mName);
            if (texture != 0) {
                texture->mRefs++;
                *request = (u32)texture;
                UnkResetRequest(&lbl_803C75C8[lbl_805FB22C]);
                if (++lbl_805FB22C >= 2000) {
                    lbl_805FB22C = 1;
                }
                lbl_805FB228 = 0;
                lbl_805FB234--;
                return 1;
            }
            void* buffer = lbl_805FB220;
            int dir = lbl_803C75C8[lbl_805FB22C].mDir;
            char path[0x80];
            fn_80142EF8(path, "%s.sir", "shared_texture");
            u32 handle = fn_801E6138(&lbl_8048A8F0, path, buffer, 0xC0000, dir);
            lbl_805FB228 = 1;
            lbl_803C75C8[lbl_805FB22C].mHandle = handle;
        }
    }
    if (*request > 2000) {
        return 1;
    }
    if (lbl_805FB22C != *request) {
        return 0;
    }
    u32 handle = lbl_803C75C8[lbl_805FB22C].mHandle;
    if (fn_801E6BD0(&lbl_8048A8F0, &handle) == 0) {
        return 0;
    }
    UnkSirReader reader;
    fn_8016B5E0(&reader, lbl_805FB220, lbl_803C75C8[lbl_805FB22C].mOffset);
    UnkTexture* texture = fn_8016B724(&reader, 0, lbl_803C75C8[lbl_805FB22C].mName);
    *request = (u32)texture;
    if (texture != 0) {
        fn_8016964C(lbl_805FB5A8, texture);
    }
    UnkResetRequest(&lbl_803C75C8[lbl_805FB22C]);
    if (++lbl_805FB22C >= 2000) {
        lbl_805FB22C = 1;
    }
    lbl_805FB228 = 0;
    lbl_805FB234--;
    return 1;
}

// 79.8 %: the DOL indexes the array again after each call (base reloaded, index
// kept), this build keeps the entry address in a saved register (index helper,
// reference helper, local for the "none" handle tried).
// Cancels a request.
int fn_801098DC(u32 id) {
    if (id != 0 && id <= 2000) {
        u32 none = fn_801E6EB0(&lbl_8048A8F0);
        if (UnkRequest(id).mHandle == none) {
            UnkRequest(id).mHandle = fn_801E6EB0(&lbl_8048A8F0);
            UnkRequest(id).mOffset = 0;
            UnkRequest(id).mDir = 0;
            fn_80142EF8(UnkRequest(id).mName, "");
            UnkRequest(id).mArg = 0;
            lbl_805FB234--;
        } else {
            u32 handle = UnkRequest(id).mHandle;
            if (fn_801E6BD0(&lbl_8048A8F0, &handle) == 0) {
                u32 wait = UnkRequest(id).mHandle;
                fn_801E6C44(&lbl_8048A8F0, &wait);
            }
            UnkRequest(id).mHandle = fn_801E6EB0(&lbl_8048A8F0);
            UnkRequest(id).mOffset = 0;
            UnkRequest(id).mDir = 0;
            fn_80142EF8(UnkRequest(id).mName, "");
            UnkRequest(id).mArg = 0;
            lbl_805FB234--;
            lbl_805FB228 = 0;
        }
    }
    return 0;
}

int fn_80109A0C(u32 id) {
    return id > 2000;
}

void fn_80109A20(void) {
    lbl_805FB230 = 1;
    lbl_805FB22C = 1;
    lbl_805FB234 = 0;
    lbl_805FB228 = 0;
    for (int i = 0; i < 2000; i++) {
        UnkResetRequest(&lbl_803C75C8[i]);
    }
    if (lbl_805FB220 == 0) {
        lbl_805FB220 = lbl_805FB224 = MEMAllocFromAllocator(lbl_803F2870, 0xC0000);
    }
}

void fn_80109AE0(void) {
    lbl_805FB230 = 1;
    lbl_805FB22C = 1;
    lbl_805FB234 = 0;
    lbl_805FB228 = 0;
    for (int i = 0; i < 2000; i++) {
        UnkResetRequest(&lbl_803C75C8[i]);
    }
    if (lbl_805FB220 != 0) {
        MEMFreeToAllocator(lbl_803F2870, lbl_805FB224);
        lbl_805FB220 = 0;
        lbl_805FB224 = 0;
    }
}
