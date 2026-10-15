#ifndef GAME_UNK_MATH_H
#define GAME_UNK_MATH_H

// Inline math helpers seen in game code (real names unknown).

#include "types.h"
#include <math.h>

// Square root with two Newton steps on frsqrte; 0 for x <= 0. Differs from MSL's
// sqrtf (three steps, returns x for x <= 0).
// The result is a float widened to double: callers round it again.
inline double UnkSqrtf(f32 x) {
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

#endif // GAME_UNK_MATH_H
