#ifndef SDK_H
#define SDK_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/* sdk/os */
void os_init(void);
u32 os_get_tick(void);
u32 os_get_buttons(void);
void os_wait_vblank(void);
u32 os_get_frame_count(void);

/* sdk/math */
s32 math_abs(s32 x);
s32 math_clamp(s32 x, s32 lo, s32 hi);
u32 math_isqrt(u32 x);
void math_srand(u32 seed);
u32 math_rand(void);

#endif
