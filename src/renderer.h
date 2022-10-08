// renderer.h

#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

typedef struct RendererTargetBuffer {
    int32_t width;
    int32_t height;
    int32_t bytes_per_pixel;
    uint8_t* pixels;
} RendererTargetBuffer;

typedef struct RendererRect {
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
} RendererRect;

typedef struct RendererPoint {
    int32_t x;
    int32_t y;
} RendererPoint;

static inline RendererPoint
renderer_point_create(int32_t x, int32_t y)
{
    RendererPoint result = {
        x, y};

    return result;
}

typedef struct RendererTriangle {
    RendererPoint p0;
    RendererPoint p1;
    RendererPoint p2;
    uint32_t c0;
    uint32_t c1;
    uint32_t c2;
} RendererTriangle;

// Pack 3 color bytes into one uint32
#define PackColorRGB(r, g, b) (r & 0xff) << 16 | (g & 0xff) << 8 | (b & 0xff)

// Pack 4 color bytes into one uint32
#define PackColorRGBA(r, g, b, a) (a & 0xff) << 24 | PackColorRGB(r, g, b)

// Unpack 3 color bytes from one uint32
#define UnpackColorRGB(color, r, g, b) r=(color>>16)&0xff;g=(color>>8)&0xff;b=(color>>0)&0xff

// Index pixel at x, y coordinates within RendererTargetBuffer target
#define IndexPixel(x, y, target) (x + y * target.width) * target.bytes_per_pixel

// Put pixel at offset into target
#define PutPixelByteOffset(target, offset, color) *(uint32_t *)(target.pixels + offset) = color

// Put pixel at x,y into target
#define PutPixelXY(target, x, y, color) PutPixelByteOffset(target, IndexPixel(x, y, target), color)

RendererTargetBuffer 
renderer_create_target_buffer(int32_t width, int32_t height, int32_t bytes_per_pixel, uint8_t* pixels);

void
renderer_fill(RendererTargetBuffer buffer, uint32_t color);

void 
renderer_fill_rect(RendererTargetBuffer buffer, RendererRect rect, uint32_t color);

void 
renderer_fill_triangle(RendererTargetBuffer buffer, RendererTriangle triangle);

#endif // RENDERER_INCLUDED