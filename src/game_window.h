// game_window.h

#ifndef GAME_WINDOW_INCLUDED
#define GAME_WINDOW_INCLUDED

#include <stdint.h>

typedef struct GameWindow {
    int32_t window_width;
    int32_t window_height;
    int32_t pixel_buffer_width;
    int32_t pixel_buffer_height;
    uint8_t flags;
    uint8_t *pixels;

    struct GameWindowInternal* internal;
} GameWindow;

enum GameWindowFlags {
    GAME_WINDOW_FLAGS_NONE = 0,
    GAME_WINDOW_FLAGS_CLOSED = 1 << 0,
};

GameWindow* game_window_create();
void game_window_destroy(GameWindow *game_window);
void game_window_process_events(GameWindow *game_window);
void game_window_surface_lock_pixels(GameWindow *game_window);
void game_window_surface_unlock_and_update_pixels(GameWindow *game_window);

#endif // GAME_WINDOW_INCLUDED