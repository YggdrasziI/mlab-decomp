// Game unit at 0x801E47D8-0x801E54E4 (real file name unknown): a queue of DVD file
// requests (open, read, close) referenced by handles. Bounds: the previous unit's
// static initializer (0x801E47BC) ends at 0x801E47D8; this unit's static initializer
// (0x801E5488) registers the destructor of the global loader lbl_8048A8B4, the
// generated destructor that follows it (0x801E54A4) ends the unit. The DOL reaches
// the queue through the global object in some places and through `this` in others,
// so the source names lbl_8048A8B4 where the DOL does.

#include "types.h"

#include <string.h>

struct UnkDvdRequest;

// Runs one step of a request; returns 1 when it is done (real name unknown, unit
// 0x8020415C).
extern "C" int fn_80204210(UnkDvdRequest* req, int arg, f32 time);
extern "C" void* fn_801D4650(u32 size);

// File slot (real type unknown): the path, a second name and the DVDFileInfo.
struct UnkDvdSlot {
    /* 0x000 */ u8 mUsed;
    /* 0x001 */ char mPath[0x100];
    /* 0x101 */ char mName[0x27];
    /* 0x128 */ u8 mFileInfo[0x3C];
};

// Request (real type unknown). Type 0 opens the slot's file, 1 reads it, 3 closes it.
struct UnkDvdRequest {
    /* 0x00 */ int mType;
    /* 0x04 */ int mState;
    /* 0x08 */ int mRetries;
    /* 0x0C */ UnkDvdSlot* mSlot;
    /* 0x10 */ void* mBuffer;
    /* 0x14 */ int mLength;
    /* 0x18 */ int mOffset; // in 0x800-byte sectors
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ int mCancel;
    /* 0x24 */ u32 mSerial;
};

// Circular queue of requests (real type unknown): new requests go before mHead, the
// oldest one is at mTail; both are -1 when it is empty.
struct UnkDvdRequestQueue {
    bool IsEmpty() { return mHead < 0 || mTail < 0; }

    // The index comes by reference: by value, CW folds `i <= tail` when i is mTail
    // (Back), where the DOL keeps the compare.
    bool Contains(const int& index) {
        int tail;
        int i = index;
        if (i < 0 || i >= mCapacity) {
            return false;
        }
        tail = mTail;
        int head = mHead;
        if (tail < head) {
            if (i <= tail) {
                i += mCapacity;
            }
            tail += mCapacity;
        }
        return head <= i && i <= tail;
    }

    UnkDvdRequest* At(int i) {
        if (Contains(i)) {
            return &mData[i];
        }
        return 0;
    }

    UnkDvdRequest* Back() { return At(mTail); }

    UnkDvdRequest* PushFront() {
        if (mHead == (mTail + 1) % mCapacity) {
            return 0;
        }
        int head = mHead - 1;
        if (head < 0) {
            head = mCapacity - 1;
        }
        mHead = head;
        if (mTail < 0) {
            mTail = head;
            m0 = 0;
        }
        return At(mHead);
    }

    // Read through a reference so that mTail is reloaded after Back, as in the DOL.
    void PopBack() {
        int& tailRef = mTail;
        if (tailRef == mHead) {
            mTail = -1;
            mHead = -1;
        } else {
            int tail = tailRef - 1;
            if (tail < 0) {
                tail = mCapacity - 1;
            }
            mTail = tail;
        }
    }

    /* 0x00 */ int m0;
    /* 0x04 */ int m4;
    /* 0x08 */ int mHead;
    /* 0x0C */ int mTail;
    /* 0x10 */ int mCapacity;
    /* 0x14 */ UnkDvdRequest* mData;
};

// DVD request loader (real type unknown). A handle holds the request's serial number
// shifted by 7 and its index in mRequests.
struct UnkDvdLoader {
    ~UnkDvdLoader() {}

    UnkDvdRequest* Get(u32 handle) {
        if (handle == 0xFFFFFFFF) {
            return 0;
        }
        if ((handle >> 7) == mRequests[handle & 0x7F].mSerial) {
            return &mRequests[handle & 0x7F];
        }
        return 0;
    }

    void fn_801E47D8();
    void fn_801E4918(int arg, f32 time);
    bool fn_801E4B00();
    bool fn_801E4B2C(const u32& handle);
    UnkDvdSlot* fn_801E4B98();
    u32 fn_801E4BE4(UnkDvdSlot* slot, const char* path, const char* name);
    u32 fn_801E4E20(UnkDvdSlot* slot);
    u32 fn_801E4FE0(UnkDvdSlot* slot, void* buffer, int length, int offset);
    bool fn_801E51AC(const u32& handle);
    void fn_801E5220(const u32& handle);
    void fn_801E5478(void* callback);
    void fn_801E5480(void* callback);

    /* 0x00 */ UnkDvdSlot* mSlots;
    /* 0x04 */ UnkDvdRequest* mRequests;
    /* 0x08 */ UnkDvdRequestQueue mQueue;
    /* 0x20 */ u32 mSerial;
    /* 0x24 */ void* mCallback24;
    /* 0x28 */ void* mCallback28;
};

