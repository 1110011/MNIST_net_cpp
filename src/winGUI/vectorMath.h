#ifndef VECTORMATH_H_INCLUDED
#define VECTORMATH_H_INCLUDED

#include "mathUtils.h"
#include <math.h>
#include <iostream>

struct vec2
{
    float x,y;

    vec2(void);
    vec2(const float, const float);

    float mag(void);
    float qmag(void);
    vec2 norm(void);
    vec2 perp(void);

    vec2 operator+(const vec2&);
    vec2 operator-(const vec2&);
    vec2 operator*(const float&);
    vec2 operator*(const vec2&);
    vec2 operator/(const float&);

    vec2& operator+=(const vec2&);
    vec2& operator-=(const vec2&);
    vec2& operator*=(const float&);
    vec2& operator/=(const float&);

    void print(void);

};

struct vec3
{
    float x,y,z;

    vec3(void);
    vec3(float s);
    //vec3(float X, float Y,float Z) : x(X), y(Y), z(Z) {}
    constexpr vec3(const float X, const float Y, const float Z) : x(X), y(Y), z(Z) {}

    float mag(void);
    float qmag(void);
    vec3 norm(void);
    vec3 perp(void);
    vec3 inverse(void);

    vec2 v2nx(void);
    vec2 v2ny(void);
    vec2 v2nz(void);

    vec3 extX(void);
    vec3 extY(void);
    vec3 extZ(void);

    vec3 operator+(const vec3&);
    vec3 operator+(const double&);
    vec3 operator-(const vec3&);
    vec3 operator-(const double&);
    //vec3 operator*(const float&);
    vec3 operator*(const vec3&);
    vec3 operator/(const vec3&);
    vec3 operator/(const float&);

    vec3& operator+=(const vec3&);
    vec3& operator-=(const vec3&);
    vec3& operator*=(const float&);
    vec3& operator*=(const vec3&);
    vec3& operator/=(const float&);
    vec3& operator/=(const vec3&);

    constexpr vec3 operator * (const float& rhs) {return vec3(this->x * rhs, this->y * rhs, this->z * rhs);}

    void roundToInt(void);

    void print(void);
};

inline vec3 randVec3(vec3 min, vec3 max)
{
    return vec3(randFloat(min.x,max.x),randFloat(min.y,max.y),randFloat(min.z,max.z));
}

inline vec3 randVec3(float min, float max)
{
    return vec3(randFloat(min,max),randFloat(min,max),randFloat(min,max));
}

struct vec3_double
{
    double x,y,z;

    vec3_double(void);
    vec3_double(const double, const double, const double);
    vec3_double(const vec3);

    double mag(void);
    double qmag(void);
    vec3_double norm(void);
    vec3_double perp(void);

    vec3_double operator + (const vec3_double&);
    vec3_double operator + (const double&);
    vec3_double operator - (const vec3_double&);
    vec3_double operator - (const double&);
    vec3_double operator * (const double&);
    vec3_double operator * (const vec3_double&);
    vec3_double operator / (const vec3_double&);
    vec3_double operator / (const double&);

    vec3_double& operator += (const vec3_double&);
    vec3_double& operator -= (const vec3_double&);
    vec3_double& operator *= (const double&);
    vec3_double& operator *= (const vec3_double&);
    vec3_double& operator /= (const double&);
    vec3_double& operator /= (const vec3&);

    void print(void);
};

inline float dot(vec3 a, vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

/// expands vec2 to vec3; zeros x
inline vec3 expv2zx(const vec2 a)
{
    return vec3(0.0f, a.x, a.y);
}

/// expands vec2 to vec3; zeros y
inline vec3 expv2zy(const vec2 a)
{
    return vec3(a.x, 0.0f, a.y);
}

/// expands vec2 to vec3; zeros z
inline vec3 expv2zz(const vec2 a)
{
    return vec3(a.x, a.y, 0.0f);
}

/// computes the vector cross product between a and b i.e. a x b
inline vec3 cross(vec3 a, vec3 b)
{
    return vec3(a.y*b.z-b.y*a.z,
                a.z*b.x-b.z*a.x,
                a.x*b.y-b.x*a.y);
}

constexpr vec3 colorFromHex(unsigned int hexColor)
{
    return vec3(float((hexColor&0xFF0000)>>16) / 255.0f, float((hexColor&0x00FF00)>>8) / 255.0f, float((hexColor&0x0000FF)) / 255.0f);
}

#endif // VECTORMATH_H_INCLUDED
