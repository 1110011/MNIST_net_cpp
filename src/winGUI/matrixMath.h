#ifndef MATRIXMATH_H_INCLUDED
#define MATRIXMATH_H_INCLUDED

#include "mathUtils.h"
#include "vectorMath.h"
#include <vector>

inline uint_fast8_t i3(uint_fast8_t index, uint_fast8_t r)
{
    return index>=r?index+1:index;
}

struct mat4
{
    float mat[4][4] = {};

    mat4(void);

    mat4(const mat4&);

    void loadIdentity(void);

    void print(void);
    void printForCollision(void);

    const float* getGLMat(void);

    mat4 operator+(const mat4&);
    mat4 operator-(const mat4&);
    mat4 operator*(const mat4&);
    vec3 operator*(const vec3&);
    mat4 operator/(const vec3&);

    vec3 getTranslation(void);

    vec3 getScale(void);

    mat4 getUntranslatedMatrix(void);
    mat4 getRotationShearMatrix(void);

    float det(void);
    float det3x3(uint_fast8_t, uint_fast8_t);
    mat4 coFactor(void);
    mat4 transpose(void);
    mat4 inverse(void);

    bool isIdentity(float tolerance=0.001f);
    bool rotatesAxisAligned(float tolerance=0.001f);
    bool rotatesYAligned(float tolerance=0.001f);
    bool isPureTranslation(float tolerance=0.001f);

};

mat4 getIdentityMatrix(void);

mat4 getBlenderToInfernoConversionMatrix(void);

mat4 getScalingMatrix(float vec[4]);
mat4 getScalingMatrix(vec3);
mat4 getScalingMatrix(float);

mat4 getTranslationMatrix(float vec[4]);
mat4 getTranslationMatrix(vec3);

mat4 getRotationMatrix(float, vec3);
mat4 getEulerRotationMatrixXZY(vec3);

mat4 getPerspectiveProjectionMatrix(float, float, float, float);
mat4 getOrthographicProjectionMatrix(float, float, float, float, float, float);
mat4 lookAt(vec3, vec3, vec3);

mat4 loadMatrixFromBlenderExport(std::vector<float>);

#endif // MATRIXMATH_H_INCLUDED