u32 lbl_805F9690 = 0xFFFFFFFF;
UnkDvdLoader lbl_8048A8B4;

static inline bool IsInvalid(const u32& handle) {
    return lbl_805F9690 == handle;
}

// Takes a request from the global loader's queue and returns its handle, or
// lbl_805F9690 when the queue is full.
static inline u32 NewRequest() {
    UnkDvdRequest* req = lbl_8048A8B4.mQueue.PushFront();
    u32 handle = lbl_805F9690;
    if (req != 0) {
        req->mState = 1;
        req->mCancel = 0;
        req->mRetries = 0;
        req->mSerial = lbl_8048A8B4.mSerial++;
        if (lbl_8048A8B4.mSerial >= 0x1FFFFFF) {
            lbl_8048A8B4.mSerial = 0;
        }
        handle = (req->mSerial << 7) | (req - lbl_8048A8B4.mQueue.mData);
    }
    return handle;
}

static inline bool IsDone(UnkDvdRequest* req) {
    if (req == 0) {
        return true;
    }
    return req->mState == 0;
}

void UnkDvdLoader::fn_801E47D8() {
    mRequests = (UnkDvdRequest*)fn_801D4650(0x80 * sizeof(UnkDvdRequest));
    mSlots = (UnkDvdSlot*)fn_801D4650(0x80 * sizeof(UnkDvdSlot));
    mQueue.mData = mRequests;
    mQueue.mCapacity = 0x80;
    mQueue.mTail = -1;
    mQueue.mHead = -1;
    mQueue.m4 = 0;
    for (int i = 0; i < 0x40; i++) {
        mSlots[i].mUsed = 0;
    }
    mSerial = 0;
}

void UnkDvdLoader::fn_801E4918(int arg, f32 time) {
    if (mQueue.IsEmpty()) {
        return;
    }
    UnkDvdRequest* req = mQueue.Back();
    while (req != 0 && req->mCancel != 0) {
        mQueue.PopBack();
        req = mQueue.Back();
    }
    if (req != 0 && fn_80204210(req, arg, time) == 1) {
        req->mState = 0;
        lbl_8048A8B4.mQueue.PopBack();
    }
}

bool UnkDvdLoader::fn_801E4B00() {
    return mQueue.IsEmpty() == false;
}

bool UnkDvdLoader::fn_801E4B2C(const u32& handle) {
    UnkDvdRequest* req = Get(handle);
    if (req != 0) {
        if (req->mState == 2) {
            return false;
        }
        req->mCancel = 1;
    }
    return true;
}

UnkDvdSlot* UnkDvdLoader::fn_801E4B98() {
    for (int i = 0; i < 0x40; i++) {
        if (mSlots[i].mUsed == 0) {
            mSlots[i].mUsed = 1;
            return &mSlots[i];
        }
    }
    return 0;
}

u32 UnkDvdLoader::fn_801E4BE4(UnkDvdSlot* slot, const char* path, const char* name) {
    u32 handle = NewRequest();
    UnkDvdRequest* req = Get(handle);
    if (req != 0) {
        req->mSlot = slot;
        req->mType = 0;
        strcpy(slot->mPath, path);
        for (char* p = slot->mPath; *p != '\0'; p++) {
            if (*p == '\\') {
                *p = '/';
            }
        }
        strcpy(slot->mName, name);
    }
    return handle;
}

u32 UnkDvdLoader::fn_801E4E20(UnkDvdSlot* slot) {
    u32 handle = NewRequest();
    UnkDvdRequest* req = Get(handle);
    if (req != 0) {
        req->mSlot = slot;
        req->mType = 3;
    }
    return handle;
}

u32 UnkDvdLoader::fn_801E4FE0(UnkDvdSlot* slot, void* buffer, int length, int offset) {
    u32 handle = NewRequest();
    UnkDvdRequest* req = Get(handle);
    if (req != 0) {
        req->mSlot = slot;
        req->mType = 1;
        req->mBuffer = buffer;
        req->mLength = length;
        req->mOffset = offset;
    }
    return handle;
}

// 99.7 %: the DOL compares `cmplw handle, invalid` after loading the invalid handle
// first; this build loads it first too but compares `cmplw invalid, handle` (the
// other operand order loads the handle first).
bool UnkDvdLoader::fn_801E51AC(const u32& handle) {
    if (IsInvalid(handle)) {
        return false;
    }
    UnkDvdRequest* req = Get(handle);
    if (req != 0) {
        return req->mState == 0;
    }
    return true;
}

void UnkDvdLoader::fn_801E5220(const u32& handle) {
    UnkDvdRequest* req = Get(handle);
    while (!IsDone(req)) {
        fn_801E4918(1, 0.01f);
    }
}

void UnkDvdLoader::fn_801E5478(void* callback) {
    mCallback24 = callback;
}

void UnkDvdLoader::fn_801E5480(void* callback) {
    mCallback28 = callback;
}
