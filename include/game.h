#ifndef GAME_H
#define GAME_H

#include "sdk.h"
#include "version.h"

#define PLAYER_MAX_HEALTH 100
#define PLAYER_GRAVITY 1
#define PLAYER_JUMP_SPEED 12
#define WORLD_WIDTH 640

#define BUTTON_A 0x0001

typedef struct Player {
    s32 x;
    s32 y;
    s32 vx;
    s32 vy;
    s32 health;
    s32 on_ground;
} Player;

typedef struct GameState {
    Player player;
    u32 score;
    u32 seconds;
} GameState;

extern GameState g_game;

/* game/main */
void game_init(void);
void game_frame(u32 frame);

/* game/player */
void player_init(Player *player);
void player_update(Player *player);
void player_jump(Player *player);
void player_take_damage(Player *player, s32 amount);

/* game/save */
u32 save_checksum(const u8 *data, u32 size);
void save_write(const GameState *game);
s32 save_read(GameState *game);
#if VERSION == VERSION_EU
extern u32 save_language;
const char *save_get_language_name(void);
#endif

#endif
