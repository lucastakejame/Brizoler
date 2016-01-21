#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL2/SDL.h>
#include <math.h>
#include "utils.h"

class Window
{
private:

    SDL_Texture* texture;

    SDL_Window* window;
    SDL_Renderer* renderer;

    int w;
    int h;
public:
    SDL_Surface* canvas;

    Window(int w, int h);
    ~Window();

    int GetW(){return w;}
    int GetH(){return h;}

    bool ResizeCanvas(int w, int h);

    bool DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b);

    void DrawLine(float x1, float y1, float x2, float y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue);

    void RefreshCanvas();


};

#endif