// main.c

#include <SDL2/SDL.h>

#include "game_window.h"
#include "renderer.h"
#include "vertex_transform.h"

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("failed initializing SDL2\n");
        return -1;
    }

    GameWindow* game_window = 
        game_window_create("back_to_basics", 680, 480);

    float rotation = 0.0f;

    while ((game_window->flags & GAME_WINDOW_FLAGS_CLOSED) == 0)
    {
        game_window_process_events(game_window);

        game_window_surface_lock_pixels(game_window);

        int32_t bytes_per_pixel = 4;
        RendererTargetBuffer pixel_buffer = 
            renderer_create_target_buffer(game_window->pixel_buffer_width, game_window->pixel_buffer_height, bytes_per_pixel, game_window->pixels);

        renderer_fill(pixel_buffer, PackColorRGB(0, 0, 0));

        if (game_window->pixel_buffer_width != 0)
        {
            RendererRect top_left = {
                0, 0, 32, 32
            };

            RendererRect top_right = {
                pixel_buffer.width - 32,
                0, 32, 32
            };

            RendererRect bottom_left = {
                0, 
                pixel_buffer.height - 32,
                32, 32
            };

            RendererRect bottom_right = {
                pixel_buffer.width - 32,
                pixel_buffer.height - 32,
                32, 32
            };

            renderer_fill_rect(pixel_buffer, top_left, PackColorRGB(255, 0, 0));
            renderer_fill_rect(pixel_buffer, top_right, PackColorRGB(0, 255, 0));
            renderer_fill_rect(pixel_buffer, bottom_left, PackColorRGB(0, 255, 255));
            renderer_fill_rect(pixel_buffer, bottom_right, PackColorRGB(255, 255, 0));

            float aspect_ratio = (float)pixel_buffer.width / pixel_buffer.height;
            Matrix4 projection = matrix4_perspective_lh(
                45.0f, aspect_ratio, 0.01f, 100.0f);

            Matrix4 view = matrix4_lookat_lh(
                vector3_create(0.0f, 0.0f, -6.0f),
                vector3_create(0.0f, 0.0f, 0.0f),
                vector3_create(0.0f, 1.0f, 0.0f));

            Matrix4 model = matrix4_rotate_y(rotation);
            rotation += 0.04f;

            Matrix4 model_view = matrix4_multiply(model, view);
            Matrix4 transform = matrix4_multiply(model_view, projection);

            Vector3 positions[6] = {
                vector3_create(-1.0f, -1.0f, 0.0f),
                vector3_create(0.0f, 1.0f, 0.0f),
                vector3_create(1.0f, -1.0f, 0.0f),
                vector3_create(0.0f, 1.0f, 0.0f),
                vector3_create(-1.0f, -1.0f, 0.0f),
                vector3_create(1.0f, -1.0f, 0.0f)};
            Vector3 transformed[6];

            vertex_transform_positions(transform, positions, transformed, 6);
            vertex_transform_map_to_viewport(
                pixel_buffer.width, pixel_buffer.height, transformed, positions, 6);

            RendererTriangle triangle;
            triangle.p0 = renderer_point_create((int32_t)positions[0].x, (int32_t)positions[0].y);
            triangle.p1 = renderer_point_create((int32_t)positions[1].x, (int32_t)positions[1].y);
            triangle.p2 = renderer_point_create((int32_t)positions[2].x, (int32_t)positions[2].y);

            RendererTriangle triangle2;
            triangle2.p0 = renderer_point_create((int32_t)positions[3].x, (int32_t)positions[3].y);
            triangle2.p1 = renderer_point_create((int32_t)positions[4].x, (int32_t)positions[4].y);
            triangle2.p2 = renderer_point_create((int32_t)positions[5].x, (int32_t)positions[5].y);

            triangle.c0 = PackColorRGB(255, 0, 0);
            triangle.c1 = PackColorRGB(0, 255, 0);
            triangle.c2 = PackColorRGB(0, 0, 255);

            triangle2.c0 = PackColorRGB(0, 255, 0);
            triangle2.c1 = PackColorRGB(255, 0, 0);
            triangle2.c2 = PackColorRGB(0, 0, 255);

            renderer_fill_triangle(pixel_buffer, triangle);
            renderer_fill_triangle(pixel_buffer, triangle2);
        }

        game_window_surface_unlock_and_update_pixels(game_window);

        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}