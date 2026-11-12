#include "game.h"

static const Player s_player_start = {
    32,                /* x */
    0,                 /* y */
    2,                 /* vx */
    0,                 /* vy */
    PLAYER_MAX_HEALTH, /* health */
    1,                 /* on_ground */
};

void player_init(Player *player) {
    *player = s_player_start;
}

void player_update(Player *player) {
    player->x += player->vx;
    player->y += player->vy;
    if (player->y <= 0) {
        player->y = 0;
        player->vy = 0;
        player->on_ground = 1;
    } else {
        player->vy -= PLAYER_GRAVITY;
    }
    player->x = math_clamp(player->x, 0, WORLD_WIDTH);
}

void player_jump(Player *player) {
    if (player->on_ground) {
        player->vy = PLAYER_JUMP_SPEED;
        player->on_ground = 0;
    }
}

/* Matches us only: eu does something else here. */
void player_take_damage(Player *player, s32 amount) {
    player->health -= amount;
}
