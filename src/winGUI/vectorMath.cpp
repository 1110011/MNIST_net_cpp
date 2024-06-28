#include "vectorMath.h"

vec2::vec2(void)
{
    this->x = 0;
    this->y = 0;
}

vec2::vec2(const float X, const float Y)
{
    this->x = X;
    this->y = Y;
}

float vec2::mag(void)
{
    return sqrt(pow2(this->x) + pow2(this->y));
}

float vec2::qmag(void)
{
    return pow2(this->x) + pow2(this->y);
}

vec2 vec2::norm(void)
{
    const float r = 1.0f/this->mag();
    return vec2(this->x*r, this->y*r);
}

vec2 vec2::perp(void)
{
    return vec2(-this->y, this->x);
}

vec2 vec2::operator+(const vec2& rhs)
{
    return vec2(this->x + rhs.x, this->y + rhs.y);
}

vec2 vec2::operator-(const vec2& rhs)
{
    return vec2(this->x - rhs.x, this->y - rhs.y);
}

vec2 vec2::operator*(const float& rhs)
{
    return vec2(this->x * rhs,   this->y * rhs);
}

vec2 vec2::operator*(const vec2& rhs)
{
    return vec2(this->x * rhs.x, this->y * rhs.y);
}

vec2 vec2::operator/(const float& rhs)
{
    return vec2(this->x / rhs,   this->y / rhs);
}

