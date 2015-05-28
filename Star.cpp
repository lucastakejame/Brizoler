#include "stdio.h"
#include "Star.h"
#include "math.h"

enum{CANVAS_RED = 0, CANVAS_GREEN, CANVAS_BLUE};


Star::Star(uint canvas_width, uint canvas_height, uint* canvas):canvas_width(canvas_width), canvas_height(canvas_height), canvas(canvas)
{

    pos_x = canvas_width/2;
    pos_y = canvas_height/2;

    // vel_x = 20;
    // vel_y = 20;
    vel_x = random()%10;
    vel_y = random()%10;

    dir_x = random()%2;
    dir_y = random()%2;

}

Star::~Star(){}

void Star::Shadow(uint mouse_x, uint mouse_y)
{

    uint index_red = pos_y*(canvas_width)+pos_x*(canvas_height)+CANVAS_RED;
    uint index_green = pos_y*(canvas_width)+pos_x*(canvas_height)+CANVAS_GREEN;
    uint index_blue = pos_y*(canvas_width)+pos_x*(canvas_height)+CANVAS_BLUE;

    uint limit = this->canvas_width*this->canvas_height*3;

    if(index_red > limit)
    {
        index_red = (index_red%limit + (mouse_y)*this->canvas_width + mouse_y)%limit;
    }
    if(index_green > limit)
    {
        index_green = (index_green%limit + (mouse_y)*this->canvas_width + mouse_y)%limit;
    }
    if(index_blue > limit)
    {
        index_blue = (index_blue%limit + (mouse_y)*this->canvas_width + mouse_y)%limit;
    }

    canvas[index_red] = 0;
    canvas[index_green] = 0;
    canvas[index_blue] = 0;
}

void Star::Shine(uint mouse_x, uint mouse_y, Uint8 red, Uint8 green, Uint8 blue)
{

    uint index_red = pos_y*(canvas_width)+pos_x*(canvas_height)+CANVAS_RED;
    uint index_green = pos_y*(canvas_width)+pos_x*(canvas_height)+CANVAS_GREEN;
    uint index_blue = pos_y*(canvas_width)+pos_x*(canvas_height)+CANVAS_BLUE;

    uint limit = this->canvas_width*this->canvas_height*3;

    if(index_red > limit)
    {
        index_red = (index_red%limit + mouse_y*this->canvas_width + mouse_x);
    }
    if(index_green > limit)
    {
        index_green = (index_green%limit + mouse_y*this->canvas_width + mouse_x);
    }
    if(index_blue > limit)
    {
        index_blue = (index_blue%limit + mouse_y*this->canvas_width + mouse_x);
    }


    canvas[index_red] = red;
    canvas[index_green] = green;
    canvas[index_blue] = blue;

    this->red = red;
    this->green = green;
    this->blue = blue;

}

void Star::Animate(uint mouse_x, uint mouse_y, uint offsetx, uint offsety)
{
    static Uint8 red = 100;
    static Uint8 green = 200;
    static Uint8 blue = 180;


    Shadow(mouse_x, mouse_y);
    Shine(mouse_x, mouse_y, red, green, blue);
    Move(mouse_x, mouse_y);

    // red += offsetx;
    // green += offsety;
    // blue += offsetx*offsety;

}

void Star::Move(uint mouse_x, uint mouse_y)
{

    uint limit = canvas_width*canvas_height;

    if(dir_x)
        pos_x = pos_x + vel_x;
        if(pos_x > canvas_width)
            pos_x = mouse_x;
    else
        pos_x = pos_x - vel_x;
        if(pos_x < 0)
            pos_x = mouse_x;

    if(dir_y)
        pos_y = pos_y + vel_y;
        if(pos_y > canvas_height)
            pos_y = mouse_y;
    else
        pos_y = pos_y - vel_y;
        if(pos_y < 0)
            pos_y = mouse_y;
}