#include "sdk.h"
#include "hw.h"

static u32 s_frame_count;

void os_init(void) {
    s_frame_count = 0;
    HW_TIMER_CTRL = 1;
}

u32 os_get_tick(void) {
    return HW_TIMER_COUNT;
}

u32 os_get_buttons(void) {
    return HW_PAD_BUTTONS & 0xFFFF;
}

void os_wait_vblank(void) {
    while ((HW_VIDEO_STATUS & 1) == 0) {
    }
    s_frame_count++;
}

u32 os_get_frame_count(void) {
    return s_frame_count;
}
