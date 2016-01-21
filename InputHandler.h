#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

#include <SDL2/SDL.h>
#include "CEvent.h"

class InputHandler: public CEvent
{
public:
    const Uint8* keystate;

    int mX; //mouse X
    int mY; //mouse Y
    int relX; // mouse relative move of X
    int relY; // mouse relative move of Y

    bool mL; // mouse left button
    bool mR; // mouse right button
    bool mM; // mouse middle button

    InputHandler();
    ~InputHandler();

    void OnEvent(SDL_Event* event);

    void OnMouseFocus();

    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

    void OnLButtonDown(int mX, int mY);

    void OnLButtonUp(int mX, int mY);

    void OnKeyDown(SDL_Keycode sym, Uint16 mod);

    void OnKeyUp(SDL_Keycode sym, Uint16 mod);


};

#endif