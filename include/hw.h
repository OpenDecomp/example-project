#ifndef HW_H
#define HW_H

#include "sdk.h"

/* Hardware registers of the made-up console. */
#define HW_REG(addr) (*(volatile u32 *)(addr))

#define HW_TIMER_CTRL HW_REG(0xC0001000)
#define HW_TIMER_COUNT HW_REG(0xC0001004)
#define HW_VIDEO_STATUS HW_REG(0xC0002000)
#define HW_PAD_BUTTONS HW_REG(0xC0003000)
#define HW_CARD_DATA HW_REG(0xC0004000)
#define HW_CARD_CTRL HW_REG(0xC0004004)

#endif
