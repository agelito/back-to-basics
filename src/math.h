// math.h

#ifndef MATH_INCLUDED
#define MATH_INCLUDED

#include <math.h>

#define Min(x, y) (x < y ? x : y)
#define Min3(x, y, z) Min(x, Min(y, z))

#define Max(x, y) (x > y ? x : y)
#define Max3(x, y, z) Max(x, Max(y, z))

typedef struct Vector3
{
    union
    {
        float xyz[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
} Vector3;

typedef struct Vector4
{
    union
    {
        float xyzw[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        };
    };
} Vector4;

static inline Vector3
vector3_create(float x, float y, float z)
{
    Vector3 result = {
        x, y, z
    };

    return result;
}

static inline Vector3
vector3_subtract(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

static inline float
vector3_length(Vector3 a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline Vector3
vector3_normalize(Vector3 a)
{
    float length = vector3_length(a);
    if (length <= 0.0f)
    {
        return vector3_create(0.0f, 0.0f, 1.0f);
    }

    Vector3 result = vector3_create(a.x / length, a.y / length, a.z / length);

    return result;
}

static inline Vector3
vector3_cross(Vector3 a, Vector3 b)
{
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.x - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;
    return vector3_create(x, y, z);
}

static inline float
vector3_dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Vector3
vector4_mul(Vector4 a, Vector3 b)
{
    Vector3 result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

typedef struct Matrix4
{
    union {
        struct {
            float x1; float y1; float z1; float w1;
            float x2; float y2; float z2; float w2;
            float x3; float y3; float z3; float w3;
            float x4; float y4; float z4; float w4;
        };
        float m[16];
    };
} Matrix4;

static inline Matrix4
matrix4_lookat_lh(Vector3 eye, Vector3 at, Vector3 up)
{
    Vector3 z = vector3_normalize(vector3_subtract(at, eye));
    Vector3 x = vector3_normalize(vector3_cross(up, z));
    Vector3 y = vector3_cross(z, x);

    float eye_x = vector3_dot(x, eye);
    float eye_y = vector3_dot(y, eye);
    float eye_z = vector3_dot(z, eye);

    Matrix4 result = {
        x.x, x.y, x.z, 0.0f,
        y.x, y.y, y.z, 0.0f,
        z.x, z.y, z.z, 0.0f,
        -eye_x, -eye_y, -eye_z, 1.0f
    };

    return result;
}

static inline Matrix4
matrix4_perspective_lh(float fov, float aspect_ratio, float z_near, float z_far)
{
    float degrees_to_radius = M_PI / 180.0f;
    float height = 1.0f / tanf(degrees_to_radius * fov / 2.0f);
    float width = height / aspect_ratio;

    float far_near = z_far - z_near;

    Matrix4 result = {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, z_far / far_near, 1.0f,
        0.0f, 0.0f, -z_near * z_far / far_near, 0.0f};

    return result;
}

static inline Matrix4
matrix4_rotate_x(float rotation)
{
    float rot_sin = sinf(rotation);
    float rot_cos = cosf(rotation);

    Matrix4 result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, rot_cos, -rot_sin, 0.0f,
        0.0f, rot_sin, rot_cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return result;
}

static inline Matrix4
matrix4_rotate_y(float rotation)
{
    float rot_sin = sinf(rotation);
    float rot_cos = cosf(rotation);

    Matrix4 result = {
        rot_cos, 0.0f, rot_sin, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -rot_sin, 0.0f, rot_cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    return result;
}

static inline Matrix4
matrix4_rotate_z(float rotation)
{
    float rot_sin = sinf(rotation);
    float rot_cos = cosf(rotation);

    Matrix4 result = {
        rot_cos, -rot_sin, 0.0f, 0.0f,
        rot_sin, rot_cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    return result;
}

static inline Matrix4 
matrix4_multiply(Matrix4 a, Matrix4 b)
{
    Matrix4 result = {0.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 0.0f};

    int i,j,k;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            for (k = 0; k < 4; k++)
            {
                result.m[j+i*4] += a.m[k+i*4]*b.m[j+k*4];
            }
        }
    }
    return result;
}

static inline Vector4
matrix4_multiply_vector3(Matrix4 m, Vector3 v)
{
    float x = v.x, y = v.y, z = v.z;
    Vector4 result = {(x*m.x1 + y * m.x2 + z * m.x3 + m.x4),
                      (x*m.y1 + y * m.y2 + z * m.y3 + m.y4),
                      (x*m.z1 + y * m.z2 + z * m.z3 + m.z4),
                      (x*m.w1 + y * m.w2 + z * m.w3 + m.w4)};
    return result;
}

#endif // MATH_INCLUDED