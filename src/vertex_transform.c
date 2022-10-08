// vertex_transform.c

#include "vertex_transform.h"

void
vertex_transform_positions(Matrix4 transform, Vector3 *positions, Vector3 *transformed_positions, int count)
{
    for (int i = 0; i < count; ++i)
    {
        Vector3 position = *(positions + i);
        Vector4 transformed = matrix4_multiply_vector3(transform, position);

        Vector3 result = {
            transformed.x / transformed.w,
            transformed.y / transformed.w,
            transformed.z / transformed.w,
        };

        *(transformed_positions + i) = result;
    }
}

void
vertex_transform_map_to_viewport(int width, int height, Vector3 *positions, Vector3 *mapped_positions, int count)
{
    for (int i = 0; i < count; ++i)
    {
        Vector3 position = *(positions + i);
        Vector3 result = {
            (0.5f + position.x * 0.5f) * width,
            (1.0f - (0.5f + position.y * 0.5f)) * height,
            (0.5f + position.z * 0.5f)
        };

        *(mapped_positions + i) = result;
    }
}