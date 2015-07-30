#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <vector>
#include "Brizoler.h"
#include "Star.h"
// #include "sin.h"

// #define zoom_limit 2


#define RED_FROM_PIXEL(pix)   (pix & 00110000) >> 16
#define GREEN_FROM_PIXEL(pix) (pix & 00001100) >> 8
#define BLUE_FROM_PIXEL(pix)  (pix & 00000011)

Uint8 Brizoler::GetRed(int x, int y)
{
    if(x >= 0 && x < surf->w && y >= 0 && y < surf->h)
    {
        Uint32 *pixel = (Uint32*)surf->pixels;
        return (pixel[y*surf->w + x] & 00110000) >> 16;
    }

}

Uint8 Brizoler::GetGreen(int x, int y)
{
    if(x >= 0 && x < surf->w && y >= 0 && y < surf->h)
    {
        Uint32 *pixel = (Uint32*)surf->pixels;
        return (pixel[y*surf->w + x] & 00001100) >> 8;
    }

}

Uint8 Brizoler::GetBlue(int x, int y)
{
    if(x >= 0 && x < surf->w && y >= 0 && y < surf->h)
    {
        Uint32 *pixel = (Uint32*)surf->pixels;
        return (pixel[y*surf->w + x] & 00000011);
    }

}

static int zoom_limit = 20;
static int speed_limit = 20;

static int stars_change_direction_flag = 0;

float param_zoom = 1;
float param_speed = 1;
int mode = 1;


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
        while(SDL_PollEvent(&Event))
        {
            OnEvent(&Event);
        }

        OnPaint(offsetx, offsety);

        offsetx++;
        offsety++;

    }

    OnCleanup();

    return 0;
}

bool Brizoler::OnInit()
{

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    if((this->surf = SDL_SetVideoMode(1600, /*width , 0 means any width*/
                                    700, /*height, 0 means any height*/
                                    0, /*bites per pixel, 0 means use current*/
                                    SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE
                                    // | SDL_FULLSCREEN
                                    )
                                    ) == NULL &&
        (this->canvas_surf = SDL_SetVideoMode(0, /*width , 0 means any width*/
                                        0, /*height, 0 means any height*/
                                        0, /*bites per pixel, 0 means use current*/
                                        SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE
                                        )
                                        ) == NULL)
    {
        return false;
    }

    cout << "surf->w: " <<  surf->w << endl;
    cout << "surf->h: " <<  surf->h << endl;

    uint canvas_size = this->surf->h*this->surf->w*3;

    canvas = new uint[canvas_size];
    if(!canvas)
    {
        return false;
    }

    SDL_EnableKeyRepeat(1, 10);

    // SDL_ShowCursor(SDL_DISABLE);

    return true;
}

void Brizoler::OnCleanup()
{
    free(canvas);
    SDL_FreeSurface(surf);
    SDL_Quit();
}


void Brizoler::OnEvent(SDL_Event* event)
{
    CEvent::OnEvent(event);
}

void Brizoler::OnMouseFocus()
{

}

void Brizoler::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{

    this->mouse_x = mX;
    this->mouse_y = mY;

    if(Left)
    {
        param_zoom = ((float)mY/surf->h)*zoom_limit + 0.01;
        param_speed = ((float)mX/surf->w)*speed_limit + 0.01;
    }
}

void Brizoler::OnLButtonDown(int mX, int mY)
{
    param_zoom = ((float)mY/surf->h)*zoom_limit + 0.01;
    param_speed = ((float)mX/surf->w)*speed_limit + 0.01;
}

void Brizoler::OnLButtonUp(int mX, int mY)
{
}

void Brizoler::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    switch(sym)
    {
        case SDLK_UP:
        {
            if(param_zoom < zoom_limit)
            {
                param_zoom += 0.005;
            }
        }
        break;

        case SDLK_DOWN:
        {
            if(param_zoom > 0.015)
            {
                param_zoom -= 0.005;
            }
        }
        break;

        case SDLK_RIGHT:
        {
            if(param_speed < speed_limit)
            {
                param_speed += 0.01;
            }
        }
        break;

        case SDLK_LEFT:
        {
            if(param_speed > 0.015)
            {
                param_speed -= 0.01;
            }
        }
        break;

        case SDLK_ESCAPE:
        {
            OnExit();
        }
        break;

        case SDLK_1:
        {
            mode = 1;
        }break;

        case SDLK_2:
        {
            mode = 2;
        }break;

        case SDLK_3:
        {
            mode = 3;
        }break;

        case SDLK_4:
        {
            mode = 4;
        }break;

        case SDLK_5:
        {
            mode = 5;
        }break;

        case SDLK_6:
        {
            mode = 6;
        }break;

        case SDLK_7:
        {
            mode = 7;
        }break;

        case SDLK_c:
        {
            stars_change_direction_flag = 1;
        }break;

        default:
        break;
    }
}

