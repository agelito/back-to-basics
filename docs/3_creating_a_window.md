# Creating a Window

Now it's time to move away from just plain terminal hello world and create a window. To display a window we'll be using the SDL2 library. 

>Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is used by video playback software, emulators, and popular games including Valve's award winning catalog and many Humble Bundle games. - https://github.com/libsdl-org/SDL

## Installing SDL2
Installing `SDL2` is easy on most Linux distributions. most package repositories already contains the library.
```sh
pacman -S sdl2
```

## Replacing Hello World
We'll start by removing the `src/chello_world.c` example code and instead create `src/main.c` this will be our entrypoint from now on, where the code starts executing when the user run our game.

```sh
rm src/hello_world.c
```

Create the new `src/main.c` file with empty main function:
```c
int main(int argc, char* argv[]) {
    return 0;
}
```

We'll also have to update our `CMakeLists.txt` to include SDL2 library and use our new `src/main.c`.

First we'll remove the old `hello_world` executable. Just remove the line
```cmake
add_executable(hello_world src/hello_world.c)
```

Next to tell cmake to find the SDL2 header files and library files add these lines to the `CMakeLists.txt`:
```cmake
find_package(SDL2 REQUIRED)
include_directories(back_to_basics ${SDL2_INCLUDE_DIRS})
```

Finally add the game executable and associated source file:
```cmake
add_executable(back_to_basics src/main.c)
target_link_libraries(back_to_basics ${SDL2_LIBRARIES})
```

## Initializing SDL2 Video Subsystem
SDL2 functionality is divided into various subsystems. The only one we're interested in for now is the VIDEO subsystem. Before using any features from the VIDEO subsystem we have to initialize it.

Add the following lines to `src/main.c`:
```c
if (SDL_Init(SDL_INIT_VIDEO) < 0)
{
    printf("failed initializing SDL2\n");
    return -1;
}
```

We'll also ensure any resources allocated by SDL2 during initialization is properly freed before the application exits. Add the following at the end of `main` function before the `return 0` statement.

```c
SDL_Quit();
```

Now our `src/main.c` file should look like this:
```c
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("failed initializing SDL2\n");
        return -1;
    }

    SDL_Quit();

    return 0;
}
```

Try compiling and running the application making sure there's no mistakes and the `failed initializing SDL2` message doesn't appear.

## Creating the window
Now finally it's time to create the window and make it stay open until the user choose to close it. Additionally we're going to ensure the window can be filled with a color.

Start by creating a source file and associated header file to ensue the `src/main.c` file doesn't get too busy. Separating code into different files helps with maintenance and organization.

Create `src/game_window.h` and `src/game_window.c`:
```sh
touch src/game_window.h
touch src/game_window.c
```

### game_window.h
The initial code inside `src/game_window.h` will look like this:
```c
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
```

Here we've defined the public `struct` for keeping track of the window state. There's a there's also a declaration of a `struct` for holding `internal` state to keep the `SDL2` references contained to `src/game_window.c` file. That way we can choose to replace `SDL2` in the future without having to modify any other parts of the code.

Additionally there's a field and associated `enum` for holding `flags` this state is packed into an eight bit integer which will allow storing eight different states. The `enum` with state will just have the `GAME_WINDOW_FLAGS_CLOSED` state for now which will be used to indicate the window has been closed by the user.

Finally there's five declared functions, these functions will be called from the `src/main.c` file to interact with the window.

### game_window.c
Start by including the necessary header files and defining the `GameWindowInternal` `struct`. Add the following to the top of the `src/game_window.c` file:

```c
// game_window.c

#include "game_window.h"

#include <SDL2/SDL.h>
typedef struct GameWindowInternal {
    SDL_Window* window_handle;
    SDL_Surface* surface;
} GameWindowInternal;
```

Next for creating our window we'll define the `game_window_create` function. This function will be responsible for allocating the memory for our `GameWindow` `struct` and then using `SDL2` to create our window. It will also populate the `struct` with some good initial values.

```c
GameWindow* game_window_create(const char* title, int width, int height)
{
    // Tell SDL our window should be resizeable
    uint32_t window_flags = SDL_WINDOW_RESIZABLE;

    // Create the window passing our desired title, position, size, and flags.
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

    // Allocate memory for the GameWindow and GameWindowInternal structs
    uintptr_t window_and_internal_size = 
        sizeof(GameWindow) + sizeof(GameWindowInternal);
    GameWindow* game_window = malloc(window_and_internal_size);
    game_window->internal = (GameWindowInternal*)(game_window + 1);

    // Assign initial values to the struct
    game_window->internal->window_handle = sdl_window;
    game_window->internal->surface = 0;

    game_window->window_width = width;
    game_window->window_height = height;
    game_window->pixel_buffer_width = 0;
    game_window->pixel_buffer_height = 0;
    game_window->pixels = 0;
    game_window->flags = GAME_WINDOW_FLAGS_NONE;

    // Return the pointer to our GameWindow struct.
    return game_window;
}
```

