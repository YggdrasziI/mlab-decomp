// Game unit at 0x8010B16C-0x8010B4AC (real file name unknown): UnkPath, a polyline
// built from the points of a wire (real name unknown). Bounds come from the static
// initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"
#include "game/unk_math.h"
#include "game/unk_vec3.h"

extern "C" {
void* fn_801D4650(u32 size);
void fn_801D4980(void* ptr);
}

UnkPath::UnkPath() {
    Clear();
}

UnkPath::~UnkPath() {
    if (mSegments != 0) {
        fn_801D4980(mSegments);
        mSegments = 0;
    }
    Clear();
}

// NonMatching (77 %): the DOL reloads mCount after the loop instead of keeping it in
// a register, and the loop index takes another register.
UnkVec3 UnkPath::fn_8010B200(f32 t) {
    f32 dist = t * mLength;
    u32 i;
    for (i = 0; i < mCount; i++) {
        if (dist < mSegments[i].mLength) {
            break;
        }
        dist -= mSegments[i].mLength;
    }
    if (i == mCount) {
        i--;
        dist = mSegments[i].mLength;
    }
    UnkPathSegment* segment = &mSegments[i];
    UnkVec3 result = segment->mStart;
    result += segment->mDir * (dist / segment->mLength);
    return result;
}

// NonMatching (98.3 %): register and stack slot assignment of the loop.
void UnkPath::fn_8010B2D8(UnkWire* wire) {
    mCount = wire->mPointCount - 1;
    mSegments = (UnkPathSegment*)fn_801D4650(mCount * sizeof(UnkPathSegment));
    mLength = 0.0f;
    for (int i = 0; i < wire->mPointCount - 1; i++) {
        mSegments[i].mStart = UnkVec3(wire->mPoints[i].mPos);
        UnkVec3 start = wire->mPoints[i].mPos;
        UnkVec3 end = (wire->mPoints + i)[1].mPos;
        UnkVec3 dir = end - start;
        mSegments[i].mDir = dir;
        mSegments[i].mLength = dir.Length();
        mLength += mSegments[i].mLength;
    }
}
