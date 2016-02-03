#ifndef _BRIZOLER_H_
#define _BRIZOLER_H_

#include <SDL2/SDL.h>
#include "Window.h"
#include "CEvent.h"

#include "Star.h"
#include "utils.h"
#include "InputHandler.h"
#include "Doodle.h"
#include "Pattern.h"
#include "Plotter.h"

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 800

class Brizoler: public CEvent
{
public:
    SDL_Surface* surf;
    SDL_Surface* canvas_surf;
    SDL_Texture* texture;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Window* wi;
    InputHandler* input;

    uint* canvas;
    bool Running;

    int mouse_x;
    int mouse_y;
    int mouse_rel_x;
    int mouse_rel_y;
    bool mouse_l;
    bool mouse_r;

    Brizoler();
    ~Brizoler();

    bool OnInit();

    int OnExecute();

    void OnPaint(int OffsetX, int OffsetY);

    void OnCleanup();

    void OnExit();


    void OnResize(int w,int h);

    void OnEvent(SDL_Event* event);

    void OnMouseFocus();

    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

    void OnLButtonDown(int mX, int mY);

    void OnLButtonUp(int mX, int mY);

    void OnKeyDown(SDL_Keycode sym, Uint16 mod);

    void OnKeyUp(SDL_Keycode sym, Uint16 mod);

    void InputRefresh();


    bool PaintPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);

    bool PaintPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue, SDL_Surface* other_surf);

    void PaintRow(int y, int x1, int x2, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void PaintColumn(int x, int y1, int y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void PaintLine(float x1, float y1, float x2, float y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    bool IsInsideCanvas(float x, float y);

    void BlackScreen();

    void BlurScreen();


    void LinesGettingSmaller(int num_lines);

    void InfiniteSquares(int num_squares);

    void Rect(int left_cornerX, int left_cornerY, int width, int height, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void Stars(uint num_stars);

    float CalculateFunc(float x);

    float CalculatePolarFunc(float angle);

    void Plot();

    void PolarPlot();
};

#endif