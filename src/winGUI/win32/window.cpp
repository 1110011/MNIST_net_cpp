#include "window.h"

void Window::createMSAABuf(void)
{
    // 4xMMAA
    glGenFramebuffers(1, &this->msaaFBO); // create frame-buffer for multi-sample rendering
    glBindFramebuffer(GL_FRAMEBUFFER, this->msaaFBO);

    // create multi-sample texture
    glGenTextures(1, &this->msaaTex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->msaaTex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, cWidth, cHeight, GL_TRUE);

    // set texture as color attachment 0 for frame-buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, this->msaaTex, 0);
}

Window::Window(const std::string name, const int width, const int height)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    this->cWidth = width;
    this->cHeight = height;

    // create window class
    this->wcex.cbSize = sizeof(WNDCLASSEX);
    this->wcex.style = CS_OWNDC;
    this->wcex.lpfnWndProc = windowProc;
    this->wcex.cbClsExtra = 0;
    this->wcex.cbWndExtra = 0;
    this->wcex.hInstance = hInstance;
    this->wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // set application icon (may also be custom. more info at https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadicona)
    this->wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // set cursor displayed in window (standard arrow, hand etc. more info at https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadcursora)
    this->wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    this->wcex.lpszMenuName = NULL;
    this->wcex.lpszClassName = "WINGUICLASS";     // class name defined here must be the same as the one used during window creation
    this->wcex.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(2), IMAGE_ICON, 0,0, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE)); // see wcex.hIcon = [...] for more info

    // register window class with OS
    if (!RegisterClassEx(&this->wcex))
    {
        this->state_OK = false;
        #ifdef DDEBUG
            std::cerr << "Failed to register window class with OS\n";
        #endif // DDEBUG
        return;
    }

    // create window
    this->hwnd = CreateWindowEx(0,"WINGUICLASS",name.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,width,height,NULL,NULL,hInstance,NULL);

    // initialize window
    ShowWindow(this->hwnd, SW_SHOWDEFAULT);
    enableOpenGL(this->hwnd, &this->hDC, &this->hRC);

    this->createMSAABuf();

    this->state_OK = true;
}

Window::Window(const std::string className, const std::string name,const int x, const int y, const int width, const int height, const int iconResource)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // create window class
    this->wcex.cbSize = sizeof(WNDCLASSEX);
    this->wcex.style = CS_OWNDC;
    this->wcex.lpfnWndProc = windowProc;
    this->wcex.cbClsExtra = 0;
    this->wcex.cbWndExtra = 0;
    this->wcex.hInstance = hInstance;
    this->wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // set application icon (may also be custom. more info at https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadicona)
    this->wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // set cursor displayed in window (standard arrow, hand etc. more info at https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadcursora)
    this->wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    this->wcex.lpszMenuName = NULL;
    this->wcex.lpszClassName = className.c_str();     // class name defined here must be the same as the one used during window creation
    this->wcex.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(iconResource), IMAGE_ICON, 0,0, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE)); // see wcex.hIcon = [...] for more info

    // register window class with OS
    if (!RegisterClassEx(&this->wcex))
    {
        this->state_OK = false;
        #ifdef DEBUG
            std::cerr << "Failed to register window class with OS\n";
        #endif // DDEBUG
        return;
    }

    // create window
    this->hwnd = CreateWindowEx(0,className.c_str(),name.c_str(),WS_OVERLAPPEDWINDOW,x,y,width,height,NULL,NULL,hInstance, NULL);

    // initialize window
    ShowWindow(this->hwnd, SW_SHOWDEFAULT);
    enableOpenGL(this->hwnd, &this->hDC, &this->hRC);

    this->state_OK = true;
}

void Window::updateCursorNDC(void)
{
    POINT cPos;
    GetCursorPos(&cPos); // Get cursor position in screen coordinates
    ScreenToClient(hwnd,&cPos); // convert cursor position to be relative to window

    // get window metrics
    RECT windowClientRect;
    GetClientRect(hwnd, &windowClientRect);

    this->cursorNDC = vec2(((float)cPos.x/windowClientRect.right)*2-1,-((float)cPos.y/windowClientRect.bottom)*2+1);
}

bool Window::update(void)
{
    if(!this->state_OK) return false;

    // get size of active drawing area (windowClientRect)
    RECT windowClientRect;
    GetClientRect(hwnd, &windowClientRect);

    unsigned int w,h;

    w = windowClientRect.right-windowClientRect.left;
    h = windowClientRect.bottom-windowClientRect.top;

    if(w != this->cWidth || h != this->cHeight) // resize event
    {
        this->cWidth = w;
        this->cHeight = h;

        // destroy MSAA-Buffer
        glDeleteTextures(1, &msaaTex);
        glDeleteFramebuffers(1, &msaaFBO);

        //re-create MSAA-Buffer
        this->createMSAABuf();
    }

    glViewport(0,0, w,h);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO);    // set multi-sample frame-buffer as source for copying
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);          // set default (visible) frame-buffer as copy destination
    // resolve multi-sample buffer to single samples using linear interpolation, and copy the data
    glBlitFramebuffer(0, 0, this->cWidth, this->cHeight, 0, 0, this->cWidth, this->cHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    SwapBuffers(hDC);

    // disable mouse events if window is not in focus
    if(GetActiveWindow() != this->hwnd)
        this->gMouseLock = true;
    else this->gMouseLock = false;

    this->updateCursorNDC();

    // Windows message handling
    if(PeekMessage(&this->msg, NULL, 0, 0, PM_REMOVE))
    {
        if(this->msg.message == WM_QUIT)
            return false;
        TranslateMessage(&this->msg);
        DispatchMessage(&this->msg);
    }

    return true;
}

void Window::prepareForRender(void)
{
    // bind multi-sample frame-buffer for rendering
    glBindFramebuffer(GL_FRAMEBUFFER, this->msaaFBO);

    glClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

WPARAM Window::destroy(void)
{
    // disable window
    disableOpenGL(this->hwnd, this->hDC, this->hRC);
    DestroyWindow(this->hwnd);
    return this->msg.wParam;
}
