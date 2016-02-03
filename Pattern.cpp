#include "Pattern.h"

Pattern::Pattern()
{
    paramZoom = 11;
    paramSpeed = 11;
    mode = 1;
}

Pattern::~Pattern(){}

void Pattern::InputRefresh()
{
    float delta = 0.1;

    if(input->keystate[SDL_SCANCODE_1]) {mode = 1; }
    if(input->keystate[SDL_SCANCODE_2]) {mode = 2; }
    if(input->keystate[SDL_SCANCODE_3]) {mode = 3; }

    if(input->mL)
    {
        paramZoom = ((float)input->mY/window->GetH())*ZOOM_LIMIT + 0.01;
        paramSpeed = ((float)input->mX/window->GetW())*SPEED_LIMIT + 0.01;
    }

    if(input->keystate[SDL_SCANCODE_UP])
    {
        if(input->keystate[SDL_SCANCODE_LCTRL] || input->keystate[SDL_SCANCODE_RCTRL])
        {
            if(paramZoom + delta < ZOOM_LIMIT) {paramZoom += delta; }
        }
        else
        {
            if(paramZoom < ZOOM_LIMIT) {paramZoom += 0.005; }
        }
    }

    if(input->keystate[SDL_SCANCODE_DOWN])
    {
        if(input->keystate[SDL_SCANCODE_LCTRL] || input->keystate[SDL_SCANCODE_RCTRL])
        {
            if(paramZoom - delta > 0) {paramZoom -= delta; }
        }
        else
        {
            if(paramZoom > 0.015) {paramZoom -= 0.005; }
        }
    }

    if(input->keystate[SDL_SCANCODE_RIGHT])
    {
        if(input->keystate[SDL_SCANCODE_LCTRL] || input->keystate[SDL_SCANCODE_RCTRL])
        {
            if(paramSpeed + delta < SPEED_LIMIT) {paramSpeed += delta; }
        }
        else
        {
            if(paramSpeed < SPEED_LIMIT) {paramSpeed += 0.01; }
        }
    }

    if(input->keystate[SDL_SCANCODE_LEFT])
    {
        if(input->keystate[SDL_SCANCODE_LCTRL] || input->keystate[SDL_SCANCODE_RCTRL])
        {
            if(paramSpeed - delta > 0) {paramSpeed -= delta; }
        }
        else
        {
            if(paramSpeed > 0.015) {paramSpeed -= 0.01; }
        }
    }
}

void Pattern::Run()
{
    static int OffsetX = 0;
    static int OffsetY = 0;

    OffsetX++;
    OffsetY++;

    switch(mode)
    {
        case 1:
        {
            for (int y = 0; y < window->GetH(); ++y)
            {
                for (int x = 0; x < window->GetW(); ++x)
                {
                    int prod = (int)((y*y + x*x)*paramZoom);
                    Uint8 r = (prod + OffsetX)%256;
                    Uint8 g = (prod + (int)( paramSpeed*OffsetY ))%256;
                    Uint8 b = (prod)%256;

                    window->DrawPixel(x, y, r, g ,b);
                }
            }

        }break;

        case 2:
        {
            for (int y = 0; y < window->GetH(); ++y)
            {
                for (int x = 0; x < window->GetW(); ++x)
                {
                    int prod = (int)(y*x*paramZoom);
                    Uint8 r = (prod + OffsetX)%256;
                    Uint8 g = (prod + (int)(paramSpeed*OffsetY))%256;
                    Uint8 b = (prod)%256;

                    window->DrawPixel(x, y, r, g, b);
                }
            }

        }break;

        case 3:
        {
            for (int y = 0; y < window->GetH(); ++y)
            {
                for (int x = 0; x < window->GetW(); ++x)
                {
                    int prod;
                    if(y%2 != 0)
                        // prod = (int)(sin((x/window->GetW())*(y/window->GetH()) * 2 * M_PI + 40 )*paramZoom);
                        // prod = (int)((y*y + x*x)*paramZoom);
                        prod = (int)(256*sin(y)*paramZoom);
                    else
                        // prod = (int)(log((window->GetH()*window->GetW() - y*x) +1)*paramZoom*100);
                        prod = (int)(256*cos(x)*paramZoom);
                        // prod = (int)(y*x*paramZoom);

                    Uint8 r = (prod + OffsetX)%256;
                    Uint8 g = (prod + (int)(paramSpeed*OffsetY))%256;
                    Uint8 b = (prod)%256;

                    window->DrawPixel(x, y, r, g, b);
                }
            }
        }break;

        default:
        break;
    }

}