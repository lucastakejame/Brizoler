#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <ctime>
#include "Brizoler.h"
#include "StarPaint.h"
#include "utils.h"
// #include "sin.h"
/*
void rotateVector(float& x, float& y, float rad)
{
    float temp_x = x;
    float temp_y = y;
    float cos_rad = cos(rad);
    float sin_rad = sin(rad);

    x = temp_x*cos_rad - temp_y*sin_rad;
    y = temp_x*sin_rad - temp_y*cos_rad;
}
void normalizeVector(float& x, float& y)
{
    if( isnormal(x) && isnormal(y))
    {
        float module = sqrt(x*x+y*y);
        x /= module;
        y /= module;

    }
    else if(x == 0 && y != 0)
    {
        y /= fabs(y);
    }
    else if(y == 0 && x != 0)
    {
        x /= fabs(x);
    }
}
*/
// struct Point
// {
//     float x;
//     float y;

//     Point():x(0.0), y(0.0){}
//     Point(float x, float y):x(x), y(y){}

// };

static int zoom_limit = 20;
static int speed_limit = 20;

static int stars_change_direction_flag = 0;

float param_zoom = 1;
float param_speed = 1;
float param_coord_x = 0;
float param_coord_y = 0;
int mode = 1;

bool change_speed_up;
bool change_speed_down;

bool gravitate_mouse = false;

#define MAX_HEIGHT 1080
#define MAX_WIDTH 1920
float array_prod[MAX_HEIGHT][MAX_WIDTH];

Doodle* doodle;
Doodle* doodle2;

Brizoler::Brizoler()
{
    this->Running = true;
}

Brizoler::~Brizoler()
{
}

int Brizoler::OnExecute()
{

    // printf("Insira um limite de zoom e velocidade: ");
    // scanf("%i %i", &zoom_limit, &speed_limit);

    if(OnInit() == false)
    {

        return -1;
    }

    SDL_Event Event;

    int offsetx = 30;
    int offsety = 0;

    while(Running)
    {
        static float time_now = clock();
        static float time_last = clock();
        float delta_frame = time_now - time_last;
        time_last = time_now;


        while(SDL_PollEvent(&Event))
        {
            OnEvent(&Event);
        }

        InputRefresh();

        OnPaint(offsetx, offsety);

        offsetx++;
        offsety++;

        // fprintf(stderr, "delta_frame:%2.5f \n", (float)delta_frame/CLOCKS_PER_SEC);

        time_now = clock();

    }

    OnCleanup();

    return 0;
}

bool Brizoler::OnInit()
{

    // log_fd = fopen( "mylogfile.txt", "w" );

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    if((this->surf = SDL_CreateRGBSurface(0,
                                    SCREEN_WIDTH, /*width , 0 means any width*/
                                    SCREEN_HEIGHT, /*height, 0 means any height*/
                                    32, /*bites per pixel, 0 means use current*/
                                    0x00FF0000,
                                    0x0000FF00,
                                    0x000000FF,
                                    0xFF000000)
        ) == NULL ||
        (this->canvas_surf = SDL_CreateRGBSurface(0,
                                    SCREEN_WIDTH, /*width , 0 means any width*/
                                    SCREEN_HEIGHT, /*height, 0 means any height*/
                                    32, /*bites per pixel, 0 means use current*/
                                    0x00FF0000,
                                    0x0000FF00,
                                    0x000000FF,
                                    0xFF000000)
        ) == NULL)
    {
        return false;
    }

    this->wi = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);

    this->input = new InputHandler();

    doodle = new Plotter();
    doodle2 = new StarPaint();
    // doodle = new Pattern();

    doodle->Initialize((Window*)this->wi, (InputHandler*)this->input);
    doodle2->Initialize((Window*)this->wi, (InputHandler*)this->input);

    // SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEnyy_HEIGHT, SDL_WINDOW_RESIZABLE, &(this->window), &(this->renderer));

    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.

    // SDL_RenderSetLogicalSize(this->renderer, SCREEN_WIDTH, SCREEnyy_HEIGHT);

    // this->texture = SDL_CreateTexture(this->renderer,
    //                            SDL_PIXELFORMAT_ARGB8888,
    //                            SDL_TEXTUREACCESS_STREAMING,
    //                            SCREEN_WIDTH, SCREEnyy_HEIGHT);



    uint canvas_size = this->surf->h*this->surf->w*3;

    canvas = new uint[canvas_size];
    if(!canvas)
    {
        return false;
    }

    // SDL_EnableKeyRepeat(1, 10);

    // SDL_ShowCursor(SDL_DISABLE);

    for (int y = 0; y < MAX_HEIGHT; ++y)
    {
        for (int x = 0; x < MAX_WIDTH; ++x)
        {
            int new_y = (y - MAX_HEIGHT/2)%MAX_HEIGHT;
            int new_x = (x - MAX_WIDTH/2)%MAX_WIDTH;
            array_prod[y][x] =
            (
            // (new_y*new_y + new_x*new_x)
            sin((float)new_y*2*M_PI/MAX_HEIGHT)
                *
            cos((float)new_x*2*M_PI/MAX_WIDTH)
            )
            ;
        }
    }

    return true;
}

