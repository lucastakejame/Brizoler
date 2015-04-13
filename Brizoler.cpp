#include <stdio.h>
#include <math.h>
#include "Brizoler.h"
// #include "sin.h"

// #define ZOOM_LIMIT 2
#define ZOOM_LIMIT 5000
#define SPEED_LIMIT 20

float param_zoom = 1;
float param_speed = 1;
int mode = 2;


Brizoler::Brizoler()
{
    this->Running = true;
}
Brizoler::~Brizoler(){}

bool Brizoler::OnInit()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    if((Surf = SDL_SetVideoMode(0, /*width , 0 means any width*/
                                0, /*height, 0 means any height*/
                                0, /*bites per pixel, 0 means use current*/
                                SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE
                                )
                                ) == NULL)
    {
        return false;
    }

    SDL_EnableKeyRepeat(1, 10);

    return true;
}

int Brizoler::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;

    int offsetx = 30;
    int offsety = 0;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnPaint(offsetx, offsety);

        offsetx++;
        offsety++;

    }

    OnCleanup();

    return 0;
}

void Brizoler::OnCleanup() {
    SDL_FreeSurface(Surf);
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
        param_zoom = ((float)mY/Surf->h)*ZOOM_LIMIT + 0.01;
        param_speed = ((float)mX/Surf->w)*SPEED_LIMIT + 0.01;
        // printf("param_zoom : %f \n", param_zoom);
        // printf("param_speed : %f \n", param_speed);
    }
}


void Brizoler::OnLButtonDown(int mX, int mY)
{
    param_zoom = ((float)mY/Surf->h)*ZOOM_LIMIT + 0.01;
    param_speed = ((float)mX/Surf->w)*SPEED_LIMIT + 0.01;
    // printf("param_zoom : %f \n", param_zoom);
    // printf("param_speed : %f \n", param_speed);
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
            if(param_zoom < ZOOM_LIMIT){
                param_zoom += 0.005;
            }
        }
        break;

        case SDLK_DOWN:
        {
            if(param_zoom > 0.015){
                param_zoom -= 0.005;
            }
        }
        break;

        case SDLK_RIGHT:
        {
            if(param_speed < SPEED_LIMIT){
                param_speed += 0.01;
            }
        }
        break;

        case SDLK_LEFT:
        {
            if(param_speed > 0.015){
                param_speed -= 0.01;
            }
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

        default:
        break;
    }
}

void Brizoler::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}

