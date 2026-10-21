#ifndef GAME_PATHAPI_H
#define GAME_PATHAPI_H

// Exploration::PathAPI (class name from RTTI, game unit at 0x800D3428) and the
// records it keeps: path points (Unk800D4C64, game unit at 0x800D4C64), paths between
// them (Unk800D5008) and the links of each point. Real names of everything but
// PathAPI are unknown; the containers are probably one template each (bounds-checked
// accessors inlined everywhere).

#include "game/unk_vec3.h"

class Unk800D5008;

// A conditional expression: an if/return version gives other registers.
template <typename T>
inline T UnkClampT(T value, T low, T high) {
    return value < low ? low : (value > high ? high : value);
}

// Path point, 0x1AC bytes.
class Unk800D4C64 {
public:
    Unk800D4C64();
    Unk800D4C64& operator=(const Unk800D4C64& other);

    /* 0x000 */ u32 m0; // id
    // Four exits, set and copied exit by exit in the DOL (the order of the stores).
    /* 0x004 */ Unk800D5008* mPaths[4];
    /* 0x014 */ u32 mPathIds[4];
    /* 0x024 */ u32 mLocks[4]; // an exit is closed when set (unless asked to ignore it)
    /* 0x034 */ UnkVec3 m34; // position
    /* 0x040 */ int m40;
    /* 0x044 */ u32 m44[3];
    /* 0x050 */ int m50; // hazard handle (UnkHazardManager)
    /* 0x054 */ u32 m54;
    /* 0x058 */ u32 m58;
    /* 0x05C */ u32 m5C;
    /* 0x060 */ int m60;
    /* 0x064 */ u8 m64;
    /* 0x068 */ int m68[10]; // link keys
    /* 0x090 */ int m90; // number of link keys
    /* 0x094 */ u32 m94;
    /* 0x098 */ u32 m98;
    /* 0x09C */ u8 m9C;
    /* 0x0A0 */ u32 mA0;
    // A flat word array: the 64-word loops unroll into the DOL's 2 iterations of
    // 32 stores (constructor) and 4 iterations of 16 copies (operator=).
    /* 0x0A4 */ u32 mA4[64];
    /* 0x1A4 */ u32 m1A4;
    /* 0x1A8 */ u32 m1A8;
};

// Point with a flag, 0x10 bytes.
struct Unk800D5008Point {
    /* 0x0 */ UnkVec3 mPos;
    /* 0xC */ u8 mFlag;
};

// Path between two points, 0xDC bytes.
class Unk800D5008 {
public:
    Unk800D5008();
    ~Unk800D5008();
    Unk800D5008& operator=(const Unk800D5008& other);

    void Add(const Unk800D5008Point& point) {
        if (mCount < 12) {
            Unk800D5008Point& dst = mPoints[mCount];
            dst.mPos.x = point.mPos.x;
            dst.mPos.y = point.mPos.y;
            dst.mPos.z = point.mPos.z;
            dst.mFlag = point.mFlag;
            mCount++;
        }
    }

    Unk800D5008Point& Point(int i) {
        Unk800D5008Point* point = mPoints;
        if (i != UnkClampT(i, 0, mCount)) {
            return *point;
        }
        return point[i];
    }

    /* 0x00 */ u32 m0; // id
    /* 0x04 */ Unk800D4C64* m4[2]; // end points
    /* 0x0C */ u32 mC[2]; // their ids
    /* 0x14 */ u32 m14;
    /* 0x18 */ Unk800D5008Point mPoints[12];
    /* 0xD8 */ int mCount;
};

// Cursor of a path follower, 0x24 bytes: either at a point (mPoint) or moving
// along a path (mPath, towards the point of index mIndex; -4 and -3 stand for the
// two end points).
struct Unk800D50C4Ctx {
    /* 0x00 */ f32 mSpeed;
    /* 0x04 */ bool mForward; // along the path towards end point 1
    /* 0x08 */ int mState; // 0 moving, 1 paused, 2 stopped
    /* 0x0C */ int mIndex;
    /* 0x10 */ Unk800D5008* mPath;
    /* 0x14 */ Unk800D4C64* mPoint;
    /* 0x18 */ UnkVec3 mPos;
};