void Brizoler::OnCleanup()
{
    free(canvas);
    SDL_FreeSurface(surf);
    SDL_FreeSurface(canvas_surf);
    SDL_Quit();
}


void Brizoler::OnEvent(SDL_Event* event)
{
    this->input->OnEvent(event);
}


void Brizoler::InputRefresh()
{
    if(input->keystate[SDL_SCANCODE_ESCAPE]){OnExit(); }

    if(input->keystate[SDL_SCANCODE_1]){ mode = 1;}
    // else if(input->keystate[SDL_SCANCODE_2]){ mode = 2;}
    // else if(input->keystate[SDL_SCANCODE_3]){ mode = 3;}
    else if(input->keystate[SDL_SCANCODE_4]){ mode = 4;}
    else if(input->keystate[SDL_SCANCODE_5]){ mode = 5;}
    else if(input->keystate[SDL_SCANCODE_6]){ mode = 6;}
    else if(input->keystate[SDL_SCANCODE_7]){ mode = 7;}
    else if(input->keystate[SDL_SCANCODE_8]){ mode = 8;}
    else if(input->keystate[SDL_SCANCODE_9]){ mode = 9;}

}

void Brizoler::OnMouseFocus()
{

}

void Brizoler::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{

    this->mouse_x = mX;
    this->mouse_y = mY;
    this->mouse_rel_x = relX;
    this->mouse_rel_y = relY;

    if(Left)
    {
        param_zoom = ((float)mY/surf->h)*zoom_limit + 0.01;
        param_speed = ((float)mX/surf->w)*speed_limit + 0.01;
    }
    if(Right)
    {
        param_coord_x = mX;
        param_coord_y = mY;
    }
}

void Brizoler::OnLButtonDown(int mX, int mY)
{
    this->mouse_x = mX;
    this->mouse_y = mY;

    this->mouse_l = true;

    param_zoom = ((float)mY/surf->h)*zoom_limit + 0.01;
    param_speed = ((float)mX/surf->w)*speed_limit + 0.01;

    gravitate_mouse = true;
}

void Brizoler::OnLButtonUp(int mX, int mY)
{
    this->mouse_l = false;
    gravitate_mouse = false;
}

void Brizoler::OnKeyDown(SDL_Keycode sym, Uint16 mod)
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    float delta = 0.1;

    switch(sym)
    {
        case SDLK_UP:
        {
            if(mod & KMOD_CTRL)
            {
                if(param_zoom + delta < zoom_limit)
                {
                    param_zoom += delta;
                }
            }
            else
            {
                if(param_zoom < zoom_limit)
                {
                    param_zoom += 0.005;
                }
            }
            if(mode == 6)
            {
                change_speed_up = true;
            }
        }
        break;

        case SDLK_DOWN:
        {
            if(mod & KMOD_CTRL)
            {
                if(param_zoom - delta > 0)
                {
                    param_zoom -= delta;
                }
            }
            else
            {
                if(param_zoom > 0.015)
                {
                    param_zoom -= 0.005;
                }
            }
            if(mode == 6)
            {
                change_speed_down = true;
            }
        }
        break;

        case SDLK_RIGHT:
        {
            if(mod & KMOD_CTRL)
            {
                if(param_speed + delta < speed_limit)
                {
                    param_speed += delta;
                }
            }
            else
            {
                if(param_speed < speed_limit)
                {
                    param_speed += 0.01;
                }
            }
        }
        break;

        case SDLK_LEFT:
        {
            if(mod & KMOD_CTRL)
            {
                if(param_speed - delta > 0)
                {
                    param_speed -= delta;
                }
            }
            else
            {
                if(param_speed > 0.015)
                {
                    param_speed -= 0.01;
                }
            }
        }
        break;

        default:
        break;
    }
}

void Brizoler::OnKeyUp(SDL_Keycode sym, Uint16 mod)
{
}

void Brizoler::OnResize(int w,int h)
{

    // surf = SDL_SetVideoMode(w, h, 0, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    // SDL_RenderSetLogicalSize(this->renderer, w, h);



    if(NULL == surf)
    {
        fprintf(stderr, "Unable to resize video mode: %s\n", SDL_GetError());
        exit(2);
    }
}

