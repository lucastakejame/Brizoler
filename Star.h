#ifndef _STAR_H_
#define _STAR_H_

#include <SDL/SDL.h>
#include "CEvent.h"

struct Star
{
    float pos_x;
    float pos_y;

    float last_pos_x;
    float last_pos_y;

    uint *canvas;
    uint canvas_width;
    uint canvas_height; // Canvas considering only (x,y) position, no pixel size
    uint canvas_limit;

    float vel_x;
    float vel_y;
    char dir_x;
    char dir_y;

    Uint8 red;
    Uint8 green;
    Uint8 blue;

    Star(uint canvas_width, uint canvas_height, uint* canvas);
    ~Star();


    void Fade(uint mouse_x, uint mouse_y);
    void Shine(uint mouse_x, uint mouse_y, Uint8 red, Uint8 green, Uint8 blue);
    void Animate(uint mouse_x, uint mouse_y, uint offsetx, uint offsety);
    void Move(uint mouse_x, uint mouse_y);

    void Reset_vel();
    void Set_Color(Uint8 red, Uint8 green, Uint8 blue);

    void Canvas_Paint(uint x, uint y, Uint8 red, Uint8 green, Uint8 blue);

};

#endif