#ifndef _STAR_H_
#define _STAR_H_

#include <SDL/SDL.h>
#include "CEvent.h"

struct Star
{
    uint pos_x, pos_y;

    uint *canvas;
    uint canvas_width;
    uint canvas_height;

    uint vel_x;
    char dir_x;
    uint vel_y;
    char dir_y;

    Uint8 red;
    Uint8 green;
    Uint8 blue;

    Star(uint canvas_width, uint canvas_height, uint* canvas);
    ~Star();


    void Shadow(uint mouse_x, uint mouse_y);
    void Shine(uint mouse_x, uint mouse_y, Uint8 red, Uint8 green, Uint8 blue);
    void Animate(uint mouse_x, uint mouse_y, uint offsetx, uint offsety);
    void Move(uint mouse_x, uint mouse_y);


};

#endif