void Brizoler::OnExit()
{
    Running = false;
}

bool Brizoler::PaintPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
{
    return this->wi->DrawPixel(x, y, red, green, blue);
}

bool Brizoler::PaintPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue, SDL_Surface* other_surf)
{
    bool result = false;
    Uint32 *pixel = (Uint32*)other_surf->pixels;

    uint index;

    if(x >= 0 && x < other_surf->w && y >= 0 && y < other_surf->h)
    {
        index = y*other_surf->w + x;
        pixel[index] = SDL_MapRGB(other_surf->format, red, green, blue);
        result = true;
    }

    return result;
}

void Brizoler::PaintRow(int y, int x1, int x2, uint thickness, Uint8 red, Uint8 green, Uint8 blue)
{
    Uint32 *pixel = (Uint32*)surf->pixels;

    for(int y_counter = y; y_counter < y+thickness; y_counter++)
    {
        for (int x_counter = 0; x_counter < x2-x1; ++x_counter)
        {
            this->wi->DrawPixel(x_counter+x1, y_counter-thickness/2, red, green, blue);
        }
    }
}

void Brizoler::PaintColumn(int x, int y1, int y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue)
{
    Uint32 *pixel = (Uint32*)surf->pixels;

    for(int x_counter = x; x_counter < x+thickness; x_counter++)
    {
        for (int y_counter = 0; y_counter < y2-y1; ++y_counter)
        {
            this->wi->DrawPixel(x_counter-thickness/2, y_counter+y1, red, green, blue);
        }
    }
}


bool Brizoler::IsInsideCanvas(float x, float y)
{
    if((int)x >= 0 && (int)x < wi->GetW() && (int)y >= 0 && (int)y < wi->GetH())
    // if((int)x >= 0 && (int)x < surf->w && (int)y >= 0 && (int)y < surf->h)
    {
        return true;
    }
    return false;
}

void Brizoler::PaintLine(float x1, float y1, float x2, float y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue)
{
    this->wi->DrawLine(x1, y1, x2, y2, thickness, red, green, blue);
    return;
    // log_count ++;

    // if(log_count > 4)
    // {
    //     rewind(log_fd);
    // }

    KILL_DENORMAL_BY_QUANTIZATION(x1);
    KILL_DENORMAL_BY_QUANTIZATION(y1);
    KILL_DENORMAL_BY_QUANTIZATION(x2);
    KILL_DENORMAL_BY_QUANTIZATION(y2);

    if(fabs(x1) > 1e+7)
    {
        x1 = 1e+6;
    }
    if(fabs(y1) > 1e+7)
    {
        y1 = 1e+6;
    }
    if(fabs(x2) > 1e+7)
    {
        x2 = 1e+6;
    }
    if(fabs(y2) > 1e+7)
    {
        y2 = 1e+6;
    }

    float x,y,dx,dy;
    int xi,yi;

    dx = (x2-x1);
    dy = (y2-y1);

    if(fabsf(dx) > fabsf(dy))
    {
        dy = dy/(fabsf(dx));
        dx = dx/(fabsf(dx));
    }
    else
    {
        dx = dx/(fabsf(dy));
        dy = dy/(fabsf(dy));
    }

    if(0)
    // if(!IsInsideCanvas(x1,y1) || !IsInsideCanvas(x2,y2))
    {
        cerr("FORA DO CANVAS\n")
        // fprintf(stderr, "P1(%2.1f, %2.1f)  P2(%2.1f, %2.1f)\n", x1, y1, x2, y2);

        vector<Point> pts_intersec;
        int num_intersec = 0;

        float coef_ang = dy/dx;
        float coef_lin = y1 - coef_ang*x1;
        float border_x;
        float border_y;

        border_y = 0;
        border_x =border_y-(coef_lin/coef_ang);

        // intersect segment with canvas borders
        if( (border_x-x1) * (border_x-x2) <= 0 ) // that means if point border_x is in between x1 and x2
        {
            num_intersec++;
            pts_intersec.push_back(Point(border_x, border_y));
        }

        border_y = surf->h;
        border_x = border_y-(coef_lin/coef_ang);

        if( (border_x-x1) * (border_x-x2) <= 0 )
        {
            num_intersec++;
            pts_intersec.push_back(Point(border_x, border_y));
        }

        border_x = 0;
        border_y = border_x*coef_ang + coef_lin;

        if( (border_y-y1) * (border_y-y2) <= 0 )
        {
            num_intersec++;
            pts_intersec.push_back(Point(border_x, border_y));
        }

        border_x = surf->w;
        border_y = border_x*coef_ang + coef_lin;

        if( (border_y-y1) * (border_y-y2) <= 0 )
        {
            num_intersec++;
            pts_intersec.push_back(Point(border_x, border_y));
        }

        // fprintf(stderr, "P1(%2.1f, %2.1f)  P2(%2.1f, %2.1f)\n", pts_intersec[0].x, pts_intersec[0].y, pts_intersec[1].x, pts_intersec[1].y);

        switch(num_intersec)
        {
            case 1:
            {
                if(!IsInsideCanvas(x1,y1))
                {
                    x1 = pts_intersec[0].x;
                    y1 = pts_intersec[0].y;
                }
                else
                {
                    x2 = pts_intersec[0].x;
                    y2 = pts_intersec[0].y;
                }
            }
            break;
            case 2:
            {
                Point A(pts_intersec[0]);
                Point B(pts_intersec[1]);

                x1 = A.x;
                y1 = A.y;
                x2 = B.x;
                y2 = B.y;

                dx = (B.x-A.x);
                dy = (B.y-A.y);

                if(fabsf(dx) > fabsf(dy))
                {
                    dy = dy/(fabsf(dx));
                    dx = dx/(fabsf(dx));
                }
                else
                {
                    dx = dx/(fabsf(dy));
                    dy = dy/(fabsf(dy));
                }
            }
            break;
            default:
            {
                return;
            }
            break;
        }

    }

    x = x1;
    y = y1;
    xi = (int)x1;
    yi = (int)y1;


    while((x-x2)*dx <= 0 && (y-y2)*dy <= 0)
    {
        static int whilst =0;
        PaintPixel(xi, yi, red, green, blue);
        x += dx;
        y += dy;
        xi = (int)(x + 0.5);
        yi = (int)(y + 0.5);

        if(fabs(x) > 1.0E+7)
        {
            cerr("x > 1.0E+7")
            cerrv(x1)
            cerrv(y1)
            cerrv(x2)
            cerrv(y)
            cerrv(x)
            cerrv(y)
        }
        else if(fabs(x) < 1.0E-7 && x != 0)
        {
            cerr("x < 1.0E-7")
            cerrv(x)
            cerrv(y)
        }
        if(fabs(x) > 1.0E+7)
        {
            cerr("y > 1.0E+7")
            cerrv(x)
            cerrv(y)
        }
        else if(fabs(y) < 1.0E-7 && y != 0)
        {
            cerr("y < 1.0E-7")
            cerrv(x)
            cerrv(y)
        }

        whilst++;

    }
    if(y2 >= 0 && y2 < surf->h && x2 >= 0 && x2 < surf->w)
        PaintPixel(x2, y2, red, green, blue);

}

