// vertex_transform.h

#ifndef VERTEX_TRANSFORM_H
#define VERTEX_TRANSFORM_H

#include "math.h"

void
vertex_transform_positions(Matrix4 transform, Vector3* positions, Vector3* transformed_positions, int count);

void
vertex_transform_map_to_viewport(int width, int height, Vector3 *positions, Vector3 *mapped_positions, int count);

#endif // VERTEX_TRANSFORM_H