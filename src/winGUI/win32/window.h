#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <windows.h>
#include <string>

#include "../vectorMath.h"
#include "../matrixMath.h"
#include "../mathUtils.h"
#include "../ogl/gl45.h"
#include "../ogl/oglInit.h"
#include "win32Init.h"


class Window
{
private:
    WNDCLASSEX wcex;
    HDC hDC;
    HGLRC hRC;
    MSG msg;

    unsigned int msaaFBO; // multisample anti-aliasing framebuffer object
    unsigned int msaaTex; // multisample anti-aliasing texture

    unsigned int cWidth; // current window width
    unsigned int cHeight; // current window height

    bool state_OK = false;

    bool lmbLock = false;
    bool gMouseLock = false;

    vec3 clearColor = vec3();

    void createMSAABuf(void);

public:
    vec2 cursorNDC = vec2();
    HWND hwnd;

    Window() {}
    Window(const std::string, const int, const int);
    Window(const std::string, const std::string, const int, const int, const int, const int, const int);

    inline void setClearColor(const vec3 color) {this->clearColor = color;}
    inline void setLineWidth(const float width) {glLineWidth(width);}

    void updateCursorNDC(void);
    bool update(void);
    void prepareForRender(void);
    WPARAM destroy(void);

};

#endif // WINDOW_H_INCLUDED
