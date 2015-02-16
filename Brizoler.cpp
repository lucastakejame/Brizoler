#include <stdio.h>
#include <math.h>
#include "Brizoler.h"
// #include "sin.h"

// #define ZOOM_LIMIT 2
#define ZOOM_LIMIT 5000
#define SPEED_LIMIT 20

float attenuator = 1;
float stimulator = 1;
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
    if(Left)
    {
        attenuator = ((float)mY/Surf->h)*ZOOM_LIMIT + 0.01;
        stimulator = ((float)mX/Surf->w)*SPEED_LIMIT + 0.01;
        printf("attenuator : %f \n", attenuator);
        printf("stimulator : %f \n", stimulator);
    }
}


void Brizoler::OnLButtonDown(int mX, int mY)
{
    attenuator = ((float)mY/Surf->h)*ZOOM_LIMIT + 0.01;
    stimulator = ((float)mX/Surf->w)*SPEED_LIMIT + 0.01;
    printf("attenuator : %f \n", attenuator);
    printf("stimulator : %f \n", stimulator);
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
            if(attenuator < ZOOM_LIMIT){
                attenuator += 0.01;
            }
        }
        break;

        case SDLK_DOWN:
        {
            if(attenuator > 0.015){
                attenuator -= 0.01;
            }
        }
        break;

        case SDLK_RIGHT:
        {
            if(stimulator < SPEED_LIMIT){
                stimulator += 0.01;
            }
        }
        break;

        case SDLK_LEFT:
        {
            if(stimulator > 0.015){
                stimulator -= 0.01;
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
    for (int i = 0; i < Surf->h; ++i)
    {
        for (int j = 0; j < Surf->w; ++j)
        {
            // float i_norm = (float) i/Surf->h;
            // float j_norm = (float) j/Surf->w;
            int prod;
            switch(mode)
            {
                case 1:
                {
                    prod = (int)((i*i + j*j)*attenuator);

                }break;
                case 2:
                {
                    prod = (int)((i*(j+OffsetY))*attenuator);
                }break;
                case 3:
                {
                    if(i%2 != 0)
                        prod = (int)((i*i + j*j)*attenuator);
                    else
                        prod = (int)((i*(j+0))*attenuator);
                }break;
                case 4:
                {
                    // if(y == )
                    //     prod = (int)((i*i + j*j)*attenuator);
                    // else
                    //     prod = (int)((i*(j+0))*attenuator);
                }break;
            }

            red = (prod + OffsetX)%256;
            green = (prod + (int)(stimulator*OffsetY))%256;
            blue = (prod)%256;

            *pixel = SDL_MapRGB(Surf->format, red, green, blue);
            pixel++;
        }
    }
    if(SDL_MUSTLOCK(Surf)) {
        SDL_UnlockSurface(Surf);
    }
    SDL_UpdateRect(Surf, 0, 0, Surf->w, Surf->h);
}