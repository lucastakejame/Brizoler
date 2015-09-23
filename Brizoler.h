#ifndef _BRIZOLER_H_
#define _BRIZOLER_H_

#include <SDL/SDL.h>
#include "CEvent.h"

class Brizoler: public CEvent
{
public:
    SDL_Surface* surf;
    SDL_Surface* canvas_surf;
    uint* canvas;
    bool Running;

    int mouse_x;
    int mouse_y;

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

    void PaintRow(int y, int x1, int x2, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void PaintColumn(int x, int y1, int y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void PaintLine(float x1, float y1, float x2, float y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void LinesGettingSmaller(int num_lines);

    void InfiniteSquares(int num_squares);

    void BlackScreen();

    void BlurScreen();

    void Rect(int left_cornerX, int left_cornerY, int width, int height, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void Stars(uint num_stars);

    float CalculateFunc(float x);

    float CalculatePolarFunc(float angle);

    void Plot();

    void PolarPlot();

    bool IsInsideCanvas(float x, float y);

    bool PaintPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);

    bool fPaintPixel(float x, float y, Uint8 red, Uint8 green, Uint8 blue);

    Uint8 GetRed(int x, int y);

    Uint8 GetGreen(int x, int y);

    Uint8 GetBlue(int x, int y);


};

#endif