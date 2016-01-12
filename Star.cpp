#include "Star.h"

enum{CANVAS_RED = 0, CANVAS_GREEN, CANVAS_BLUE};


Star::Star(uint canvas_width, uint canvas_height, uint* canvas):
canvas_width(canvas_width), canvas_height(canvas_height), canvas(canvas)
{

    canvas_limit = canvas_width*canvas_height;

    pos_x = canvas_width/2;
    pos_y = canvas_height/2;
    last_pos_x = pos_x;
    last_pos_y = pos_y;

    accel_x = 0;
    accel_y = 0;

    Reset_vel();

    size = random()%10 + 1;

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

    if(1)
    // if(size == 1)
    {
        float dx = (pos_x - last_pos_x);
        float dy = (pos_y - last_pos_y);

        if( isnormal(dx) && isnormal(dy))
        {
            float module = sqrt(dx*dx+dy*dy);
            dx /= module;
            dy /= module;

        }
        else if(dx == 0 && dy != 0)
        {
            dy /= fabs(dy);
        }
        else if(dx != 0 && dy == 0)
        {
            dx /= fabs(dx);
        }

        if(pos_x != last_pos_x && pos_y != last_pos_y)
        {
            int count = 0;
            for (float i = last_pos_x, j = last_pos_y;
            (dx != 0 && dy != 0)
            && (((pos_x - i)*dx >= 0)
            && ((pos_y - j)*dy >= 0))
            // && count < 100
            ;
            i += dx, j += dy, ++count)
            {
                Canvas_Paint(i-1, j-1, red, green, blue);
                Canvas_Paint(i, j-1, red, 255, 255);
                Canvas_Paint(i+1, j-1, red, green, blue);

                Canvas_Paint(i-1, j, 255, green, 255);
                // Canvas_Paint(i, j, red, green, blue);
                Canvas_Paint(i, j, 255, 255, 255);
                Canvas_Paint(i+1, j, 255, 255, blue);

                Canvas_Paint(i-1, j+1, red, green, blue);
                Canvas_Paint(i, j+1, 255, 255, 255);
                Canvas_Paint(i+1, j+1, red, green, blue);
            }
        }
        else
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

    }
    else
    {
        int draw_size = size;
        // int draw_size = size*2;
        for (int j = 0; j < draw_size; ++j)
        {
            for (int i = 0; i < draw_size; ++i)
            {
                if((i == j)
                || (i == draw_size-j))
                {
                    Canvas_Paint(pos_x - (draw_size/2) + i, pos_y - (draw_size/2) + j, 255, 255, 255);
                }
                else if(((i < draw_size/2) && (j < draw_size/2))
                || ((i > draw_size/2) && (j > draw_size/2)))
                {
                    Canvas_Paint(pos_x - (draw_size/2) + i, pos_y - (draw_size/2) + j, red / (0.3*fabs(i-j)), green / (0.3*fabs(i-j)), blue / (0.3*fabs(i-j)));
                }
                else
                {
                    Canvas_Paint(pos_x - (draw_size/2) + i, pos_y - (draw_size/2) + j, red / (0.3*fabs(i+j-draw_size)), green / (0.3*fabs(i+j-draw_size)), blue / (0.3*fabs(i+j-draw_size)));
                }
            }
        }
    }
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

    bool bounce = true;
    float dumping = 0.59;

    vel_x += accel_x*dumping;
    vel_y += accel_y*dumping;

    if((isnormal(pos_x) || pos_x == 0)
    && (isnormal(pos_y) || pos_y == 0))
    {
        last_pos_x = pos_x;
        last_pos_y = pos_y;
        pos_x += vel_x;
        pos_y += vel_y;
    }
    else
    {
        fprintf(stderr, "NAN MEU\n");
        fprintf(stderr, "last_pos:\t(%2.2f, %2.2f)\n", last_pos_x, last_pos_y);
        fprintf(stderr, "pos:\t(%2.2f, %2.2f)\n", pos_x, pos_y);
        fprintf(stderr, "vel:\t(%2.2f, %2.2f)\n", vel_x, vel_y);
        fprintf(stderr, "accel:\t(%2.2f, %2.2f)\n", accel_x, accel_y);

        cin.get();
    }

    float unseen_canvas_x = canvas_width;
    float unseen_canvas_y = canvas_height;
    float x_limit0 = 0 - unseen_canvas_x;
    float x_limit1 = canvas_width-1 + unseen_canvas_x;
    float y_limit0 = 0 - unseen_canvas_y;
    float y_limit1 = canvas_height-1 + unseen_canvas_y;

    if(bounce)
    {
        if(pos_x < x_limit0)
            vel_x = fabs(vel_x)*dumping;
        else if(pos_x > x_limit1)
            vel_x = -fabs(vel_x)*dumping;

        if(pos_y < y_limit0)
            vel_y = fabs(vel_y)*dumping;
        else if(pos_y > y_limit1)
            vel_y = -fabs(vel_y)*dumping;
    }
    else if( (pos_x < x_limit0)
    || (pos_x > x_limit1)
    || (pos_y < y_limit0)
    || (pos_y > y_limit1) )
    {
        static int wait_last_shine = 0;
        wait_last_shine++;

        // if(0)
        if(wait_last_shine == 30)
        {
            if((0 <= mouse_x)
                && (mouse_x < canvas_width)
                && (0 <= mouse_y)
                && (mouse_y < canvas_height))
            {
                last_pos_x = pos_x = (int)mouse_x;
                last_pos_y = pos_y = (int)mouse_y;
            }
            else
            {
                pos_x = canvas_width/2;
                pos_y = canvas_height/2;
            }
            Reset_vel();

            wait_last_shine = 0;
        }
    }

}

void Star::Reset_vel()
{


    accel_x = 0;
    accel_y = 0;

    vel_x = float(random()%30)/10;
    vel_y = float(random()%30)/10;
    // vel_x = float(random()%30)/20;
    // vel_y = float(random()%30)/20;

    int dir_x = random()%2;
    int dir_y = random()%2;

    if(!dir_x)
        vel_x *= -1;
    if(!dir_y)
        vel_y *= -1;


}

void Star::Multiply_vel(float factor)
{
    vel_x *= factor;
    vel_y *= factor;
}


void Star::Set_Color(Uint8 red, Uint8 green, Uint8 blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void Star::Canvas_Paint(uint x, uint y, Uint8 red, Uint8 green, Uint8 blue)
{
    if((x > canvas_width)
    || (y > canvas_height)
    || (y*(canvas_width) + x >= canvas_limit)
    )
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

void Star::Change_Acceleration(float a_x, float a_y)
{
    accel_x = a_x;
    accel_y = a_y;
}

void Star::Add_Acceleration(float a_x, float a_y)
{
    accel_x += a_x;
    accel_y += a_y;
}