vec2& vec2::operator+=(const vec2& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

vec2& vec2::operator-=(const vec2& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

vec2& vec2::operator*=(const float& rhs)
{
    this->x *= rhs;
    this->y *= rhs;
    return *this;
}

vec2& vec2::operator/=(const float& rhs)
{
    this->x /= rhs;
    this->y /= rhs;
    return *this;
}

void vec2::print(void)
{
    std::cout << this->x << ',' << this->y << '\n';
}

vec3::vec3(void) : x(0), y(0), z(0) {}
vec3::vec3(float s) : x(s), y(s), z(s) {}
//vec3(float X, float Y,float Z) : x(X), y(Y), z(Z) {}

float vec3::mag(void)
{
    return sqrt(pow2(this->x) + pow2(this->y) + pow2(this->z));
}

float vec3::qmag(void)
{
    return pow2(this->x) + pow2(this->y) + pow2(this->z);
}

vec3 vec3::norm(void)
{
    float r = 1.0f/mag();
    return vec3(this->x*r, this->y*r, this->z*r);
}

vec3 vec3::perp(void)
{
    return vec3(-this->z, this->y, this->x);
}

vec3 vec3::inverse(void)
{
    return vec3(1.0f/this->x, 1.0f/this->y, 1.0f/this->z);
}

vec2 vec3::v2nx(void)
{
    return vec2(this->y, this->z);
}

vec2 vec3::v2ny(void)
{
    return vec2(this->x, this->z);
}

vec2 vec3::v2nz(void)
{
    return vec2(this->x, this->y);
}

vec3 vec3::extX(void)
{
    return vec3(this->x, 0.0f, 0.0f);
}

vec3 vec3::extY(void)
{
    return vec3(0.0f, this->y, 0.0f);
}

vec3 vec3::extZ(void)
{
    return vec3(0.0f, 0.0f, this->z);
}

vec3 vec3::operator+(const vec3& rhs)
{
    return vec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

vec3 vec3::operator+(const double& rhs)
{
    return vec3(this->x + rhs,   this->y + rhs,   this->z + rhs);
}

vec3 vec3::operator-(const vec3& rhs)
{
    return vec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

vec3 vec3::operator-(const double& rhs)
{
    return vec3(this->x - rhs,   this->y - rhs,   this->z - rhs);
}

vec3 vec3::operator*(const vec3& rhs)
{
    return vec3(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
}

vec3 vec3::operator/(const vec3& rhs)
{
    return vec3(this->x/rhs.x, this->y/rhs.y, this->z/rhs.z);
}

vec3 vec3::operator/(const float& rhs)
{
    return vec3(this->x/rhs, this->y/rhs, this->z/rhs);
}

vec3& vec3::operator+=(const vec3& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

vec3& vec3::operator*=(const float& rhs)
{
    this->x *= rhs;
    this->y *= rhs;
    this->z *= rhs;
    return *this;
}

vec3& vec3::operator*=(const vec3& rhs)
{
    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;
    return *this;
}

vec3& vec3::operator/=(const float& rhs)
{
    this->x /= rhs;
    this->y /= rhs;
    this->z /= rhs;
    return *this;
}

vec3& vec3::operator/=(const vec3& rhs)
{
    this->x /= rhs.x;
    this->y /= rhs.y;
    this->z /= rhs.z;
    return *this;
}

void vec3::roundToInt(void)
{
    this->x = round(this->x);
    this->y = round(this->y);
    this->z = round(this->z);
}

void vec3::print(void)
{
    std::cout << this->x << ',' << this->y << ',' << this->z << '\n';
}

vec3_double::vec3_double(void) : x(0), y(0), z(0) {}
vec3_double::vec3_double(const double X, const double Y,double Z) : x(X), y(Y), z(Z) {}
vec3_double::vec3_double(const vec3 v)
{
    this->x = (double)v.x;
    this->y = (double)v.y;
    this->z = (double)v.z;
}

double vec3_double::mag(void)
{
    return sqrt(pow2(this->x) + pow2(this->y) + pow2(this->z));
}

double vec3_double::qmag(void)
{
    return pow2(this->x) + pow2(this->y) + pow2(this->z);
}

vec3_double vec3_double::norm(void)
{
    double r = 1.0d / this->mag();
    return vec3_double(x*r, y*r, z*r);
}
vec3_double vec3_double::perp(void)
{
    return vec3_double(-z, y, x);
}

vec3_double vec3_double::operator+(const vec3_double& rhs)
{
    return vec3_double(this->x+rhs.x, this->y+rhs.y, this->z+rhs.z);
}

vec3_double vec3_double::operator+(const double& rhs)
{
    return vec3_double(this->x+rhs, this->y+rhs, this->z+rhs);
}

vec3_double vec3_double::operator-(const vec3_double& rhs)
{
    return vec3_double(this->x-rhs.x, this->y-rhs.y, this->z-rhs.z);
}

vec3_double vec3_double::operator-(const double& rhs)
{
    return vec3_double(this->x-rhs, this->y-rhs, this->z-rhs);
}

vec3_double vec3_double::operator*(const double& rhs)
{
    return vec3_double(this->x*rhs, this->y*rhs, this->z*rhs);
}

vec3_double vec3_double::operator*(const vec3_double& rhs)
{
    return vec3_double(this->x*rhs.x, this->y*rhs.y, this->z*rhs.z);
}

vec3_double vec3_double::operator/(const vec3_double& rhs)
{
    return vec3_double(this->x/rhs.x, this->y/rhs.y, this->z/rhs.z);
}

vec3_double vec3_double::operator/(const double& rhs)
{
    return vec3_double(this->x/rhs, this->y/rhs, this->z/rhs);
}

vec3_double& vec3_double::operator+=(const vec3_double& rhs)
{
    this -> x += rhs.x;
    this -> y += rhs.y;
    this -> z += rhs.z;
    return *this;
}

vec3_double& vec3_double::operator-=(const vec3_double& rhs)
{
    this -> x -= rhs.x;
    this -> y -= rhs.y;
    this -> z -= rhs.z;
    return *this;
}

vec3_double& vec3_double::operator*=(const double& rhs)
{
    this -> x *= rhs;
    this -> y *= rhs;
    this -> z *= rhs;
    return *this;
}

vec3_double& vec3_double::operator*=(const vec3_double& rhs)
{
    this -> x *= rhs.x;
    this -> y *= rhs.y;
    this -> z *= rhs.z;
    return *this;
}

vec3_double& vec3_double::operator/=(const double& rhs)
{
    this -> x /= rhs;
    this -> y /= rhs;
    this -> z /= rhs;
    return *this;
}

vec3_double& vec3_double::operator/=(const vec3& rhs)
{
    this -> x /= rhs.x;
    this -> y /= rhs.y;
    this -> z /= rhs.z;
    return *this;
}

void vec3_double::print(void)
{
    std::cout << this->x << ',' << this->y << ',' << this->z << '\n';
}
