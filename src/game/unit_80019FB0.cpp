// Game unit at 0x80019FB0-0x8001A17C (real file name unknown). Bounds come from the
// static initializers of this unit and the previous one.

#include "game/unk_vec3.mch"
#include "game/unk_vec3.h"

inline f32 UnkSqrtfF(f32 x) {
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

// Launch speed that makes a projectile fired from `from` at `angle` (scaled by pi/2)
// under `gravity` reach `to`; 0 when no speed does. Real name unknown.
// NonMatching (89.8 %): `height` and `s` get each other's registers (f27 / f28), the
// y and z stores of `horizontal` are swapped, and the from.z / to.z loads use other
// temporaries.
extern "C" f32 fn_80019FB0(const UnkVec3& from, const UnkVec3& to, f32 angle, f32 gravity) {
    f32 dx = to.x - from.x;
    f32 dy = to.y - from.y;
    UnkVec3 horizontal(dx, dy, 0.0f);
    f32 distance = UnkSqrtfF(horizontal.LengthSq());
    f32 rad = angle * 1.5707964f;
    f32 height = to.z - from.z;
    f32 s = sin(rad);
    f32 c = cos(angle * 1.5707964f);
    f32 speedSq = (distance * (-gravity * distance) / (2.0f * c * c)) / (height - s * distance / c);
    if (speedSq < 0.0f) {
        return 0.0f;
    }
    return UnkSqrtfF(speedSq);
}
