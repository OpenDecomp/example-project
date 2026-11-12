#include "sdk.h"

static u32 s_seed = 0x2545F491;

s32 math_abs(s32 x) {
    return x < 0 ? -x : x;
}

s32 math_clamp(s32 x, s32 lo, s32 hi) {
    if (x < lo) {
        return lo;
    }
    if (x > hi) {
        return hi;
    }
    return x;
}

/* Nonmatching: same result, but the loop compiles differently. */
u32 math_isqrt(u32 x) {
    u32 result = 0;
    u32 bit = 1u << 30;

    while (bit > x) {
        bit >>= 2;
    }
    for (; bit != 0; bit >>= 2) {
        u32 trial = result + bit;

        result >>= 1;
        if (x >= trial) {
            x -= trial;
            result += bit;
        }
    }
    return result;
}

void math_srand(u32 seed) {
    s_seed = seed != 0 ? seed : 1;
}

u32 math_rand(void) {
    u32 x = s_seed;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    s_seed = x;
    return x;
}
