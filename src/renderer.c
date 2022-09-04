// renderer.c

#include <stdint.h>
#include "renderer.h"

renderer_target_buffer
renderer_create_target_buffer(int32_t width, int32_t height, int32_t bytes_per_pixel, uint8_t *pixels)
{
    renderer_target_buffer target = {
        width, height, bytes_per_pixel, pixels
    };

    return target;
}

void 
renderer_fill(renderer_target_buffer buffer, uint32_t color)
{
    uint32_t fill_until = buffer.width * buffer.height * buffer.bytes_per_pixel;
    uint32_t per_pixel_add = buffer.bytes_per_pixel;
    for (int offset = 0; offset < fill_until; offset += per_pixel_add)
    {
        PutPixelByteOffset(buffer, offset, color);
    }
}

void 
renderer_fill_rect(renderer_target_buffer buffer, renderer_rect rect, uint32_t color)
{
    for (int y = rect.y; y < rect.y + rect.h; y++)
    {
        for(int x = rect.x; x < rect.x + rect.w; x++)
        {
            PutPixelXY(buffer, x, y, color);
        }
    }
}