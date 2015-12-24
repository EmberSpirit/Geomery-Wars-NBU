#if !defined(VECTOR_INTRINSICS_H)

#include <math.h>

#define Pi 3.14159265359

struct vec2
{
    float x;
    float y;
};

inline vec2 operator +(vec2 one, vec2 two)
{
    vec2 result;

    result.x = one.x + two.x;
    result.y = one.y + two.y;

    return result;
}

inline vec2 operator +=(vec2 one, vec2 two)
{
    vec2 result;

    result.x = one.x + two.x;
    result.y = one.y + two.y;

    return result;
}

inline vec2 operator -(vec2 one, vec2 two)
{
    vec2 result;

    result.x = one.x - two.x;
    result.y = one.y - two.y;

    return result;
}

inline vec2 operator -=(vec2 one, vec2 two)
{
    vec2 result;

    result.x = one.x - two.x;
    result.y = one.y - two.y;

    return result;
}

inline vec2 operator /(vec2 vec, float scalar)
{
    vec2 result;

    result.x = vec.x / scalar;
    result.y = vec.y / scalar;

    return result;
}

inline vec2 operator *(vec2 vec, float scalar)
{
    vec2 result;

    result.x = vec.x * scalar;
    result.y = vec.y * scalar;

    return result;
}

inline vec2 operator *(vec2 one, vec2 two)
{
    vec2 result;
    
    result.x = one.x * two.x;
    result.y = one.y * two.y;

    return result;
}

float VectorLength(vec2 vec)
{
    float result;

    result = sqrtf(vec.x * vec.x + vec.y * vec.y);

    return result;
}

vec2 NormalizeVector(vec2 vec)
{
    vec2 result;

    result = vec / VectorLength(vec);

    return result;
}

float DotProduct(vec2 one, vec2 two)
{
    float result;

    result = one.x * two.x + one.y * two.y;

    return result;
}

float CrossProduct(vec2 one, vec2 two)
{
    float result;

    result = one.x * two.y - one.y * two.x;

    return result;
}

float AngleBetweenVectors(vec2 one, vec2 two)
{
    float result;

    float dot = DotProduct(one, two);
    float cross = CrossProduct(one, two);  
    
    result = -(atan2(cross, dot) * 180) / Pi + 180;
    
    return result;
}

#define VECTOR_INTRINSICS_H
#endif
