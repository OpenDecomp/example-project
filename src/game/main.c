#include "game.h"

GameState g_game;

void game_init(void) {
    os_init();
    math_srand(os_get_tick());
    player_init(&g_game.player);
    if (save_read(&g_game) != 0) {
        g_game.score = 0;
        g_game.seconds = 0;
    }
}

void game_frame(u32 frame) {
    Player *player = &g_game.player;

    if (os_get_buttons() & BUTTON_A) {
        player_jump(player);
    }
    player_update(player);
    if ((math_rand() & 0xFF) == 0) {
        player_take_damage(player, 10);
    }
    if (frame % FRAME_RATE == 0) {
        g_game.seconds++;
        g_game.score += 10;
    }
}

int main(void) {
    u32 frame = 0;

    game_init();
    while (g_game.player.health > 0) {
        os_wait_vblank();
        game_frame(frame);
        frame++;
    }
    save_write(&g_game);
    return 0;
}
