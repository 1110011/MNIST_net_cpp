#include "primitiveShapes.h"

shapeRenderer::shapeRenderer(const std::string vertexShader, const std::string fragmetShader)
{
    this->flatPolygonShader = glslShader(vertexShader,fragmetShader);
    this->flatPolygonShader.use();
    this->colorLoc = glGetUniformLocation(this->flatPolygonShader.getID(),"col");
    this->transformLoc = glGetUniformLocation(this->flatPolygonShader.getID(),"transform");
    this->transform = getOrthographicProjectionMatrix(-1,1,-1,1,0.0f,10);
    glUniformMatrix4fv(this->transformLoc, 1, GL_FALSE, this->transform.getGLMat());

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

shapeRenderer::shapeRenderer(const unsigned int vertexShaderID, const unsigned int fragmetShaderID)
{
    this->flatPolygonShader = glslShader(vertexShaderID,fragmetShaderID);
    this->flatPolygonShader.use();
    this->colorLoc = glGetUniformLocation(this->flatPolygonShader.getID(),"col");
    this->transformLoc = glGetUniformLocation(this->flatPolygonShader.getID(),"transform");
    this->transform = getOrthographicProjectionMatrix(-1,1,-1,1,0.0f,10);
    glUniformMatrix4fv(this->transformLoc, 1, GL_FALSE, this->transform.getGLMat());

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void shapeRenderer::render(vec2 center, vec2 size, vec3 color, uint16_t renderType, vec2 cp2, vec2 cp3)
{
    std::vector<float> data;
    unsigned int glPrimitive = GL_TRIANGLES;
    unsigned int vCnt = 0;
    float param;
    vec2 imcp1, imcp2, imcp3, imcp4, imcp5, imcp6;
    switch(renderType)
    {
    case SHP_QUAD:
        data.resize(12);
        data[0] = center.x-size.x;
        data[1] = center.y+size.y;
        data[2] = center.x+size.x;
        data[3] = center.y+size.y;
        data[4] = center.x+size.x;
        data[5] = center.y-size.y;

        data[6] = center.x-size.x;
        data[7] = center.y+size.y;
        data[8] = center.x+size.x;
        data[9] = center.y-size.y;
        data[10] = center.x-size.x;
        data[11] = center.y-size.y;
        glPrimitive = GL_TRIANGLES;
        vCnt = 6;
        break;

    case SHP_ELLIPSE:
        data.resize(5+(size_t(SHP_ELLIPSE_DIVISIONS)<<1));

        data[0] = center.x;
        data[1] = center.y;

        for(uint_fast32_t i = 0; i < SHP_ELLIPSE_DIVISIONS+2; ++i)
        {
            param = ((float)i/SHP_ELLIPSE_DIVISIONS)*M_PI*2.0f;

            data[2+2*i] = center.x+size.x*sin(param);
            data[2+2*i+1] = center.y+size.y*cos(param);
        }

        glPrimitive = GL_TRIANGLE_FAN;
        vCnt = SHP_ELLIPSE_DIVISIONS+2;
        break;

    case SHP_SUPERELLIPSE_N4:
        data.resize(5+(size_t(SHP_ELLIPSE_DIVISIONS)<<1));

        data[0] = center.x;
        data[1] = center.y;

        for(uint_fast32_t i = 0; i < SHP_ELLIPSE_DIVISIONS+2; ++i)
        {
            param = ((float)i/SHP_ELLIPSE_DIVISIONS)*M_PI*2.0f;

            data[2+2*i] = center.x+size.x*pow(evilBithack_fabs(cos(param)),0.5)*(param>M_PI/2&&param<M_PI+M_PI/2?-1:1);
            data[2+2*i+1] = center.y+size.y*pow(evilBithack_fabs(sin(param)),0.5)*(param>M_PI?-1:1);
        }

        glPrimitive = GL_TRIANGLE_FAN;
        vCnt = SHP_ELLIPSE_DIVISIONS+2;
        break;

    case SHP_LINE:
        data.resize(4);
        data[0] = center.x;
        data[1] = center.y;
        data[2] = size.x;
        data[3] = size.y;
        glPrimitive = GL_LINES;
        vCnt = 2;
        break;

    case SHP_SUPERELLIPSE_N16:
        data.resize(5+(size_t(SHP_ELLIPSE_DIVISIONS)<<1));

        data[0] = center.x;
        data[1] = center.y;

        for(uint_fast32_t i = 0; i < SHP_ELLIPSE_DIVISIONS+1; ++i)
        {
            param = ((float)i/SHP_ELLIPSE_DIVISIONS)*M_PI*2.0f;

            data[2+2*i] = center.x+size.x*pow(evilBithack_fabs(cos(param)),0.125)*(param>M_PI/2&&param<M_PI+M_PI/2?-1:1);
            data[2+2*i+1] = center.y+size.y*pow(evilBithack_fabs(sin(param)),0.125)*(param>M_PI?-1:1);
        }

        data[2+2*SHP_ELLIPSE_DIVISIONS] = center.x+size.x;
        data[2+2*SHP_ELLIPSE_DIVISIONS+1] = center.y;

        glPrimitive = GL_TRIANGLE_FAN;
        vCnt = SHP_ELLIPSE_DIVISIONS+2;
        break;
        break;

    case SHP_CUBIC_BEZIER:
        data.resize(size_t(SHP_BEZIER_DIVISIONS)<<1);
        for(uint_fast32_t i = 0; i < SHP_BEZIER_DIVISIONS; ++i)
        {
            param = (float)i/SHP_BEZIER_DIVISIONS;
            imcp1 = size*param + center*(1-param);
            imcp2 = cp2*param + size*(1-param);
            imcp3 = imcp2*param + imcp1*(1-param);
            data[i<<1] = imcp3.x;
            data[(i<<1)+1] = imcp3.y;
        }
        glPrimitive = GL_LINE_STRIP;
        vCnt = SHP_BEZIER_DIVISIONS;
        break;

    case SHP_QUADRATIC_BEZIER:
        data.resize(size_t(SHP_BEZIER_DIVISIONS)<<1);
        for(uint_fast32_t i = 0; i < SHP_BEZIER_DIVISIONS; ++i)
        {
            param = (float)i/SHP_BEZIER_DIVISIONS;

            imcp1 = size*param + center*(1-param);
            imcp2 = cp2*param + size*(1-param);
            imcp3 = cp3*param + cp2*(1-param);

            imcp4 = imcp2*param + imcp1*(1-param);
            imcp5 = imcp3*param + imcp2*(1-param);

            imcp6 = imcp5*param + imcp4*(1-param);

            data[i<<1] = imcp6.x;
            data[(i<<1)+1] = imcp6.y;
        }
        glPrimitive = GL_LINE_STRIP;
        vCnt = SHP_BEZIER_DIVISIONS;
        break;

    default:
        break;
    }

    this->flatPolygonShader.use();
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUniform3f(this->colorLoc, color.x, color.y, color.z);
    glDrawArrays(glPrimitive, 0, vCnt);
}
