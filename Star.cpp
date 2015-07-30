#include "stdio.h"
#include "Star.h"
#include "math.h"

enum{CANVAS_RED = 0, CANVAS_GREEN, CANVAS_BLUE};


Star::Star(uint canvas_width, uint canvas_height, uint* canvas):
canvas_width(canvas_width), canvas_height(canvas_height), canvas(canvas)
{

    canvas_limit = canvas_width*canvas_height;

    pos_x = canvas_width/2;
    pos_y = canvas_height/2;
    last_pos_x = pos_x;
    last_pos_y = pos_y;

    Reset_vel();

}

Star::~Star(){}

void Star::Fade(uint mouse_x, uint mouse_y)
{
    Canvas_Paint(last_pos_x-1, last_pos_y-1, 0, 0, 0);
    Canvas_Paint(last_pos_x, last_pos_y-1, 0, 0, 0);
    Canvas_Paint(last_pos_x+1, last_pos_y-1, 0, 0, 0);

    Canvas_Paint(last_pos_x-1, last_pos_y, 0, 0, 0);
    Canvas_Paint(last_pos_x, last_pos_y, 0, 0, 0);
    Canvas_Paint(last_pos_x+1, last_pos_y, 0, 0, 0);

    Canvas_Paint(last_pos_x-1, last_pos_y+1, 0, 0, 0);
    Canvas_Paint(last_pos_x, last_pos_y+1, 0, 0, 0);
    Canvas_Paint(last_pos_x+1, last_pos_y+1, 0, 0, 0);
}

void Star::Shine(uint mouse_x, uint mouse_y, Uint8 red, Uint8 green, Uint8 blue)
{

    Canvas_Paint(pos_x-1, pos_y-1, red, green, blue);
    Canvas_Paint(pos_x, pos_y-1, red, 255, 255);
    Canvas_Paint(pos_x+1, pos_y-1, red, green, blue);

    Canvas_Paint(pos_x-1, pos_y, 255, green, 255);
    // Canvas_Paint(pos_x, pos_y, red, green, blue);
    Canvas_Paint(pos_x, pos_y, 255, 255, 255);
    Canvas_Paint(pos_x+1, pos_y, 255, 255, blue);

    Canvas_Paint(pos_x-1, pos_y+1, red, green, blue);
    Canvas_Paint(pos_x, pos_y+1, 255, 255, 255);
    Canvas_Paint(pos_x+1, pos_y+1, red, green, blue);
}

void Star::Animate(uint mouse_x, uint mouse_y, uint offsetx, uint offsety)
{

    Shine(mouse_x, mouse_y, red, green, blue);
    // Fade(mouse_x, mouse_y);
    Move(mouse_x, mouse_y);

    // red += offsetx;
    // green += offsety;
    // blue += offsetx*offsety;

}

void Star::Move(uint mouse_x, uint mouse_y)
{

    last_pos_x = pos_x;
    last_pos_y = pos_y;
    pos_x += vel_x;
    pos_y += vel_y;

    if( pos_x < 0 || pos_x > canvas_width-1 || pos_y < 0 || pos_y > canvas_height-1 )
    {
        static int count = 0;

        pos_x = mouse_x;
        pos_y = mouse_y;
        Reset_vel();
    }
}

void Star::Reset_vel()
{
    vel_x = float(random()%30)/10;
    vel_y = float(random()%30)/10;
    // vel_x = float(random()%30)/20;
    // vel_y = float(random()%30)/20;

    dir_x = random()%2;
    dir_y = random()%2;

    if(!dir_x)
        vel_x *= -1;
    if(!dir_y)
        vel_y *= -1;
}

void Star::Set_Color(Uint8 red, Uint8 green, Uint8 blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void Star::Canvas_Paint(uint x, uint y, Uint8 red, Uint8 green, Uint8 blue)
{
    if(x > canvas_width || y > canvas_height || y*(canvas_width) + x >= canvas_limit)
    {
        return;
    }

    uint index = y*(canvas_width) + x;
    uint color = red << 16 | green << 8 | blue;

    canvas[index] = color;

    return;
}

void Star::Change_Direction()
{
    float old_vel_y = vel_y;
    vel_y = vel_x;
    vel_x = - old_vel_y;
}