void Brizoler::Rect(int left_cornerX, int left_cornerY, int width, int height, uint thickness, Uint8 red, Uint8 green, Uint8 blue)
{
    PaintRow(left_cornerY, left_cornerX, left_cornerX + width, thickness, red, green, blue);            //  -----
    PaintColumn(left_cornerX + width, left_cornerY, left_cornerY + height, thickness, red, green, blue); //       |
    PaintRow(left_cornerY + height, left_cornerX, left_cornerX + width, thickness, red, green, blue);   //  ______
    PaintColumn(left_cornerX, left_cornerY, left_cornerY + height, thickness, red, green, blue);         // |
}

void Brizoler::BlackScreen()
{
    this->wi->FillCanvas(0, 0, 0);
}

void Brizoler::BlurScreen()
{

    Uint8 r[9];
    Uint8 g[9];
    Uint8 b[9];

    Uint32* pixels = (Uint32*) wi->canvas->pixels;

    for (int y = 0; y < surf->h; ++y)
    {
        for (int x = 0; x < surf->w; ++x)
        {
            SDL_GetRGB( pixels[((x-1) + (y-1)*surf->w)], wi->canvas->format, &r[0], &g[0], &b[0]);
            SDL_GetRGB( pixels[((x)   + (y-1)*surf->w)], wi->canvas->format, &r[1], &g[1], &b[1]);
            SDL_GetRGB( pixels[((x+1) + (y-1)*surf->w)], wi->canvas->format, &r[2], &g[2], &b[2]);
            SDL_GetRGB( pixels[((x-1) + (y)*surf->w  )], wi->canvas->format, &r[3], &g[3], &b[3]);
            SDL_GetRGB( pixels[((x)   + (y)*surf->w  )], wi->canvas->format, &r[4], &g[4], &b[4]);
            SDL_GetRGB( pixels[((x+1) + (y)*surf->w  )], wi->canvas->format, &r[5], &g[5], &b[5]);
            SDL_GetRGB( pixels[((x-1) + (y+1)*surf->w)], wi->canvas->format, &r[6], &g[6], &b[6]);
            SDL_GetRGB( pixels[((x)   + (y+1)*surf->w)], wi->canvas->format, &r[7], &g[7], &b[7]);
            SDL_GetRGB( pixels[((x+1) + (y+1)*surf->w)], wi->canvas->format, &r[8], &g[8], &b[8]);

            PaintPixel(x, y
                , (r[0] + r[1] + r[2]
                  + r[3] + 2*r[4] + r[5]
                  + r[6] + r[7] + r[8])/10

                , (g[0] + g[1] + g[2]
                  + g[3] + 2*g[4] + g[5]
                  + g[6] + g[7] + g[8])/10

                , (b[0] + b[1] + b[2]
                  + b[3] + 2*b[4] + b[5]
                  + b[6] + b[7] + b[8])/10
                );
        }
    }
}

