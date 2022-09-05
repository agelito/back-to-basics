// math_utils.h

#ifndef MATH_UTILS_INCLUDED
#define MATH_UTILS_INCLUDED

#define Min(x, y) (x < y ? x : y)
#define Min3(x, y, z) Min(x, Min(y, z))

#define Max(x, y) (x > y ? x : y)
#define Max3(x, y, z) Max(x, Max(y, z))

#endif // MATH_UTILS_INCLUDED