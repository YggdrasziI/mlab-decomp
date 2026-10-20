// Game unit at 0x800C4530-0x800C52B4 (real file name unknown): the ExWorldCurvedTile
// class (RTTI; its RTTI record is emitted by an earlier unit, as for the track
// classes). The tile's path is an arc in the plane of its two bones' directions:
// vfBC finds its centre and angle, vfD0 gives its length, vfD4 and vfD8 the position
// and rotation at a distance along it. Bounds come from the static initializers of
// this unit (0x800C5294) and the previous one.

#include "game/unk_vec3.mch"
#include "game/Entity.h"

#include <math.h>

extern "C" {
// Rotation of a 3x3 matrix as a quaternion (real name unknown).
void fn_8014474C(const f32* mtx, UnkQuat* out);
// Rotation of angle (in quarter turns) around axis (real name unknown).
UnkQuat fn_80145EE8(const UnkVec3* axis, f32 angle);
}

// UnkSqrtf returning a float (rounded once).
static inline f32 UnkSqrtfF(f32 x) {
    f32 result;
    if (x <= 0.0f) {
        result = 0.0f;
    } else {
        double guess = __frsqrte(x);
        guess = 0.5 * (guess * (3.0 - x * guess * guess));
        guess = 0.5 * (guess * (3.0 - x * guess * guess));
        result = guess * x;
    }
    return result;
}

