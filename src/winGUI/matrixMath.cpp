#include "matrixMath.h"

mat4::mat4(void)
{

}

mat4::mat4(const mat4& cpy)
{
    memcpy(&this->mat[0][0], &cpy.mat[0][0], sizeof(float)*4*4);
}

void mat4::loadIdentity(void)
{
    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t j = 0; j < 4; j++)
        {
            if(j == i)
                this->mat[i][j] = 1.0f;
            else this->mat[i][j] = 0.0f;
        }
    }
}

void mat4::print(void)
{
    printf("%+.2f, %+.2f, %+.2f, %+.2f\n",this->mat[0][0],this->mat[1][0],this->mat[2][0],this->mat[3][0]);
    printf("%+.2f, %+.2f, %+.2f, %+.2f\n",this->mat[0][1],this->mat[1][1],this->mat[2][1],this->mat[3][1]);
    printf("%+.2f, %+.2f, %+.2f, %+.2f\n",this->mat[0][2],this->mat[1][2],this->mat[2][2],this->mat[3][2]);
    printf("%+.2f, %+.2f, %+.2f, %+.2f\n",this->mat[0][3],this->mat[1][3],this->mat[2][3],this->mat[3][3]);
}

void mat4::printForCollision(void)
{
    for(uint_fast8_t x = 0; x < 4; ++x)
    {
        for(uint_fast8_t y = 0; y < 4; ++y)
            std::cout << this->mat[x][y] << '\n';
    }
}

const float* mat4::getGLMat(void)
{
    return (const float*)&this->mat;
}

mat4 mat4::operator+(const mat4& rhs)
{
    mat4 tmp = mat4();
    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t j = 0; j < 4; j++)
            tmp.mat[i][j] = this->mat[i][j] + rhs.mat[i][j];
    }
    return tmp;
}

mat4 mat4::operator-(const mat4& rhs)
{
    mat4 tmp = mat4();
    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t j = 0; j < 4; j++)
            tmp.mat[i][j] = this->mat[i][j] - rhs.mat[i][j];
    }
    return tmp;
}

mat4 mat4::operator*(const mat4& rhs)
{
    mat4 tmp = mat4();
    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t j = 0; j < 4; j++)
        {
            for(uint_fast8_t k = 0; k < 4; k++)
                tmp.mat[i][j] += this->mat[i][k] * rhs.mat[k][j];
        }
    }
    return tmp;
}

vec3 mat4::operator*(const vec3& rhs)
{
    vec3 tmp;
    tmp.x = rhs.x*this->mat[0][0] + rhs.y*this->mat[1][0] + rhs.z*this->mat[2][0] + this->mat[3][0];
    tmp.y = rhs.x*this->mat[0][1] + rhs.y*this->mat[1][1] + rhs.z*this->mat[2][1] + this->mat[3][1];
    tmp.z = rhs.x*this->mat[0][2] + rhs.y*this->mat[1][2] + rhs.z*this->mat[2][2] + this->mat[3][2];
    return tmp;
}

mat4 mat4::operator/(const vec3& rhs)
{
    mat4 tmp = *this;
    tmp.mat[0][0] /= rhs.x;
    tmp.mat[1][0] /= rhs.x;
    tmp.mat[2][0] /= rhs.x;

    tmp.mat[0][1] /= rhs.y;
    tmp.mat[1][1] /= rhs.y;
    tmp.mat[2][1] /= rhs.y;

    tmp.mat[0][2] /= rhs.z;
    tmp.mat[1][2] /= rhs.z;
    tmp.mat[2][2] /= rhs.z;

    return tmp;
}

vec3 mat4::getTranslation(void)
{
    return vec3(this->mat[3][0], this->mat[3][1],this->mat[3][2]);
}

vec3 mat4::getScale(void)
{
    vec3 scale;
    scale.x = vec3(this->mat[0][0],this->mat[1][0],this->mat[2][0]).mag();
    scale.y = vec3(this->mat[0][1],this->mat[1][1],this->mat[2][1]).mag();
    scale.z = vec3(this->mat[0][2],this->mat[1][2],this->mat[2][2]).mag();
    return scale;
}

