#ifndef GLSLSHADERCOMPILER_H_INCLUDED
#define GLSLSHADERCOMPILER_H_INCLUDED

#include <string>
#include <fstream>
#include "gl45.h"

#include <windows.h>

inline std::string resourceGetLine(unsigned int& fdRpOf, const HGLOBAL& fdRp, const size_t fdRSize)
{
    std::string sTmp = "";
    char cChar;

    do
    {
        cChar = *(reinterpret_cast<char*>(fdRp) + fdRpOf);
        fdRpOf++;
        if(cChar == '\n')
            break;
        if(cChar > 0 && cChar < 32)
            continue;
        sTmp += cChar;
    }while(fdRpOf < fdRSize);
    return sTmp;
}

class glslShader
{
private:
    unsigned int id;

public:
    glslShader() : id(0) {}

    glslShader(const std::string, const std::string);
    glslShader(const unsigned int, const unsigned int);

    inline unsigned int getID(void) {return this->id;}

    void use(void);
};

#endif // GLSLSHADERCOMPILER_H_INCLUDED