Many times in programming symmetry is applied, so when we add a function to create something we'll also want to be able to destroy or free that same thing. Add the `game_window_destroy` function to `src/game_window.c`:

```c
void game_window_destroy(GameWindow* game_window)
{
    // Tell SDL to destroy our window
    SDL_DestroyWindow(game_window->internal->window_handle);
    game_window->internal->window_handle = 0;

    // Free the memory we allocated for the GameWindow struct
    free(game_window);
}
```

Next up is processing window events. These events is generated by `SDL2` and should be processed regularly. There's multiple different kind of events but we're only interested in knowing if the user want to close the window or if the window changes size for now. Add the following function to `src/game_window.c`:
```c
void game_window_process_events(GameWindow* game_window)
{
    // Use SDL_PollEvent in a loop to retrieve events until there's no more events available.
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
```

`SDL_PollEvent` will look in the queue of events and retrieve the first one and remove it from the queue. We do this in a loop to retrieve all the events until there's none left. This should happen regularly because otherwise the events queue will fill up and window may become unresponsive.

We're using a `switch` statement to determine what type of event was retrieved. If it was a `SDL_WINDOWEVENT` we'll check if it was the window being shown or resized, if it was a `SDL_QUIT` event we'll update the `game_window->flags` field to indicate the window was closed.

Above the `game_window_process_events` function we should add the `game_window_update_size` function. This function isn't declared in the `src/game_window.h` header file so it needs to be defined before being used. The function is responsible for reading and updating the window size as well as the underlying pixel buffer size these two may differ due to the screen DPI.

Add the function above `game_window_process_events`:
```c
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
```

Finally to modify the buffer of pixels we'll have to retrieve a pointer to the memory where it's stored. Aditionally it's best to 'lock' the buffer, this is not applicable to all configurations but doesn't hurt much to do it. Add the following two functions for locking and unlocking the buffer to the end of `src/game_window.c`:

```c
void game_window_surface_lock_pixels(GameWindow* game_window)
{
    // Retrieve pointer to SDL_Surface
    SDL_Surface *surface = 
        SDL_GetWindowSurface(game_window->internal->window_handle);
    
    // Lock the surface
    SDL_LockSurface(surface);

    // Assign internal state as well as the pixels pointer
    game_window->internal->surface = surface;
    game_window->pixels = surface->pixels;
}

void game_window_surface_unlock_and_update_pixels(GameWindow* game_window)
{
    // Reset the pixels pointer to prevent accidentally writing to it while
    // surface isn't locked.
    game_window->pixels = 0;

    // Unlock the surface
    SDL_UnlockSurface(game_window->internal->surface);

    // Tell SDL2 to redraw the window with content from the pixels buffer
    SDL_UpdateWindowSurface(game_window->internal->window_handle);
}
```

Now the `src/game_window.c` file is done and we just have to modify `src/main.c` to call the functions we've just created.

### main.c
Since all the window logic is nicely encapsulated inside `src/game_window.c` we just have to make pretty minor modifications to the `src/main.c` file. Add the following content to `src/main.c` between the `SDL_Init(SDL_INIT_VIDEO)` and `SDL_Quit()` calls:

```c
    GameWindow* game_window = 
        game_window_create("back_to_basics", 680, 480);

    // Keep running while the window is open
    while ((game_window->flags & GAME_WINDOW_FLAGS_CLOSED) == 0)
    {
        // Process the window events
        game_window_process_events(game_window);

        // Lock the window surface pixel buffer
        game_window_surface_lock_pixels(game_window);

        // Write blue color to every pixel
        uint32_t *pixels_32bpp = (uint32_t *)game_window->pixels;
        for (int y = 0; y < game_window->pixel_buffer_height; y++)
        {
            uint32_t row = y * game_window->pixel_buffer_width;
            for (int x = 0; x < game_window->pixel_buffer_width; x++)
            {
                pixels_32bpp[x + row] = 0xff0000ff;
            }
        }

        // Unlock the pixel buffer and update window
        game_window_surface_unlock_and_update_pixels(game_window);

        // Wait 10 milliseconds so the CPU isn't working all the time
        SDL_Delay(10);
    }
```

Compiling and running the code now should display a window filled with blue color. Don't forget to commit the changes using `git` after verifying all the code is working properly.

## Full diff for this chapter

```diff
diff --git a/CMakeLists.txt b/CMakeLists.txt
index 7164dcf..90d4d68 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -4,4 +4,8 @@ project(back_to_basics)
 set(CMAKE_C_COMPILER /usr/bin/clang)
 set(CMAKE_C_STANDARD 11)

-add_executable(hello_world src/hello_world.c)
+find_package(SDL2 REQUIRED)
+include_directories(back_to_basics ${SDL2_INCLUDE_DIRS})
+
+add_executable(back_to_basics src/main.c src/game_window.c)
+target_link_libraries(back_to_basics ${SDL2_LIBRARIES})
```