mat4 mat4::getUntranslatedMatrix(void)
{
    mat4 tmp = *this;
    tmp.mat[3][0] = 0.0f;
    tmp.mat[3][1] = 0.0f;
    tmp.mat[3][2] = 0.0f;
    return tmp;
}

mat4 mat4::getRotationShearMatrix(void)
{
    mat4 tmp = this->getUntranslatedMatrix();
    vec3 scale = tmp.getScale();
    tmp = tmp / scale;
    return tmp;
}

float mat4::det(void)
{
    mat4 A = *(this);
    float c, r = 1.0f;
    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t k = i+1; k < 4; ++k)
        {
            c = A.mat[i][k] / A.mat[i][i];
            for(uint_fast8_t j = i; j < 4; ++j)
                A.mat[j][k] = A.mat[j][k] - c*A.mat[j][i];
        }
    }
    for(uint_fast8_t i = 0; i < 4; ++i)
        r *= A.mat[i][i];
    return r;
}

float mat4::det3x3(uint_fast8_t ni, uint_fast8_t nj)
{
    return this->mat[i3(0,ni)][i3(0,nj)]*this->mat[i3(1,ni)][i3(1,nj)]*this->mat[i3(2,ni)][i3(2,nj)]+   // a*e*i +
           this->mat[i3(0,ni)][i3(1,nj)]*this->mat[i3(1,ni)][i3(2,nj)]*this->mat[i3(2,ni)][i3(0,nj)]+   // b*f*g +
           this->mat[i3(0,ni)][i3(2,nj)]*this->mat[i3(1,ni)][i3(0,nj)]*this->mat[i3(2,ni)][i3(1,nj)]-   // c*d*h -
           this->mat[i3(0,ni)][i3(2,nj)]*this->mat[i3(1,ni)][i3(1,nj)]*this->mat[i3(2,ni)][i3(0,nj)]-   // c*e*g -
           this->mat[i3(0,ni)][i3(1,nj)]*this->mat[i3(1,ni)][i3(0,nj)]*this->mat[i3(2,ni)][i3(2,nj)]-   // b*d*i -
           this->mat[i3(0,ni)][i3(0,nj)]*this->mat[i3(1,ni)][i3(2,nj)]*this->mat[i3(2,ni)][i3(1,nj)];   // a*f*h
}

mat4 mat4::coFactor(void)
{
    mat4 tmp = mat4();
    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t j = 0; j < 4; j++)
            tmp.mat[i][j] = ((i+j)%2==0?1:-1) * this->det3x3(i,j);
    }
    return tmp;
}

mat4 mat4::transpose(void)
{
    mat4 tmp;
    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t j = 0; j < 4; j++)
            tmp.mat[i][j] = this->mat[j][i];
    }
    return tmp;
}

mat4 mat4::inverse(void)
{
    mat4 tmp = *(this);
    float d = 1.0f/this->det();
    if(std::isnan(d))
    {
        std::cerr << "Singular Matrix!\n";
        tmp.loadIdentity();
        return tmp;
    }

    tmp = tmp.coFactor();
    tmp = tmp.transpose();

    for(uint_fast8_t i = 0; i < 4; ++i)
    {
        for(uint_fast8_t j = 0; j < 4; j++)
            tmp.mat[i][j] *= d;
    }
    return tmp;
}

