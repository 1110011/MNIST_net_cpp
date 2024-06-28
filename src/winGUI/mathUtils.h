#ifndef MATHUTILS_H_INCLUDED
#define MATHUTILS_H_INCLUDED

#define M_PI 3.14159265358979
#define M_E 2.71828182846

#include <math.h>
#include <cstdint>

inline float pow3(const float a)
{
    return a*a*a;
}

inline float pow2(const float a)
{
    return a*a;
}

inline int hexToBin(char h)
{
    if(h < 65)
        return h-48;
    else if(h < 97)
        return h-55;
    else return h-87;
}

inline float max3(const float a, const float b, const float c)
{
    return std::max(std::max(a,b),c);
}

inline float easeInOutCubic(const float x)
{
    return x < 0.5f ? 4*pow3(x) : 1 - pow3(-2 * x + 2) / 2;
}

// finds the absolute value of "a" using dark magic (and some rather simple bit manipulation)
// at least on my computer this is much faster than std::fabs
// warning: this function is only safe to use if "a" is definitely a "normal" number and not something like NaN or inf
inline float evilBithack_fabs(float a)
{
    uint32_t* ia = reinterpret_cast<uint32_t*>(&a);
    *ia &= 0x7FFFFFFF;
    return a;
}

inline float sign(float a)
{
    if(a > 0) return 1.0f;
    return -1.0f;
}

inline float randFloat(float min, float max)
{
    float a = (float)rand()/(float)RAND_MAX;
    return a*(max-min)+min;
}

inline bool randBool()
{
    return rand()&1;
}

inline uint8_t randUI8()
{
    return rand()&0xFF;
}

inline uint16_t randUI16()
{
    return randUI8()|(randUI8()<<8);
}

inline uint32_t randUI32()
{
    return randUI16()|(randUI16()<<16);
}

inline double hash(double seed)
{
    double tmp;
    return modf(sin(seed*78.233d)*43758.5453d,&tmp);
}

inline double hashRand(double seed, double min, double max)
{
    double a = hash(seed)*0.5d+1.0d;
    return a*(max-min)+min;
}

#endif // MATHUTILS_H_INCLUDED
