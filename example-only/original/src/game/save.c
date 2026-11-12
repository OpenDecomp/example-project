#include "game.h"
#include "hw.h"

#define SAVE_MAGIC 0x53415645 /* "SAVE" */

typedef struct SaveFile {
    u32 magic;
    u32 score;
    u32 seconds;
#if VERSION == VERSION_EU
    u32 language;
#endif
    u32 checksum;
} SaveFile;

#define SAVE_WORDS (sizeof(SaveFile) / sizeof(u32))
#define SAVE_CHECKED_BYTES (sizeof(SaveFile) - sizeof(u32))

static SaveFile s_save;

#if VERSION == VERSION_EU
static const char *const s_language_names[] = {"English", "Deutsch", "Francais"};

u32 save_language;

const char *save_get_language_name(void) {
    return s_language_names[save_language % 3];
}
#endif

u32 save_checksum(const u8 *data, u32 size) {
    u32 sum = 0;
    u32 i;

    for (i = 0; i < size; i++) {
        sum = ((sum << 1) | (sum >> 31)) + data[i];
    }
    return sum;
}

void save_write(const GameState *game) {
    const u32 *words = (const u32 *)&s_save;
    u32 i;

    s_save.magic = SAVE_MAGIC;
    s_save.score = game->score;
    s_save.seconds = game->seconds;
#if VERSION == VERSION_EU
    s_save.language = save_language;
#endif
    s_save.checksum = save_checksum((const u8 *)&s_save, SAVE_CHECKED_BYTES);
    for (i = 0; i < SAVE_WORDS; i++) {
        HW_CARD_DATA = words[i];
    }
    HW_CARD_CTRL = 1;
}

s32 save_read(GameState *game) {
    u32 *words = (u32 *)&s_save;
    u32 i;

    HW_CARD_CTRL = 2;
    for (i = 0; i < SAVE_WORDS; i++) {
        words[i] = HW_CARD_DATA;
    }
    if (s_save.magic != SAVE_MAGIC) {
        return -1;
    }
    if (s_save.checksum != save_checksum((const u8 *)&s_save, SAVE_CHECKED_BYTES)) {
        return -2;
    }
    game->score = s_save.score;
    game->seconds = s_save.seconds;
#if VERSION == VERSION_EU
    save_language = s_save.language;
#endif
    return 0;
}