void Brizoler::OnResize(int w,int h)
{
    Surf = SDL_SetVideoMode(w, h, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    if(NULL == Surf) {
        fprintf(stderr, "Unable to resize video mode: %s\n", SDL_GetError());
        exit(2);
    }
}

void Brizoler::OnExit()
{
    Running = false;
}

int FunctionPoint(int x)
{
    int y;

    y = x*(float)param_zoom/800;

    return y;
}

void Brizoler::PaintLine(int line, int start, int end, int red, int green, int blue)
{

    Uint32 *pixel = (Uint32*)Surf->pixels;

    int current_line;

    if(end > Surf->w)
        end = Surf->w - 1;

    // goes to desired start point
    int offset = line*Surf->w + start;
    int limit = Surf->w*Surf->h;


    for (int j = 0; j < end-start; ++j)
    {
        int index = offset + j;
        if(index > limit)
        {
            index = limit;
        }
        pixel[index] = SDL_MapRGB(Surf->format, red, green, blue);
    }
}

void Brizoler::PaintColumn(int column, int start, int end, int red, int green, int blue)
{
    Uint32 *pixel = (Uint32*)Surf->pixels;

    int current_column;

    if(end > Surf->h)
        end = Surf->h - 1;

    int limit = Surf->w*Surf->h;

    // goes to desired start point
    int offset = Surf->w*start + column;

    for (int j = 0; j < end-start; ++j)
    {
        int index = offset + j*Surf->w;
        if(index > limit)
        {
            index = limit;
        }

        pixel[index] = SDL_MapRGB(Surf->format, red, green, blue);
    }
}

void Brizoler::Rect(int left_cornerX, int left_cornerY, int width, int height, int red, int green, int blue)
{
    PaintLine(left_cornerY, left_cornerX, left_cornerX + width, red, green, blue);            //  -----
    PaintColumn(left_cornerX + width, left_cornerY, left_cornerY + height, red, green, blue); //       |
    PaintLine(left_cornerY + height, left_cornerX, left_cornerX + width, red, green, blue);   //  ______
    PaintColumn(left_cornerX, left_cornerY, left_cornerY + height, red, green, blue);         // |
}

void Brizoler::BlackScreen()
{

    Uint32 *pixel;
    pixel = (Uint32*)Surf->pixels;


    for (int i = 0; i < Surf->h; ++i)
    {
        for (int j = 0; j < Surf->w; ++j)
        {
            *pixel = SDL_MapRGB(Surf->format, 0, 0, 0);
            pixel++;
        }
    }
}

void Brizoler::LinesGettingSmaller()
{
    int current_line_height = 0;
    int current_line_width = Surf->w;

    int num_lines = 20;

    float space_between_lines = (float) this->mouse_y/num_lines;
    float width_decrease = ((float) current_line_width)/(2*num_lines);

    float width_delta1 = (float) this->mouse_x/num_lines;
    float width_delta2 = (float) (Surf->w - this->mouse_x)/num_lines;

    float height_delta1 = (float) this->mouse_y/num_lines;
    float height_delta2 = (float) (Surf->h - this->mouse_y)/num_lines;

    for(int i = 0;current_line_height <= this->mouse_y &&
                 current_line_height <= Surf->h &&
                  current_line_width > 0 &&
                  space_between_lines > 1; i++ )
    {
        PaintLine(current_line_height, (int)(i*width_delta1), Surf->w - (int)(i*width_delta2), 255, 255, 255);
        PaintColumn((int)(i*width_delta1), current_line_height, Surf->h - (int)(i*height_delta2), 255, 255, 255);

        current_line_height += (int) space_between_lines;
    }

}

void Brizoler::InfiniteSquares(int num_squares)
{
    static Uint8 red_offset = 0;
    static Uint8 green_offset = 0;
    static Uint8 blue_offset = 0;

    red_offset += 2;
    green_offset += 3;
    blue_offset += 5;


    float horizontal_step = ((float) mouse_x)/num_squares; // distance between squares
    float vertical_step = ((float) mouse_y)/num_squares;
    float width_decrease_step = ((float) Surf->w)/num_squares; // square lines decreasing rate
    float height_decrease_step = ((float) Surf->h)/num_squares;

    int square_x = 0;
    int square_y = 0;
    int square_width = Surf->w;
    int square_height = Surf->h;

    for(int i = 0; i < num_squares; i++)
    {
        Rect(square_x, square_y, square_width, square_height, ((Uint8)red_offset*i), ((Uint8)green_offset*i), ((Uint8)blue_offset*i));

        square_x += horizontal_step ;
        square_y += vertical_step ;
        square_width -= width_decrease_step ;
        square_height -=  height_decrease_step;
    }

}


void Brizoler::OnPaint(int OffsetX, int OffsetY)
{
    if(SDL_MUSTLOCK(Surf)) {
        SDL_LockSurface(Surf);
    }

    Uint8 red;
    Uint8 green;
    Uint8 blue;

    int bytes_per_pixel = Surf->format->BytesPerPixel;
    Uint32 *pixel;
    pixel = (Uint32*)Surf->pixels;


    int old_func_x = FunctionPoint(0);

            // float i_norm = (float) i/Surf->h;
            // float j_norm = (float) j/Surf->w;
    int prod;
    switch(mode)
    {
        case 1:
        {

            for (int i = 0; i < Surf->h; ++i)
            {
                for (int j = 0; j < Surf->w; ++j)
                {

                    prod = (int)((i*i + j*j)*param_zoom);

                    red = (prod + OffsetX)%256;
                    green = (prod + (int)(param_speed*OffsetY))%256;
                    blue = (prod)%256;


                    *pixel = SDL_MapRGB(Surf->format, red, green, blue);
                    pixel++;
                }
            }


        }break;
        case 2:
        {

            for (int i = 0; i < Surf->h; ++i)
            {
                for (int j = 0; j < Surf->w; ++j)
                {

                    prod = (int)((i*(j+OffsetY))*param_zoom);
                    red = (prod + OffsetX)%256;
                    green = (prod + (int)(param_speed*OffsetY))%256;
                    blue = (prod)%256;

                    *pixel = SDL_MapRGB(Surf->format, red, green, blue);
                    pixel++;
                }
            }


        }break;
        case 3:
        {

            for (int i = 0; i < Surf->h; ++i)
            {
                for (int j = 0; j < Surf->w; ++j)
                {

                    if(i%2 != 0)
                        prod = (int)((i*i*i + j*j*j)*param_zoom);
                    else
                        prod = (int)((i*(j+0))*param_zoom);
                    red = (prod + OffsetX)%256;
                    green = (prod + (int)(param_speed*OffsetY))%256;
                    blue = (prod)%256;

                    *pixel = SDL_MapRGB(Surf->format, red, green, blue);
                    pixel++;
                }
            }


        }break;
        case 4:
        {

            for (int i = 0; i < Surf->h; ++i)
            {
                for (int j = 0; j < Surf->w; ++j)
                {

                    int func_x = FunctionPoint(i);
                    if(j == func_x)
                        prod = 1;
                    else
                        prod = 0;

                    red = prod * 255;
                    green = prod * 255;
                    blue = prod * 255;

                    *pixel = SDL_MapRGB(Surf->format, red, green, blue);
                    pixel++;
                }
            }


        }break;
        case 5:
        {
            static int num = 40;
            static int sin_phase = 0;
            static int change_counter = 0;

            // if(change_counter++ == 10)
            // {
            //     num = (int)(15 + 14*sin(sin_phase++));
            // }


            // BlackScreen();
            InfiniteSquares(num);
        }break;

        default:
        {
            BlackScreen();
            LinesGettingSmaller();
        }break;
    }

    if(SDL_MUSTLOCK(Surf))
    {
        SDL_UnlockSurface(Surf);
    }
    SDL_UpdateRect(Surf, 0, 0, Surf->w, Surf->h);
}