void Brizoler::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void Brizoler::OnResize(int w,int h)
{
    surf = SDL_SetVideoMode(w, h, 0, SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);

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

bool Brizoler::fPaintPixel(float x, float y, Uint8 red, Uint8 green, Uint8 blue)
{

    // normalizing color intensity;
    float f_red = (float)red/255;
    float f_green = (float)green/255;
    float f_blue = (float)blue/255;

    Uint32 *pixel = (Uint32*)surf->pixels;
    uint index;


    for (int j = (int)y-1; j < (int)y+2; ++j)
    {
        for (int i = (int)x-1; i < (int)x+2; ++i)
        {
            if(i >= 0 && i < surf->w && j >= 0 && j < surf->h)
            {
                index = j*surf->w + i;

                float f_pix_red = RED_FROM_PIXEL(pixel[index])/255;
                float f_pix_green = GREEN_FROM_PIXEL(pixel[index])/255;
                float f_pix_blue = BLUE_FROM_PIXEL(pixel[index])/255;

                // float dist;
                // if((int) x == i)
                //     dist = 4;
                // else
                //     dist = 3.75;

                // const float max_dist = 4;
                float dist = 1+sqrt((pow(x-(i+0.5),2) + pow(j-(y+0.5),2))/2);
                const float max_dist = (1.5*sqrt(2));

                float new_red = (((max_dist-dist)*(f_pix_red) + (dist)*(f_red))/max_dist);
                float new_green = (((max_dist-dist)*(f_pix_green) + (dist)*(f_green))/max_dist);
                float new_blue = (((max_dist-dist)*(f_pix_blue) + (dist)*(f_blue))/max_dist);

                // float new_red = sqrt(((max_dist-dist)*pow(f_pix_red,2) + (dist)*pow(f_red,2))/max_dist);
                // float new_green = sqrt(((max_dist-dist)*pow(f_pix_green,2) + (dist)*pow(f_green,2))/max_dist);
                // float new_blue = sqrt(((max_dist-dist)*pow(f_pix_blue,2) + (dist)*pow(f_blue,2))/max_dist);

                pixel[index] = SDL_MapRGB(surf->format, new_red*255, new_green*255, new_blue*255);
                // pixel[index] = SDL_MapRGB(surf->format, red, green, blue);
                return true;
            }
        }
    }


    return false;
}

bool Brizoler::PaintPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
{
    Uint32 *pixel = (Uint32*)surf->pixels;


    uint index;

    if(x >= 0 && x < surf->w && y >= 0 && y < surf->h)
    {
        index = y*surf->w + x;
        pixel[index] = SDL_MapRGB(surf->format, red, green, blue);
        return true;
    }
    return false;
}

void Brizoler::PaintRow(int y, int x1, int x2, uint thickness, Uint8 red, Uint8 green, Uint8 blue)
{
    Uint32 *pixel = (Uint32*)surf->pixels;

    for(int y_counter = y; y_counter < y+thickness; y_counter++)
    {
        for (int x_counter = 0; x_counter < x2-x1; ++x_counter)
        {
            PaintPixel(x_counter+x1, y_counter-thickness/2, red, green, blue);
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
            PaintPixel(x_counter-thickness/2, y_counter+y1, red, green, blue);
        }
    }
}


void Brizoler::PaintLine(float x1, float y1, float x2, float y2, uint thickness, Uint8 red, Uint8 green, Uint8 blue)
{
    // fprintf(stderr, "P1(%.2f , %.2f) -> P2(%.2f , %.2f)\n", x1, y1, x2, y2);
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

    x = x1;
    y = y1;
    xi = (int)x1;
    yi = (int)y1;

    int count_pri = 0;

    while((x-x2)*dx < 0 && (y-y2)*dy < 0)
    {
        if(yi >= 0 && yi < surf->h && xi >= 0 && xi < surf->w)
            PaintPixel(xi, yi, red, green, blue);
        x += dx;
        y += dy;
        xi = (int)(x + 0.5);
        yi = (int)(y + 0.5);
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
    for (int y = 0; y < surf->h; ++y)
    {
        for (int x = 0; x < surf->w; ++x)
        {
            PaintPixel(x, y, 0, 0, 0);
        }
    }
}

void Brizoler::BlurScreen()
{
    for (int y = 0; y < surf->h; ++y)
    {
        for (int x = 0; x < surf->w; ++x)
        {
            PaintPixel(x, y
                , ((int)GetRed(x-1,y-1) + (int)GetRed(x-1,y) + (int)GetRed(x-1,y+1) + (int)GetRed(x,y-1) + (int)2*GetRed(x,y) + (int)GetRed(x,y+1) + (int)GetRed(x+1,y-1) + (int)GetRed(x+1,y) + (int)GetRed(x+1,y+1))/10
                , ((int)GetGreen(x-1,y-1) + (int)GetGreen(x-1,y) + (int)GetGreen(x-1,y+1) + (int)GetGreen(x,y-1) + (int)2*GetGreen(x,y) + (int)GetGreen(x,y+1) + (int)GetGreen(x+1,y-1) + (int)GetGreen(x+1,y) + (int)GetGreen(x+1,y+1))/10
                , ((int)GetBlue(x-1,y-1) + (int)GetBlue(x-1,y) + (int)GetBlue(x-1,y+1) + (int)GetBlue(x,y-1) + (int)2*GetBlue(x,y) + (int)GetBlue(x,y+1) + (int)GetBlue(x+1,y-1) + (int)GetBlue(x+1,y) + (int)GetBlue(x+1,y+1))/10
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

void Brizoler::Stars(uint num_stars)
{
    static Star **stars = 0;
    Uint32 *writing_pixel;


    if(!stars)
    {
        stars = (Star**) malloc(num_stars*(sizeof(Star*)));
        for(uint k = 0; k < num_stars; k++)
        {
            stars[k] = new Star(surf->w, surf->h, (Uint32*) surf->pixels);
            // stars[k] = new Star(surf->w, surf->h, 4, this->canvas);
            stars[k]->Set_Color(random()%256, random()%256, random()%256);
        }
    }
    static int count = 1;
    static int count2 = 50;

    for(uint i = 0; i < num_stars; i++)
    {
        stars[i]->Animate(this->mouse_x, this->mouse_y, 10, 11);


        if(count % count2 == 0)
        // if(stars_change_direction_flag)
        {
            stars[i]->Change_Direction();
        }
        count2 = (int) 50 + 25*sin(count);
    }
        count++;
    // stars_change_direction_flag = 0;
}

float Brizoler::PlotFunc(float x)
{
    static float offset = 0;
    static float inc = 0.001;
    static float persist_inc = 0;

    offset += inc;
    float inc_limit = 100*inc;

    if(inc > 0)
    {
        if(persist_inc <= inc_limit)
            persist_inc += inc;
        else
            inc = -inc;
    }
    else
    {
        if(persist_inc >= 0)
            persist_inc += inc;
        else
            inc = -inc;
    }

    float param_x = 2*param_speed/speed_limit;
    float param_y = -0.5 + param_zoom/zoom_limit;

    const int amplitude = 300;
    const int max_freq = 200;

    // return (param_x)*x*x + -(surf->h/2)+(surf->h)*(param_y);
    return amplitude*sin(param_x*max_freq*(x/surf->w)*2*M_PI+ offset) + (param_y*(surf->h-amplitude*2));
    // return amplitude*tan(param_x*max_freq*(x/surf->w)*2*M_PI+ offset) + (param_y*(surf->h-amplitude*2));
    // return 400*(param_y)*sin((x/surf->w)*2*M_PI*100*(param_x));
}

void Brizoler::Plot() // in the form of y = 3x + 4
{

    static bool ploted = false;

    static float inc = 0.01;
    static float persist_inc = 0;
    float inc_limit = 100*inc;

    if(inc > 0)
    {
        if(persist_inc <= inc_limit)
            persist_inc += inc;
        else
            inc = -inc;
    }
    else
    {
        if(persist_inc >= -1)
            persist_inc += inc;
        else
            inc = -inc;
    }

    BlackScreen();

    vector<float> func_x;
    vector<float> func_y;
    vector<float>::iterator it_x;
    vector<float>::iterator it_y;

    float zero_x = (float)surf->w/2;
    float zero_y = (float)surf->h/2;
    float cart_y_min = (float)-surf->h/2;
    float cart_y_max = (float)surf->h/2;

    PaintRow(zero_y, 0, surf->w, 1, 125, 125, 125);
    PaintColumn(zero_x, 0, surf->h, 1, 125, 125, 125);

    for (int x = 0; x < surf->w; x++)
    {
        float cart_x = -zero_x+x;
        float cart_y = PlotFunc(cart_x);

        if(cart_y <= cart_y_max && cart_y >= cart_y_min)
        {
            func_x.push_back(x);
            func_y.push_back(zero_y - cart_y);
        }

    }

    for(it_x = func_x.begin(), it_y = func_y.begin();
        it_x != func_x.end()-1, it_y != func_y.end()-1;
        it_x++, it_y++)
    {
        if(*(it_x+1) == *(it_x) +1)
        {
            // PaintLine(*it_x, *it_y, *(it_x+1), *(it_y+1), 1, 255, 255, 255);
            PaintLine(*it_x, *it_y, *(it_x+1), *(it_y+1), 1, sin(*it_x)*255, cos(*it_x)*255, tan(*it_x)*255);
            // PaintLine(*it_x, *it_y, *(it_x+1), *(it_y+1), 1, ((sin(persist_inc*M_PI)+1)/2)*255, (*it_y)*255, (*it_x * *it_y)*255);
        }
    }
    ploted = true;
    return;
}

void Brizoler::OnPaint(int OffsetX, int OffsetY)
{
    if(SDL_MUSTLOCK(surf))
    {
        SDL_LockSurface(surf);
    }

    Uint8 red;
    Uint8 green;
    Uint8 blue;

    int bytes_per_pixel = surf->format->BytesPerPixel;
    Uint32 *pixel;
    pixel = (Uint32*)surf->pixels;



            // float i_norm = (float) i/surf->h;
            // float j_norm = (float) j/surf->w;
    int prod;
    switch(mode)
    {
        case 1:
        {

            for (int y = 0; y < surf->h; ++y)
            {
                for (int x = 0; x < surf->w; ++x)
                {
                    prod = (int)((y*y + x*x)*param_zoom);
                    red = (prod + OffsetX)%256;
                    green = (prod + (int)(param_speed*OffsetY))%256;
                    blue = (prod)%256;

                    PaintPixel(x, y, red, green ,blue);
                }
            }


        }break;
        case 2:
        {

            for (int y = 0; y < surf->h; ++y)
            {
                for (int x = 0; x < surf->w; ++x)
                {
                    prod = (int)(y*x*param_zoom);
                    red = (prod + OffsetX)%256;
                    green = (prod + (int)(param_speed*OffsetY))%256;
                    blue = (prod)%256;

                    PaintPixel(x, y, red, green, blue);
                }
            }


        }break;
        case 3:
        {

            for (int y = 0; y < surf->h; ++y)
            {
                for (int x = 0; x < surf->w; ++x)
                {

                    if(y%2 != 0)
                        // prod = (int)(sin((x/surf->w)*(y/surf->h) * 2 * M_PI + 40 )*param_zoom);
                        // prod = (int)((y*y + x*x)*param_zoom);
                        prod = (int)(256*sin(y)*param_zoom);
                    else
                        // prod = (int)(log((surf->h*surf->w - y*x) +1)*param_zoom*100);
                        prod = (int)(256*cos(x)*param_zoom);
                        // prod = (int)(y*x*param_zoom);
                    red = (prod + OffsetX)%256;
                    green = (prod + (int)(param_speed*OffsetY))%256;
                    blue = (prod)%256;

                    PaintPixel(x, y, red, green, blue);
                }
            }


        }break;
        case 4:
        {
            Plot();

        }break;
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
            SDL_SetAlpha(surf, SDL_SRCALPHA|SDL_RLEACCEL, 40);
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
            static int num = 10000;
            Stars(num);

        }break;

        default:
        {
            static int num = 36;

            BlackScreen();
            LinesGettingSmaller(num);
        }break;
    }

    if(SDL_MUSTLOCK(surf))
    {
        SDL_UnlockSurface(surf);
    }
    SDL_UpdateRect(surf, 0, 0, surf->w, surf->h);

    SDL_Delay(5);
}