// main.c

#include <SDL2/SDL.h>

#include "game_window.h"
#include "renderer.h"

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

        int32_t bytes_per_pixel = 4;
        renderer_target_buffer pixel_buffer = 
            renderer_create_target_buffer(game_window->pixel_buffer_width, game_window->pixel_buffer_height, bytes_per_pixel, game_window->pixels);

        renderer_fill(pixel_buffer, PackColorRGB(0, 0, 255));

        if (game_window->pixel_buffer_width != 0)
        {
            renderer_rect top_left = {
                0, 0, 32, 32
            };

            renderer_rect top_right = {
                pixel_buffer.width - 32,
                0, 32, 32
            };

            renderer_rect bottom_left = {
                0, 
                pixel_buffer.height - 32,
                32, 32
            };

            renderer_rect bottom_right = {
                pixel_buffer.width - 32,
                pixel_buffer.height - 32,
                32, 32
            };

            renderer_fill_rect(pixel_buffer, top_left, PackColorRGB(255, 0, 0));
            renderer_fill_rect(pixel_buffer, top_right, PackColorRGB(0, 255, 0));
            renderer_fill_rect(pixel_buffer, bottom_left, PackColorRGB(0, 255, 255));
            renderer_fill_rect(pixel_buffer, bottom_right, PackColorRGB(255, 255, 0));
        }

        game_window_surface_unlock_and_update_pixels(game_window);

        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}