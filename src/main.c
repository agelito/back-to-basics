// main.c

#include <SDL2/SDL.h>

#include "game_window.h"

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("failed initializing SDL2\n");
        return -1;
    }

    GameWindow* game_window = 
        game_window_create("back_to_basics", 680, 480);

    while ((game_window->flags & GAME_WINDOW_FLAGS_CLOSED) == 0)
    {
        game_window_process_events(game_window);

        game_window_surface_lock_pixels(game_window);

        uint32_t *pixels_32bpp = (uint32_t *)game_window->pixels;
        for (int y = 0; y < game_window->pixel_buffer_height; y++)
        {
            uint32_t row = y * game_window->pixel_buffer_width;
            for (int x = 0; x < game_window->pixel_buffer_width; x++)
            {
                pixels_32bpp[x + row] = 0xff0000ff;
            }
        }

        game_window_surface_unlock_and_update_pixels(game_window);

        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}