// Path follower (game unit at 0x800D50C4), 0x404 bytes: its cursor and the points
// visited by the last path search.
class Unk800D50C4 {
public:
    Unk800D50C4();
    ~Unk800D50C4();
    void fn_800D5110(Unk800D50C4Ctx* ctx, Unk800D4C64* point);
    void fn_800D5174(f32 dt);

    /* 0x000 */ Unk800D50C4Ctx* m0;
    /* 0x004 */ Unk800D4C64* mVisited[255];
    /* 0x400 */ int mVisitedCount;
};

// Pool entry of PathAPI, 0x428 bytes.
struct Unk800D4BB4 {
    Unk800D4BB4();
    ~Unk800D4BB4();

    /* 0x000 */ Unk800D50C4Ctx mCtx;
    /* 0x024 */ Unk800D50C4 mObj;
};

// Point links: the ids of the points sharing a key, 0x88 bytes.
struct UnkPathLink {
    /* 0x00 */ int mKey;
    /* 0x04 */ u32 mValues[32];
    /* 0x84 */ int mCount;
};

// Array with a count; out of range indexes give the first element.
template <typename T, int N>
class UnkCountedArray {
public:
    UnkCountedArray() { mCount = 0; }

    T& operator[](int i) {
        if (i != UnkClampT(i, 0, mCount)) {
            return mData[0];
        }
        return mData[i];
    }
    int Count() const { return mCount; }
    void Clear() { mCount = 0; }
    void Add(const T& item) {
        if (mCount < N) {
            mData[mCount] = item;
            mCount++;
        }
    }
    // Removes an element by moving the last one in its place.
    void RemoveAt(int i) {
        if (i < mCount) {
            mCount--;
            if (i != mCount) {
                mData[i] = mData[mCount];
            }
        }
    }

    T mData[N];
    int mCount;
};

// Fixed pool with a used flag per entry.
template <typename T, int N>
class UnkPool {
public:
    UnkPool() {
        for (int i = 0; i < N; i++) {
            mUsed[i] = false;
        }
    }

    T& operator[](int i) {
        if (i != UnkClampT(i, 0, N)) {
            return mData[0];
        }
        // `== false`, not `!`, for the DOL's branch order.
        if (mUsed[i] == false) {
            return mData[0];
        }
        return mData[i];
    }
    bool Next(int& i) {
        if (i != UnkClampT(i, -1, N)) {
            return false;
        }
        for (int j = i + 1; j < N; j++) {
            if (mUsed[j]) {
                i = j;
                return true;
            }
        }
        return false;
    }
    T* Alloc() {
        for (int i = 0; i < N; i++) {
            if (!mUsed[i]) {
                mUsed[i] = true;
                return &mData[i];
            }
        }
        return 0;
    }
    void Free(int i) { mUsed[i] = false; }

    T mData[N];
    bool mUsed[N];
};

namespace Exploration {

class PathAPI {
public:
    // The data members come before the first virtual function so that the vtable
    // pointer goes last, as in the DOL.
    /* 0x00000 */ Unk800D50C4 mObj;
    /* 0x00404 */ Unk800D50C4Ctx mCtx;
    /* 0x00428 */ UnkPool<Unk800D4BB4, 20> mPool;
    /* 0x0575C */ UnkCountedArray<Unk800D5008, 255> mPaths;
    /* 0x13284 */ UnkCountedArray<Unk800D4C64, 255> mPoints;
    /* 0x2DCDC */ UnkCountedArray<UnkPathLink, 255> mLinks;

    PathAPI() {}
    virtual ~PathAPI();

    static void fn_800D3428();
    static void fn_800D36A0();
    void fn_800D3818(f32 dt);
    int fn_800D3930();
    Unk800D50C4* fn_800D393C(int i);
    Unk800D50C4* fn_800D3AC4(const UnkVec3& pos);
    void fn_800D3C60(Unk800D50C4* obj);
    void fn_800D42F4(int key, u32 value);
    u32 fn_800D4588(const UnkVec3& pos);
    void fn_800D46B0();
    u32 fn_800D4730(int key);

    /* 0x36458 */ // vtable
};

} // namespace Exploration

// The PathAPI instance (fn_800D3428 creates it).
extern Exploration::PathAPI* lbl_805FB180;

#endif // GAME_PATHAPI_H