bool mat4::isIdentity(float tolerance)
{
    // a 3x3 matrix (a homogeneous n+1 matrix without translation is effectively a nxn; in this case 3x3 matrix)
    // can have up to 6 invariant points thus 6 independent vectors must be checked
    vec3 vec = vec3(1,1,1);
    vec = *this * vec;
    if(evilBithack_fabs((vec-vec3(1,1,1)).qmag()) > tolerance)
        return false;
    vec = vec3(-1,1,1);
    if(evilBithack_fabs((*this*vec-vec3(-1,1,1)).qmag()) > tolerance)
        return false;
    vec = vec3(1,0,0);
    if(evilBithack_fabs((*this*vec-vec3(1,0,0)).qmag()) > tolerance)
        return false;
    vec = vec3(0,1,0);
    if(evilBithack_fabs((*this*vec-vec3(0,1,0)).qmag()) > tolerance)
        return false;
    vec = vec3(0,0,1);
    if(evilBithack_fabs((*this*vec-vec3(0,0,1)).qmag()) > tolerance)
        return false;
    vec = vec3(0,-1,2);
    if(evilBithack_fabs((*this*vec-vec3(0,-1,2)).qmag()) > tolerance)
        return false;
    vec = vec3(-1,-2,1);
    if(evilBithack_fabs((*this*vec-vec3(-1,-2,1)).qmag()) > tolerance)
        return false;
    return true;
}

bool mat4::rotatesAxisAligned(float tolerance)
{
    uint_fast8_t count;
    for(unsigned int x = 0; x < 3; ++x)
    {
        count = 0;
        for(unsigned int y = 0; y < 3; ++y)
        {
            if(evilBithack_fabs(this->mat[x][y]) < tolerance)
                ++count;
        }
        if(count != 2)
            return false;
    }
    return true;
}

/// only works on pure rotation matrices
bool mat4::rotatesYAligned(float tolerance)
{
    if(this->mat[0][1] < tolerance)
    {
        if(evilBithack_fabs(1.0f-this->mat[1][1]) < tolerance)
        {
            if(this->mat[2][1] < tolerance)
                return true;
        }
    }
    return false;
}

bool mat4::isPureTranslation(float tolerance)
{
    mat4 tmp = this->getUntranslatedMatrix();
    return tmp.isIdentity(tolerance);

}

mat4 getIdentityMatrix(void)
{
    mat4 tmp = mat4();
    tmp.loadIdentity();
    return tmp;
}


mat4 getBlenderToInfernoConversionMatrix(void)
{
    mat4 tmp = mat4();
    tmp.mat[0][0] = 1.0f;
    tmp.mat[2][1] = 1.0f;
    tmp.mat[1][2] = 1.0f;
    tmp.mat[3][3] = 1.0f;
    return tmp;
}

mat4 getScalingMatrix(float vec[4])
{
    mat4 tmp = mat4();
    for(uint16_t i = 0; i < 4; ++i)
        tmp.mat[i][i] = vec[i];
    return tmp;
}
mat4 getScalingMatrix(vec3 vec)
{
    mat4 tmp = mat4();
    tmp.mat[0][0] = vec.x;
    tmp.mat[1][1] = vec.y;
    tmp.mat[2][2] = vec.z;
    tmp.mat[3][3] = 1.0f;
    return tmp;
}
mat4 getScalingMatrix(float s)
{
    mat4 tmp = mat4();
    tmp.mat[0][0] = s;
    tmp.mat[1][1] = s;
    tmp.mat[2][2] = s;
    tmp.mat[3][3] = 1.0f;
    return tmp;
}

mat4 getTranslationMatrix(float vec[4])
{
    mat4 tmp = mat4();
    tmp.loadIdentity();
    tmp.mat[3][0] = vec[0];
    tmp.mat[3][1] = vec[1];
    tmp.mat[3][2] = vec[2];
    return tmp;
}
mat4 getTranslationMatrix(vec3 vec)
{
    mat4 tmp = mat4();
    tmp.loadIdentity();
    tmp.mat[3][0] = vec.x;
    tmp.mat[3][1] = vec.y;
    tmp.mat[3][2] = vec.z;
    return tmp;
}

