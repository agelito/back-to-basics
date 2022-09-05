// renderer.c

#include <stdint.h>
#include "renderer.h"
#include "math_utils.h"

static inline int32_t 
signed_area2(RendererPoint p0, RendererPoint p1, RendererPoint p2)
{
    return (p1.x - p0.x) * (p2.y - p0.y) -
        (p1.y - p0.y) * (p2.x - p0.x);
}

RendererTargetBuffer
renderer_create_target_buffer(int32_t width, int32_t height, int32_t bytes_per_pixel, uint8_t *pixels)
{
    RendererTargetBuffer target = {
        width, height, bytes_per_pixel, pixels
    };

    return target;
}

void 
renderer_fill(RendererTargetBuffer buffer, uint32_t color)
{
    uint32_t fill_until = buffer.width * buffer.height * buffer.bytes_per_pixel;
    uint32_t per_pixel_add = buffer.bytes_per_pixel;
    for (int offset = 0; offset < fill_until; offset += per_pixel_add)
    {
        PutPixelByteOffset(buffer, offset, color);
    }
}

void 
renderer_fill_rect(RendererTargetBuffer buffer, RendererRect rect, uint32_t color)
{
    for (int y = rect.y; y < rect.y + rect.h; y++)
    {
        for(int x = rect.x; x < rect.x + rect.w; x++)
        {
            PutPixelXY(buffer, x, y, color);
        }
    }
}

void
renderer_fill_triangle(RendererTargetBuffer buffer, RendererTriangle triangle)
{
    RendererPoint p0 = triangle.p0;
    RendererPoint p1 = triangle.p1;
    RendererPoint p2 = triangle.p2;

    int32_t min_x = Min3(p0.x, p1.x, p2.x);
    int32_t max_x = Max3(p0.x, p1.x, p2.x);
    int32_t min_y = Min3(p0.y, p1.y, p2.y);
    int32_t max_y = Max3(p0.y, p1.y, p2.y);

    int32_t a12 = p1.y - p2.y; int32_t b12 = p2.x - p1.x;
    int32_t a20 = p2.y - p0.y; int32_t b20 = p0.x - p2.x;
    int32_t a01 = p0.y - p1.y; int32_t b01 = p1.x - p0.x;

    RendererPoint test_p = { min_x, min_y };

    int32_t bcoord_row0 = signed_area2(p1, p2, test_p);
    int32_t bcoord_row1 = signed_area2(p2, p0, test_p);
    int32_t bcoord_row2 = signed_area2(p0, p1, test_p);
    
    int32_t total_area2 = bcoord_row0 + bcoord_row1 + bcoord_row2;
    float total_area2_inv = 1.0f / total_area2;

    uint8_t color_r0; uint8_t color_r1; uint8_t color_r2;
    uint8_t color_g0; uint8_t color_g1; uint8_t color_g2;
    uint8_t color_b0; uint8_t color_b1; uint8_t color_b2;

    UnpackColorRGB(triangle.c0, color_r0, color_g0, color_b0);
    UnpackColorRGB(triangle.c1, color_r1, color_g1, color_b1);
    UnpackColorRGB(triangle.c2, color_r2, color_g2, color_b2);

    int32_t color_base_r0 = (int32_t)(color_r0 << 18);
    int32_t color_base_g0 = (int32_t)(color_g0 << 18);
    int32_t color_base_b0 = (int32_t)(color_b0 << 18);

    int32_t color_r10 = (int32_t)(((color_r1 - color_r0) << 18) * total_area2_inv);
    int32_t color_r20 = (int32_t)(((color_r2 - color_r0) << 18) * total_area2_inv);
    int32_t color_g10 = (int32_t)(((color_g1 - color_g0) << 18) * total_area2_inv);
    int32_t color_g20 = (int32_t)(((color_g2 - color_g0) << 18) * total_area2_inv);
    int32_t color_b10 = (int32_t)(((color_b1 - color_b0) << 18) * total_area2_inv);
    int32_t color_b20 = (int32_t)(((color_b2 - color_b0) << 18) * total_area2_inv);

    for(test_p.y = min_y; test_p.y < max_y; test_p.y++)
    {
        int32_t bcoord0 = bcoord_row0;
        int32_t bcoord1 = bcoord_row1;
        int32_t bcoord2 = bcoord_row2;

        for(test_p.x = min_x; test_p.x < max_x; test_p.x++)
        {
            if ((bcoord0|bcoord1|bcoord2) >= 0)
            {
                uint8_t color_r = (uint8_t)((color_base_r0 + bcoord1 * color_r10 + bcoord2 * color_r20) >> 18);
                uint8_t color_g = (uint8_t)((color_base_g0 + bcoord1 * color_g10 + bcoord2 * color_g20) >> 18);
                uint8_t color_b = (uint8_t)((color_base_b0 + bcoord1 * color_b10 + bcoord2 * color_b20) >> 18);
                uint32_t pixel_color = PackColorRGB(color_r, color_g, color_b);

                PutPixelXY(buffer, test_p.x, test_p.y, pixel_color);
            }

            bcoord0 += a12;
            bcoord1 += a20;
            bcoord2 += a01;
        }

        bcoord_row0 += b12;
        bcoord_row1 += b20;
        bcoord_row2 += b01;
    }
}