static inline UnkVec3 UnkCross(const UnkVec3& a, const UnkVec3& b) {
    UnkVec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

static inline bool UnkIsNearZero(f32 x) {
    return x <= 0.0001f && x >= -0.0001f;
}

static inline bool UnkIsNearZero(const UnkVec3& v) {
    return v.x <= 0.0001f && v.x >= -0.0001f && v.y <= 0.0001f && v.y >= -0.0001f &&
           v.z <= 0.0001f && v.z >= -0.0001f;
}

// Clamp whose result is returned as a double (see UnkClampD in unit_8009F66C).
static inline double UnkClampD(f32 value, f32 low, f32 high) {
    f32 result;
    if (value < low) {
        result = low;
    } else if (value > high) {
        result = high;
    } else {
        result = value;
    }
    return result;
}

// Angle between a and b in quarter turns.
static inline f32 UnkAngleFraction(const UnkVec3& a, const UnkVec3& b) {
    f32 dot = a.x * b.x + a.y * b.y + a.z * b.z;
    f32 lengths = (f32)UnkSqrtf(a.LengthSq()) * (f32)UnkSqrtf(b.LengthSq());
    bool zero = lengths <= 0.0f;
    if (zero) {
        return 0.0f;
    }
    return 0.63661975f * acosf(UnkClamp(UnkClampD(dot / lengths, -1.0f, 1.0f), -1.0f, 1.0f));
}

static inline UnkQuat UnkQuatMul(const UnkQuat& a, const UnkQuat& b) {
    UnkQuat result;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    return result;
}

static inline UnkQuat UnkQuatConj(const UnkQuat& q) {
    UnkQuat result;
    result.x = -q.x;
    result.y = -q.y;
    result.z = -q.z;
    result.w = q.w;
    return result;
}

// Ratio of a to b along the first axis where b is not near zero (0 if none).
static inline double UnkRatio(const UnkVec3& a, const UnkVec3& b) {
    f32 t = 0.0f;
    if (!UnkIsNearZero(b.x)) {
        t = a.x / b.x;
    } else if (!UnkIsNearZero(b.y)) {
        t = a.y / b.y;
    } else if (!UnkIsNearZero(b.z)) {
        t = a.z / b.z;
    }
    return t;
}

static inline UnkVec3 UnkLocalToWorld(EntityInfo* info, const UnkVec3& v) {
    UnkQuat conj = UnkQuatConj(info->mRot);
    UnkQuat point;
    point.x = v.x;
    point.y = v.y;
    point.z = v.z;
    point.w = 0.0f;
    UnkQuat rotated = UnkQuatMul(UnkQuatMul(info->mRot, point), conj);
    UnkVec3 world(rotated.x, rotated.y, rotated.z);
    return world + info->mPos;
}

ExWorldCurvedTile::ExWorldCurvedTile(int type, EntityInfo* info, int init)
    : ExWorldTile(type, info, init) {}

ExWorldCurvedTile::~ExWorldCurvedTile() {}

// NonMatching (95.8 %): register allocation in the angle and centre computations,
// and the DOL loads each divisor before its dividend.
void ExWorldCurvedTile::vfBC() {
    ExWorldTile::vfBC();

    UnkVec3 posA = UnkVec3Zero();
    UnkVec3 dirA(0.0f, 1.0f, 0.0f);
    if (mStartBone >= 0) {
        UnkBone* start = &m94.mBones[mStartBone];
        posA = start->m2C;
        dirA = start->m14;
    }
    UnkBone* end = &m94.mBones[mEndBone];
    UnkVec3 posB = end->m2C;
    UnkVec3 dirB = end->m14;
    UnkVec3 axis = UnkCross(dirA, dirB);

    mCenter = UnkVec3Zero();
    mRadius = -1.0f;
    mAngle = 0.0f;
    if (UnkIsNearZero(axis)) {
        return;
    }

    UnkVec3 perpA = UnkCross(dirA, axis);
    UnkVec3 perpB = UnkCross(dirB, axis);
    UnkVec3 diff = posA - posB;
    UnkVec3 delta = perpB - perpA;
    mCenter = posA + perpA * UnkRatio(diff, delta);
    mAngle = 1.5707964f * UnkAngleFraction(dirA, dirB);
    posA.x -= mCenter.x;
    posA.y -= mCenter.y;
    posA.z -= mCenter.z;
    posB.x -= mCenter.x;
    posB.y -= mCenter.y;
    posB.z -= mCenter.z;
}

f32 ExWorldCurvedTile::vfD0() const {
    if (mRadius < 0.0f) {
        UnkVec3 posA = UnkVec3Zero();
        if (mStartBone >= 0) {
            posA = m94.mBones[mStartBone].m2C;
        }
        UnkVec3 posB = m94.mBones[mEndBone].m2C;
        UnkVec3 delta = posB - posA;
        return UnkSqrtfF(delta.LengthSq());
    }
    return mRadius * mAngle;
}

// NonMatching (62 %): the DOL keeps the scaled vectors and the quaternion products
// in stack temporaries (0x150-byte frame, f22-f31 saved); this build keeps most of
// them in registers.
UnkVec3 ExWorldCurvedTile::vfD4(f32 t) {
    f32 u = t / vfD0();
    double a = 1.0f - u;
    double b = u;
    if (mAngle > 0.0f) {
        f32 angle = 0.63661975f * mAngle;
        f32 inv = 1.0f / sinf(1.5707964f * angle);
        a = inv * sinf(1.5707964f * ((1.0f - u) * angle));
        b = inv * sinf(1.5707964f * (u * angle));
    }

    UnkVec3 posA = UnkVec3Zero();
    if (mStartBone >= 0) {
        posA = m94.mBones[mStartBone].m2C;
    }
    UnkVec3 posB = m94.mBones[mEndBone].m2C;
    posA = posA - mCenter;
    posB = posB - mCenter;
    UnkVec3 result = posA * a + posB * b + mCenter;

    result = UnkLocalToWorld(mInfo, result);
    return result;
}

// NonMatching (78 %): the DOL also copies three words of the start bone's matrix
// (0x44-0x4C, without the index check) to scattered stack slots that are never
// read, and its frame is 0x30 bytes larger.
UnkQuat ExWorldCurvedTile::vfD8(f32 t) {
    f32 u = t / vfD0();
    UnkQuat rotA;
    rotA.x = rotA.y = rotA.z = 0.0f;
    rotA.w = 1.0f;
    if (mStartBone >= 0) {
        fn_8014474C(m94.mBones[mStartBone].m38, &rotA);
    }
    UnkQuat rotB;
    fn_8014474C(m94.mBones[mEndBone].m38, &rotB);
    UnkQuat result = rotA.fn_80145F9C(&rotB, u);
    UnkVec3 up(0.0f, 1.0f, 0.0f);
    UnkQuat roll = fn_80145EE8(&up, 0.63661975f * mRoll);
    result = UnkQuatMul(result, UnkQuatConj(roll));
    return result;
}