void Brizoler::LinesGettingSmaller(int num_lines)
{
    int current_line_height = 0;
    int current_line_width = surf->w;

    float space_between_lines = (float) this->mouse_y/num_lines;
    float width_decrease = ((float) current_line_width)/(2*num_lines);

    float width_delta1 = (float) this->mouse_x/num_lines;
    float width_delta2 = (float) (surf->w - this->mouse_x)/num_lines;

    float height_delta1 = (float) this->mouse_y/num_lines;
    float height_delta2 = (float) (surf->h - this->mouse_y)/num_lines;

    for(int i = 0;current_line_height <= this->mouse_y &&
                 current_line_height <= surf->h &&
                  current_line_width > 0 &&
                  space_between_lines > 1; i++ )
    {
        PaintRow(current_line_height, (int)(i*width_delta1), surf->w - (int)(i*width_delta2), 1, 255, 255, 255);
        PaintColumn((int)(i*width_delta1), current_line_height, surf->h - (int)(i*height_delta2), 1, 255, 255, 255);

        current_line_height += (int) space_between_lines;
    }

}

void Brizoler::InfiniteSquares(int num_squares)
{
    static float offset = 0;
    static Uint8 red_offset = 0;
    static Uint8 green_offset = 0;
    static Uint8 blue_offset = 0;

    float horizontal_step = ((float) mouse_x)/num_squares; // distance between squares
    float vertical_step = ((float) mouse_y)/num_squares;
    float width_decrease_step = ((float) surf->w)/num_squares; // square lines decreasing rate
    float height_decrease_step = ((float) surf->h)/num_squares;

    float square_x = 0;
    float square_y = 0;
    float square_width = surf->w;
    float square_height = surf->h;

    const int amplitude = 20;
    float thickness = amplitude;
    // float thickness = amplitude * sin( (float)i/num_squares*2*M_PI + offset) + amplitude+1;

    const float thickness_step = (float)(amplitude)/num_squares;
    float far = 0;

    for(int i = 0; i < num_squares; i++)
    {
        red_offset += 1;
        green_offset += 2;
        blue_offset += 3;
        far += thickness_step;

        Rect(square_x, square_y, square_width, square_height, (int)2, ((Uint8)red_offset*green_offset), ((Uint8)green_offset*blue_offset), ((Uint8)blue_offset*red_offset));

        square_x += horizontal_step ;
        square_y += vertical_step ;
        square_width -= width_decrease_step ;
        square_height -=  height_decrease_step;
    }

    offset += 0.1;
}
/*
void Brizoler::Stars(uint num_stars)
{
    static Star **stars = 0;

    if(!stars)
    {
        stars = (Star**) malloc(num_stars*(sizeof(Star*)));
        for(uint k = 0; k < num_stars; k++)
        {
            stars[k] = new Star(surf->w, surf->h, (Uint32*) wi->canvas->pixels);
            // stars[k] = new Star(surf->w, surf->h, 4, this->canvas);
            stars[k]->Set_Color(random()%256, random()%256, random()%256);
        }
    }
    static int count = 1;
    static int count2 = 50;

    static float max_accel = 0;
    static float min_accel = 999999;
    static float max_radius_x = 0;
    static float min_radius_x = 999999;
    static float max_radius_y = 0;
    static float min_radius_y = 999999;

    for(uint i = 0; i < num_stars; i++)
    {

        if(change_speed_up)
            stars[i]->Multiply_vel(1.1);
        else if(change_speed_down)
            stars[i]->Multiply_vel(1/1.1);

        if(gravitate_mouse)
        {
            float star_x = stars[i]->pos_x;
            float star_y = stars[i]->pos_y;
            float radius_x = this->mouse_x - star_x; // vector in the line that pass through each planets center
            float radius_y = this->mouse_y - star_y;

            float dist_sq = radius_x*radius_x + radius_y*radius_y;
            float dist = sqrt(dist_sq);

            normalizeVector(radius_x, radius_y);

            float G = 0.1;
            float mouse_mass = 0.5;
            float mouse_radius = 15;
            float accel;

            if(0)
            // if(dist <= mouse_radius)
            {
                // idea here is to make the circle around the mouse simulate a surface.
                // so when a star touches the surface, it bounces with the out_angle = in_angle

                // if(stars[i]->vel_x*radius_x + stars[i]->vel_y*radius_y > 0) // dot product
                if(0) // dot product
                {
                    float in_angle = atan2(-radius_y, -radius_x);

                    rotateVector(stars[i]->vel_x, stars[i]->vel_y, (M_PI)- 2*in_angle);
                    stars[i]->vel_x *= 0.9;
                    stars[i]->vel_y *= 0.9;
                    accel = 0.01;
                }
                else
                {
                    accel = -0.1;
                }
            }
            else
            {
                // accel = G*mouse_mass*dist/10;
                // accel = 1000*G*mouse_mass*(dist-mouse_radius)/(dist_sq);
                // accel = 1000*G*mouse_mass/(dist_sq);
                accel = G*mouse_mass*10; /// this way is more interesting

            }

            CHANGE_MIN_MAX(accel, min_accel, max_accel)
            CHANGE_MIN_MAX(radius_x, min_radius_x, max_radius_x)
            CHANGE_MIN_MAX(radius_y, min_radius_y, max_radius_y)

            if((isnormal(accel)) // accel was generating nan and radius inf
                && (isnormal(radius_x) || radius_x == 0.0)
                && (isnormal(radius_y) || radius_y == 0.0))
            {
                stars[i]->Change_Acceleration(accel*radius_x, accel*radius_y);
            }

            if(dist <= mouse_radius)
            {
                stars[i]->Set_Color(255, 0, 0);
            }
            else if(1)
            // else if(accel < 5)
            {
                // stars[i]->Set_Color(radius_x*radius_x - dist, radius_y*radius_y - dist, dist);
                // stars[i]->Set_Color((accel)*(1), (accel)*(1), (accel)*(1));
                stars[i]->Set_Color(stars[i]->vel_x, stars[i]->vel_y, stars[i]->vel_x*stars[i]->vel_y);
                // stars[i]->Set_Color(accel+stars[i]->vel_x, accel+stars[i]->vel_y, accel*stars[i]->vel_x*stars[i]->vel_y);
                // static int color_counter = 0;
                // color_counter++;
                // if(color_counter % 10 == 0)
                //     stars[i]->Set_Color(random()%256, random()%256, random()%256);
            }
            else
            {
                stars[i]->Set_Color(255, 255, 255);
            }
        }
        else
        {
            stars[i]->Change_Acceleration(0,0);
        }

        stars[i]->Animate(this->mouse_x, this->mouse_y, 10, 11);


        if(count % count2 == 0)
        // if(stars_change_direction_flag)
        {
            // stars[i]->Change_Direction();
        }
        count2 = (int) 50 + 25*sin(count);
    }
        count++;
    // stars_change_direction_flag = 0;

    // fprintf(stderr, "Accel: [%g,%g]\n", min_accel, max_accel);
    // fprintf(stderr, "Radius_x: [%g,%g]\n", min_radius_x, max_radius_x);
    // fprintf(stderr, "Radius_y: [%g,%g]\n", min_radius_y, max_radius_y);
    change_speed_up = false;
    change_speed_down = false;
}
*/
void Brizoler::OnPaint(int OffsetX, int OffsetY)
{
    if(SDL_MUSTLOCK(surf))
    {
        SDL_LockSurface(surf);
    }

    Uint8 red;
    Uint8 green;
    Uint8 blue;

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    int prod;

    switch(mode)
    {
        case 1:
        {
            doodle->InputRefresh();
            doodle->Run();
        }break;

        case 4:
        case 5:
        {
            static int num = 30;
            static int sin_phase = 0;
            static int change_counter = 0;

            // if(change_counter++ == 10)
            // {
            //     num = (int)(15 + 14*sin(sin_phase++));
            // }


            BlackScreen();
            // SDL_SetAlpha(surf, SDL_SRCALPHA|SDL_RLEACCEL, 40);
            InfiniteSquares(num);

            // for (int i = 0; i < 9999999; ++i);
            // SDL_SetAlpha(surf, SDL_SRCALPHA|SDL_RLEACCEL, 40);
            // InfiniteSquares(num);
            // for (int i = 0; i < 9999999; ++i);
            // SDL_SetAlpha(surf, SDL_SRCALPHA|SDL_RLEACCEL, 255);
            // InfiniteSquares(num);
            // for (int i = 0; i < 9999999; ++i);
            // SDL_SetAlpha(surf, SDL_SRCALPHA|SDL_RLEACCEL, 255);
            // InfiniteSquares(num);
        }break;

        case 6:
        {
            // static int num_stars = 1000;
            // Stars(num_stars);
            doodle2->InputRefresh();
            doodle2->Run();
        }break;

        case 7:
        {
            for (int y = 0; y < surf->h; ++y)
            {
                for (int x = 0; x < surf->w; ++x)
                {
                    prod = (int)(array_prod[y][x]*100*param_zoom);
                    // prod = (int)(( tan(y*2*M_PI/surf->h) + tan((float)x*2*M_PI/surf->w))*param_zoom);
                    red = (prod + OffsetX)%256;
                    green = (prod + (int)(param_speed*OffsetY))%256;
                    blue = (prod)%256;

                    PaintPixel(x, y, red, green ,blue);
                    // PaintPixel(x,y,x,y,x+y);
                }
            }

        }break;

        case 8:
        {
            BlackScreen();
            // vector of points
            vector<Point> pts;
            int pts_size = 10000 * (param_zoom/zoom_limit) + 1; /// points number

            // Points determination

            // * Circunference
            float radius = (surf->w < surf->h? surf->w/2 : surf->h/2);
            float angle_delta = 2*M_PI/(float)pts_size;

            Point center_pt;
            center_pt.x = surf->w/2;
            center_pt.y = surf->h/2;

            Point initial_pt;
            initial_pt.x = center_pt.x + radius;
            initial_pt.y = center_pt.y;

            Point current_pt = initial_pt;

            for(int i = 0; i < pts_size; ++i)
            {
                current_pt.x = center_pt.x + radius*cos(angle_delta*i);
                current_pt.y = center_pt.y + -radius*sin(angle_delta*i);

                pts.push_back(current_pt);
            }

            // * Rectangle
/*            float rec_delta = (surf->w*2 + surf->h*2)/pts_size;
            Point current_pt;

            int side_1 = 0;
            int side_2 = 0;
            int side_3 = 0;
            int side_4 = 0;

            for (int i = 0; i < pts_size; ++i)
            {
                if(side_1 < surf->w-1)
                {
                    current_pt.x = side_1;
                    current_pt.y = 0;

                    side_1 += rec_delta;
                }
                else if(side_2 < surf->h-1)
                {
                    current_pt.x = surf->w-1;
                    current_pt.y = side_2;

                    side_2 += rec_delta;
                }
                else if(side_3 < surf->w-1)
                {
                    current_pt.x = surf->w-1 - side_3;
                    current_pt.y = surf->h-1;

                    side_3 += rec_delta;
                }
                else
                {
                    current_pt.x = 0;
                    current_pt.y = surf->h-1 - side_4;

                    side_4 += rec_delta;
                }

                pts.push_back(current_pt);
            }
*/
            // Connecting the dots
            for (int i = 0; i < pts.size(); ++i)
            {
                // int step = pts.size()*(param_speed/speed_limit);
                // int step = int(30*(param_speed/speed_limit)*i)%(111);
                int step = 30*(param_speed/speed_limit)*i;

                Uint8 red;
                Uint8 green;
                Uint8 blue;

                float progress = (float)i/pts.size();

                //GRADIENT?
                if(progress < 0.33)
                {
                    red = 255 - (progress/0.33)*255;
                    green = (progress/0.33)*255;
                    blue = 0;
                }
                else if(progress < 0.66)
                {
                    red = 0;
                    green = 255 - ((progress-0.33)/0.33)*255;
                    blue = ((progress-0.33)/0.33)*255;
                }
                else
                {
                    red = ((progress-0.66)/0.33)*255;
                    green = 0;
                    blue = 255 - ((progress-0.66)/0.33)*255;
                }

                static int new_i = 0;

                // PaintLine(pts[i].x, pts[i].y, pts[ int(i+step)%pts.size() ].x, pts[ int(i+step)%pts.size() ].y, 1, red, green, blue);
                PaintLine(pts[(new_i)%pts.size()].x, pts[(new_i)%pts.size()].y, pts[ int(new_i+step)%pts.size() ].x, pts[ int(new_i+step)%pts.size() ].y, 1, 255-i, 255-step, i+step);
                new_i += step;
            }


            /// FRACTAL
/*            BlackScreen();

            const int MaxIters = 100;
             int SIZE = 500*(float)param_speed/speed_limit;
            const int BLACK = -1;
            const float LEFT = -2.0;
            const float RIGHT = 1.0;
            const float TOP = 1.0;
            const float BOTTOM = -1.0;

            short   x, y, count;
            long double zr, zi, cr, ci;
            long double rsquared, isquared;

            for (y = 0; y < SIZE; y++)
            {
                for (x = 0; x < SIZE; x++)
                {
                    zr = 0.0;
                    zi = 0.0;
                    cr = LEFT + x * (RIGHT - LEFT) / SIZE;

                    ci = TOP + y * (BOTTOM - TOP) / SIZE;
                    rsquared = zr * zr;
                    isquared = zi * zi;

                    for (count = 0; rsquared + isquared <= 4.0
                                    && count < MaxIters; count++)
                    {
                        zi = zr * zi * 2;
                        zi += ci;

                        zr = rsquared - isquared;
                        zr += cr;

                        rsquared = zr * zr;
                        isquared = zi * zi;
                    }

                    if (rsquared + isquared <= 4.0)
                        PaintPixel(x+(surf->w/2), y+(surf->h/2), 0, 0, 0);
                    else
                        PaintPixel(x+(surf->w/2), y+(surf->h/2), count%256, (count*2)%256, (count*3)%256);
                }
            }
*/
        }break;

        case 9:
        {


            static bool initiated = false;

            if(!initiated)
            {
                if(SDL_MUSTLOCK(canvas_surf))
                {
                    SDL_LockSurface(canvas_surf);
                }

                BlackScreen();

                //rotation angle
                float rot_angle = param_zoom/zoom_limit * 2 * M_PI;
                float sin_rot = sin(rot_angle);
                float cos_rot = cos(rot_angle);




                // Where the rotation center will be in the screen.
                static float new_center_x = surf->w/2;
                static float new_center_y = surf->h/2;

                // gives more movemente freedom.
                new_center_x = param_coord_x;
                new_center_y = param_coord_y;

                // where the center was whem rotation around (0,0).
                float old_center_x = (surf->w/2)*cos_rot - (surf->h/2)*sin_rot;
                float old_center_y = (surf->w/2)*sin_rot + (surf->h/2)*cos_rot;

                // Translation vector that will be applied on the old center to get to the new place
                float move_x = new_center_x - old_center_x;
                float move_y = new_center_y - old_center_y;

                for (int y = 0; y < surf->h; ++y)
                {
                    for (int x = 0; x < surf->w; ++x)
                    {
                        // if((x%2 == 0 && y%2 == 0) || (x%2 != 0 && y%2 != 0) )
                        if(random()%(int)(100*(param_speed+1)/speed_limit) == 0)
                        {
                            int new_x = x*cos_rot - y*sin_rot;
                            int new_y = x*sin_rot + y*cos_rot;

                            static float seed = 0.1;
                            if(seed < 2*M_PI)
                            {
                                seed += 0.1;
                            }
                            else
                            {
                                seed = 0.1;
                            }

                            Uint8 back_red = 128 + 128*sin(seed + x*2*M_PI/surf->w);
                            Uint8 back_green = 128 + 128*sin(seed + 2*M_PI*(x/surf->w + 0.3));
                            Uint8 back_blue = 128 + 128*sin(seed + 2*M_PI*(x/surf->w + 0.6));

                            Uint8 front_red = 128 + 128*sin(seed + 2*M_PI*(x/surf->w + 0.9));
                            Uint8 front_green = 128 + 128*sin(seed + 2*M_PI*(x/surf->w + 0.3));
                            Uint8 front_blue = 128 + 128*sin(seed + x*2*M_PI/surf->w + 0.6);


                            PaintPixel(x, y, front_red, front_green, front_blue);
                            // PaintPixel(new_x + move_x, new_y + move_y, back_red, back_green, back_blue, canvas_surf);
                        }
                    }
                }

                if(SDL_MUSTLOCK(canvas_surf))
                {
                    SDL_UnlockSurface(canvas_surf);
                }
                // SDL_UpdateRect(canvas_surf, 0, 0, canvas_surf->w, canvas_surf->h);

                // SDL_SetAlpha(surf, SDL_SRCALPHA|SDL_RLEACCEL, 125);

                // initiated = true;
            }



        }break;

        default:
        {
            static int num = 36;

            BlackScreen();
            LinesGettingSmaller(num);
        }break;
    }

    if ( keystate[SDL_SCANCODE_C] ) // bom jeito de ler o estado das teclas
        BlurScreen();


    if(SDL_MUSTLOCK(surf))
    {
        SDL_UnlockSurface(surf);
    }

    this->wi->RefreshCanvas();

    // SDL_UpdateTexture(this->texture, NULL, this->surf->pixels, MAX_WIDTH * sizeof (Uint32));

    // SDL_RenderClear(this->renderer);
    // SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    // SDL_RenderPresent(this->renderer);

    // SDL_UpdateRect(surf, 0, 0, surf->w, surf->h);

    SDL_Delay(5);
}