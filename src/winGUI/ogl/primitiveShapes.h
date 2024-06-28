#ifndef PRIMITIVESHAPES_H_INCLUDED
#define PRIMITIVESHAPES_H_INCLUDED

#define SHP_ELLIPSE_DIVISIONS 128

#define SHP_BEZIER_DIVISIONS 128

#define SHP_QUAD 0
#define SHP_ELLIPSE 1
#define SHP_SUPERELLIPSE_N4 2
#define SHP_LINE 3
#define SHP_SUPERELLIPSE_N16 4
#define SHP_CUBIC_BEZIER 5
#define SHP_QUADRATIC_BEZIER 6

#include "glslShaderCompiler.h"
#include "../vectorMath.h"
#include "../matrixMath.h"
#include "../mathUtils.h"
#include <vector>

class shapeRenderer
{
private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    glslShader flatPolygonShader = glslShader();
    unsigned int colorLoc = 0;
    unsigned int transformLoc = 0;
    mat4 transform = mat4();

public:
    shapeRenderer(const std::string="winGUI\\ogl\\shaders\\flatPoly.ver", const std::string="winGUI\\ogl\\shaders\\flatPoly.frg");
    shapeRenderer(const unsigned int, const unsigned int);

    void render(vec2, vec2, vec3, uint_fast16_t, vec2=vec2(), vec2=vec2());
};

#endif // PRIMITIVESHAPES_H_INCLUDED
