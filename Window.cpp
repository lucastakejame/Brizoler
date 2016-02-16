#include "Window.h"

Window::Window(int w, int h):w(w), h(h)
{
    canvas = SDL_CreateRGBSurface(0,
                                  w, /*width , 0 means any width*/
                                  h, /*height, 0 means any height*/
                                  32, /*bites per pixel, 0 means use current*/
                                  0x00FF0000,
                                  0x0000FF00,
                                  0x000000FF,
                                  0xFF000000);

    SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, &(this->window), &(this->renderer));

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.

    SDL_RenderSetLogicalSize(this->renderer, w, h);

    this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
}

Window::~Window()
{
    SDL_FreeSurface(canvas);
}

void Window::FillCanvas(Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 color = r << 16 | g << 8 | b;
    SDL_FillRect(this->canvas, 0, color);
}

bool Window::DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, SDL_Surface* outCanvas)
{
    bool result = false;
    Uint32 *pixel = (Uint32*)outCanvas->pixels;


    if(SDL_MUSTLOCK(outCanvas))
    {
        SDL_LockSurface(outCanvas);
    }

    if(0 <= x && x < this->w
    && 0 <= y && y < this->h)
    {
        pixel[y*this->w + x] = SDL_MapRGB(outCanvas->format, r, g, b);
        // pixel[y*this->w + x] = 10;
        result = true;
    }

    if(SDL_MUSTLOCK(outCanvas))
    {
        SDL_UnlockSurface(outCanvas);
    }

    return result;
}

bool Window::DrawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    bool result = false;
    Uint32 *pixel = (Uint32*)this->canvas->pixels;


    if(SDL_MUSTLOCK(canvas))
    {
        SDL_LockSurface(canvas);
    }

    if(0 <= x && x < this->w
    && 0 <= y && y < this->h)
    {
        pixel[y*this->w + x] = SDL_MapRGB(canvas->format, r, g, b);
        // pixel[y*this->w + x] = 10;
        result = true;
    }

    if(SDL_MUSTLOCK(canvas))
    {
        SDL_UnlockSurface(canvas);
    }

    return result;
}


void Window::DrawLine(float x1, float y1, float x2, float y2, uint thickness, Uint8 r, Uint8 g, Uint8 b)
{
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

    #if 0
    /////NOTA: usar semelhança de triangulos pra encontrar a intersecção
    if(!IsInsideCanvas(x1,y1) || !IsInsideCanvas(x2,y2))
    {
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

        border_y = this->h;
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

        border_x = this->w;
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
    #endif

    x = x1;
    y = y1;
    xi = (int)x1;
    yi = (int)y1;


    while((x-x2)*dx <= 0 && (y-y2)*dy <= 0)
    {
        DrawPixel(xi, yi, r, g, b);
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

    }
    if(y2 >= 0 && y2 < this->h && x2 >= 0 && x2 < this->w)
    {
        DrawPixel(x2, y2, r, g, b);
    }

}

void Window::RefreshCanvas()
{
    SDL_UpdateTexture(this->texture, NULL, this->canvas->pixels, this->w * sizeof (Uint32));

    SDL_RenderClear(this->renderer);
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    SDL_RenderPresent(this->renderer);
}