mat4 getRotationMatrix(float theta, vec3 r)
{
    float st = sin(theta);
    float ct = cos(theta);
    mat4 tmp = mat4();

    tmp.mat[0][0] = ct+(r.x*r.x)*(1-ct);
    tmp.mat[0][1] = r.x*r.y*(1-ct)-r.z*st;
    tmp.mat[0][2] = r.x*r.z*(1-ct)+r.y*st;
    tmp.mat[0][3] = 0.0f;

    tmp.mat[1][0] = r.y*r.x*(1-ct)+r.z*st;
    tmp.mat[1][1] = ct+(r.y*r.y)*(1-ct);
    tmp.mat[1][2] = r.y*r.z*(1-ct)-r.x*st;
    tmp.mat[1][3] = 0.0f;

    tmp.mat[2][0] = r.z*r.x*(1-ct)-r.y*st;
    tmp.mat[2][1] = r.z*r.y*(1-ct)+r.x*st;
    tmp.mat[2][2] = ct+(r.z*r.z)*(1-ct);
    tmp.mat[2][3] = 0.0f;

    tmp.mat[3][0] = 0.0f;
    tmp.mat[3][1] = 0.0f;
    tmp.mat[3][2] = 0.0f;
    tmp.mat[3][3] = 1.0f;

    return tmp;

}

mat4 getEulerRotationMatrixXZY(vec3 rotation)
{
    return getRotationMatrix(rotation.x, vec3(1,0,0)) * getRotationMatrix(rotation.y, vec3(0,1,0)) * getRotationMatrix(rotation.z, vec3(0,0,1));
}

mat4 getPerspectiveProjectionMatrix(float FOV, float aspect, float zNear, float zFar)
{
    mat4 tmp = mat4();
    float tanHalfFovy = tan(FOV/2);

    tmp.mat[0][0] = 1 / (aspect * tanHalfFovy);
    tmp.mat[1][1] = 1 / tanHalfFovy;
    tmp.mat[2][2] = -(zFar + zNear) / (zFar - zNear);
    tmp.mat[2][3] = -1.0f;
    tmp.mat[3][2] = -(2 * zFar * zNear) / (zFar - zNear);

    return tmp;
}

mat4 getOrthographicProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
    mat4 tmp = mat4();
    tmp.loadIdentity();

    tmp.mat[0][0] =  2.0f/(right-left);
    tmp.mat[1][1] =  2.0f/(top-bottom);
    tmp.mat[2][2] = -2.0f/(zFar-zNear);
    tmp.mat[3][0] = -(right+left)/(right-left);
    tmp.mat[3][1] = -(top+bottom)/(top-bottom);
    tmp.mat[3][2] = -(zFar+zNear)/(zFar-zNear);

    return tmp;
}

mat4 lookAt(vec3 eye, vec3 center, vec3 up)
{
    mat4 c = mat4();

    vec3 d = eye-center; d = d.norm();
    vec3 r = cross(up,d); r = r.norm();
    vec3 u = cross(d,r);

    c.mat[0][0] = r.x;
    c.mat[1][0] = r.y;
    c.mat[2][0] = r.z;
    c.mat[3][0] = 0.0f;

    c.mat[0][1] = u.x;
    c.mat[1][1] = u.y;
    c.mat[2][1] = u.z;
    c.mat[3][1] = 0.0f;

    c.mat[0][2] = d.x;
    c.mat[1][2] = d.y;
    c.mat[2][2] = d.z;
    c.mat[3][2] = 0.0f;

    c.mat[0][3] = 0.0f;
    c.mat[1][3] = 0.0f;
    c.mat[2][3] = 0.0f;
    c.mat[3][3] = 1.0f;

    mat4 e = mat4();
    e.loadIdentity();

    e.mat[3][0] = -eye.x;
    e.mat[3][1] = -eye.y;
    e.mat[3][2] = -eye.z;

	return e*c;

}

mat4 loadMatrixFromBlenderExport(std::vector<float> in)
{
    vec3 translation = vec3(in[0],in[1],in[2]);
    vec3 rotation = vec3(in[3],in[4],in[5]);
    vec3 scale = vec3(in[6],in[7],in[8]);

    return getScalingMatrix(scale) * getRotationMatrix(rotation.x, vec3(1,0,0)) * getRotationMatrix(rotation.z, vec3(0,0,1)) * getRotationMatrix(rotation.y, vec3(0,1,0)) * getTranslationMatrix(translation);
}
