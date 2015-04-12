#ifndef _BRIZOLER_H_
#define _BRIZOLER_H_

#include <SDL/SDL.h>
#include "CEvent.h"

class Brizoler: public CEvent
{
public:
    SDL_Surface* Surf;
    bool Running;

    int MouseX;
    int MouseY;

    Brizoler();
    ~Brizoler();

    bool OnInit();

    int OnExecute();

    void OnCleanup();

    void OnEvent(SDL_Event* event);

    void OnResize(int w,int h);

    void OnExit();

    void OnMouseFocus();

    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

    void OnLButtonDown(int mX, int mY);

    void OnLButtonUp(int mX, int mY);

    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

    void OnPaint(int OffsetX, int OffsetY);

    void PaintLine(int line, int start, int end);

    void PaintColumn(int column, int start, int end);

    void LinesGettingSmaller();

    void BlackScreen();
};

#endif