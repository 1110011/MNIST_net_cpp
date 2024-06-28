#include "oglInit.h"

// enables OpenGL for the window (hwnd) given the device context handle (hDC) and the rendering context handle (hRC)
void enableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    // allocate memory for pixel format descriptor
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    *hDC = GetDC(hwnd);
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;   // enable drawing to window, OpenGL and double-buffering
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;    // 24 bits per pixel in color buffer (8 bit red, 8 bit green, 8 bit blue)
    pfd.cDepthBits = 0;    // 0 bits per pixel in depth buffer
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(*hDC, &pfd);    // create pixel format from PFD
    SetPixelFormat(*hDC, iFormat, &pfd);        // set pixel format
    *hRC = wglCreateContext(*hDC);              // create rendering context
    wglMakeCurrent(*hDC, *hRC);                 // activate rendering context on window
}

// disables OpenGL, the device- and rendering context, and frees all associated memory
void disableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
