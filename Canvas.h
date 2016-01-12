#ifndef _CANVAS_
#define _CANVAS_

class Point
{
    float x;
    float y;

    Point():x(0.0), y(0.0){}

    Point(float x, float y):x(x), y(y){}

};

struct Canvas
{
    SDL_Surface* canvas;
};

#endif