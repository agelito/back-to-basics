// game_window.c

#include "game_window.h"

#include <SDL2/SDL.h>
typedef struct GameWindowInternal {
    SDL_Window* window_handle;
    SDL_Surface* surface;
} GameWindowInternal;

void game_window_update_size(GameWindow* window)
{
    SDL_GetWindowSize(window->internal->window_handle,
                      &window->window_width,
                      &window->window_height);

    SDL_Renderer* renderer = 
        SDL_GetRenderer(window->internal->window_handle);

    SDL_GetRendererOutputSize(renderer,
                              &window->pixel_buffer_width,
                              &window->pixel_buffer_height);
}

GameWindow* game_window_create(const char* title, int width, int height)
{
    uint32_t window_flags = SDL_WINDOW_RESIZABLE;
    SDL_Window *sdl_window = SDL_CreateWindow(title,
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          width, height,
                                          window_flags);

    if (!sdl_window)
    {
        printf("failed to create window\n");
        return 0;
    }

    uintptr_t window_and_internal_size = 
        sizeof(GameWindow) + sizeof(GameWindowInternal);
    GameWindow* game_window = malloc(window_and_internal_size);
    game_window->internal = (GameWindowInternal*)(game_window + 1);
    game_window->internal->window_handle = sdl_window;
    game_window->internal->surface = 0;

    game_window->window_width = width;
    game_window->window_height = height;
    game_window->pixel_buffer_width = 0;
    game_window->pixel_buffer_height = 0;
    game_window->pixels = 0;
    game_window->flags = GAME_WINDOW_FLAGS_NONE;

    return game_window;
}

void game_window_destroy(GameWindow* game_window)
{
    SDL_DestroyWindow(game_window->internal->window_handle);
    game_window->internal->window_handle = 0;

    free(game_window);
}

void game_window_process_events(GameWindow* game_window)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SHOWN) {
                    game_window_update_size(game_window);
                }
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    game_window_update_size(game_window);
                }
                break;
            case SDL_QUIT:
                game_window->flags |= GAME_WINDOW_FLAGS_CLOSED;
                break;
        }
    }
}

void game_window_surface_lock_pixels(GameWindow* game_window)
{
    SDL_Surface *surface = 
        SDL_GetWindowSurface(game_window->internal->window_handle);
    
    SDL_LockSurface(surface);

    game_window->internal->surface = surface;
    game_window->pixels = surface->pixels;
}

void game_window_surface_unlock_and_update_pixels(GameWindow* game_window)
{
    game_window->pixels = 0;

    SDL_UnlockSurface(game_window->internal->surface);
    SDL_UpdateWindowSurface(game_window->internal->window_handle);
}
