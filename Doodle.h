#ifndef _DOODLE_H_
#define _DOODLE_H_

#include "SDL2/SDL.h"
#include "InputHandler.h"
#include "Window.h"

class Doodle
{
public:
    const char* name;
    InputHandler* input;
    Window* window;

    Doodle();
    ~Doodle();

    virtual bool Initialize(Window* window, InputHandler* input);

    virtual void InputRefresh()=0;

    virtual void Run()=0;
};

#endif