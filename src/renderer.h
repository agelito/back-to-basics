// renderer.h

#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

typedef struct renderer_target_buffer {
    int32_t width;
    int32_t height;
    int32_t bytes_per_pixel;
    uint8_t* pixels;
} renderer_target_buffer;

typedef struct renderer_rect {
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
} renderer_rect;

// Pack 3 color bytes into one uint32
#define PackColorRGB(r, g, b) (r & 0xff) << 16 | (g & 0xff) << 8 | (b & 0xff)

// Pack 4 color bytes into one uint32
#define PackColorRGBA(r, g, b, a) (a & 0xff) << 24 | PackColorRGB(r, g, b)

// Index pixel at x, y coordinates within renderer_target_buffer target
#define IndexPixel(x, y, target) (x + y * target.width) * target.bytes_per_pixel

// Put pixel at offset into target
#define PutPixelByteOffset(target, offset, color) *(uint32_t *)(target.pixels + offset) = color

// Put pixel at x,y into target
#define PutPixelXY(target, x, y, color) PutPixelByteOffset(target, IndexPixel(x, y, target), color)

renderer_target_buffer 
renderer_create_target_buffer(int32_t width, int32_t height, int32_t bytes_per_pixel, uint8_t* pixels);

void
renderer_fill(renderer_target_buffer buffer, uint32_t color);

void 
renderer_fill_rect(renderer_target_buffer buffer, renderer_rect rect, uint32_t color);

#endif // RENDERER_INCLUDED