#include "StarPaint.h"

StarPaint::StarPaint()
{
    mode = 1;

    mX = 0;
    mY = 0;

    boostVel = false;
    shrinkVel = false;
    bounce = false;
    painting = false;

    moveCanvas = false;

    int numStars = 2000;

    for (int i = 0; i < numStars; ++i)
    {
        stars.push_back(Star());
    }
}

StarPaint::~StarPaint(){}


bool StarPaint::Initialize(Window* window, InputHandler* input)
{
    Doodle::Initialize(window, input);

    int wW = 200;
    int wH = 200;
    // int wW = window->GetW();
    // int wH = window->GetH();


    canvasLimitX1 = 0 - wW;
    canvasLimitX2 = wW-1 + wW;
    canvasLimitY1 = 0 - wH;
    canvasLimitY2 = wH-1 + wH;

    BigBang(wW/2, wH/2);
}

void StarPaint::InputRefresh()
{
    float delta = 0.1;

    if(input->keystate[SDL_SCANCODE_1]) {mode = 1; }
    if(input->keystate[SDL_SCANCODE_2]) {mode = 2; }
    if(input->keystate[SDL_SCANCODE_3]) {mode = 3; }

    mX = input->mX;
    mY = input->mY;

    gravitateMouse = input->mL ? true : false;

    if(input->keystate[SDL_SCANCODE_B])
    {
        BigBang(mX, mY);
    }
    if(input->keystate[SDL_SCANCODE_M])
    {
        moveCanvas ^= 1;
    }
    if(input->keystate[SDL_SCANCODE_R])
    {
        //resize canvas
        canvasLimitX1 = 0;
        canvasLimitX2 = mX;
        canvasLimitY1 = 0;
        canvasLimitY2 = mY;
    }

    boostVel = (input->keystate[SDL_SCANCODE_UP]) ? true:false;
    shrinkVel = (input->keystate[SDL_SCANCODE_DOWN]) ? true:false;

    if(input->keystate[SDL_SCANCODE_RIGHT])
    {
        bounce = true;
    }
    if(input->keystate[SDL_SCANCODE_LEFT])
    {
        bounce = false;
    }

}

void StarPaint::Run()
{
    static int count = 1;
    static int count2 = 50;

    static float max_accel = 0;
    static float min_accel = 999999;
    static float max_radius_x = 0;
    static float min_radius_x = 999999;
    static float max_radius_y = 0;
    static float min_radius_y = 999999;

    if(moveCanvas)
    {
        int tempW = canvasLimitX2 - canvasLimitX1;
        int tempH = canvasLimitY2 - canvasLimitY1;

        canvasLimitX1 = mX - tempW/2;
        canvasLimitX2 = mX + tempW/2;
        canvasLimitY1 = mY - tempH/2;
        canvasLimitY2 = mY + tempH/2;
    }

    for(uint i = 0; i < stars.size(); i++)
    {
        if(gravitateMouse)
        {
            float starX = stars[i].pX;
            float starY = stars[i].pY;
            float radiusX = mX - starX; // vector in the line that pass through each planets center
            float radiusY = mY - starY;

            float distSq = radiusX*radiusX + radiusY*radiusY;
            float dist = sqrt(distSq);

            normalizeVector(radiusX, radiusY);

            float G = 0.1;
            float mouse_mass = 0.5;
            float mouseRadius = 15;
            float accel;

/*            if(0)
            // if(dist <= mouseRadius)
            {
                // idea here is to make the circle around the mouse simulate a surface.
                // so when a star touches the surface, it bounces with the out_angle = in_angle

                // if(stars[i].velX*radiusX + stars[i].velY*radiusY > 0) // dot product
                if(0) // dot product
                {
                    float in_angle = atan2(-radiusY, -radiusX);

                    rotate_vector(stars[i].velX, stars[i].velY, (M_PI)- 2*in_angle);
                    stars[i].velX *= 0.9;
                    stars[i].velY *= 0.9;
                    accel = 0.01;
                }
                else
                {
                    accel = -0.1;
                }
            }*/
            // accel = G*mouse_mass*dist/10;
            // accel = 1000*G*mouse_mass*(dist-mouseRadius)/(distSq);
            // accel = 1000*G*mouse_mass/(distSq);
            accel = G*mouse_mass*10; /// this way is more interesting

            // CHANGE_MIN_MAX(accel, min_accel, max_accel)
            // CHANGE_MIN_MAX(radiusX, min_radius_x, max_radius_x)
            // CHANGE_MIN_MAX(radiusY, min_radius_y, max_radius_y)

            if((isnormal(accel)) // accel was generating nan and radius inf
                && (isnormal(radiusX) || radiusX == 0.0)
                && (isnormal(radiusY) || radiusY == 0.0))
            {
                stars[i].accelX = accel*radiusX;
                stars[i].accelY = accel*radiusY;
            }

            if(dist <= mouseRadius)
            {
                // stars[i].SetColor(255, 0, 0);
            }
            else if(1)
            // else if(accel < 5)
            {
                // stars[i].SetColor(radiusX*radiusX - dist, radiusY*radiusY - dist, dist);
                // stars[i].SetColor((accel)*(1), (accel)*(1), (accel)*(1));
                // stars[i].SetColor(stars[i].velX, stars[i].velY, stars[i].velX*stars[i].velY);
                // stars[i].SetColor(accel+stars[i].velX, accel+stars[i].velY, accel*stars[i].velX*stars[i].velY);
                // static int color_counter = 0;
                // color_counter++;
                // if(color_counter % 10 == 0)
                //     stars[i].SetColor(random()%256, random()%256, random()%256);
            }
            else
            {
                stars[i].SetColor(255, 255, 255);
            }
        }
        else
        {
            stars[i].accelX = 0;
            stars[i].accelY = 0;
        }

        if(boostVel)
        {
            stars[i].velX *= 1.1;
            stars[i].velY *= 1.1;
        }
        if(shrinkVel)
        {
            stars[i].velX /= 1.1;
            stars[i].velY /= 1.1;
        }

        stars[i].Move();

        if(bounce)
        {
            if(stars[i].pX < canvasLimitX1)
            {
                stars[i].pX = canvasLimitX1;
                stars[i].lastpX = canvasLimitX1;
                stars[i].velX *= -1;
            }
            else if(stars[i].pX > canvasLimitX2)
            {
                stars[i].pX = canvasLimitX2;
                stars[i].lastpX = canvasLimitX2;
                stars[i].velX *= -1;
            }

            if(stars[i].pY < canvasLimitY1)
            {
                stars[i].pY = canvasLimitY1;
                stars[i].lastpY = canvasLimitY1;
                stars[i].velY *= -1;
            }
            else if(stars[i].pY > canvasLimitY2)
            {
                stars[i].pY = canvasLimitY2;
                stars[i].lastpY = canvasLimitY2;
                stars[i].velY *= -1;
            }
        }

        Draw(i);

        // if(count % count2 == 0)
        // if(stars_change_direction_flag)
        // {
            // stars[i].Change_Direction();
        // }
        // count2 = (int) 50 + 25*sin(count);
    }



        // count++;
    // stars_change_direction_flag = 0;

    // fprintf(stderr, "Accel: [%g,%g]\n", min_accel, max_accel);
    // fprintf(stderr, "Radius_x: [%g,%g]\n", min_radius_x, max_radius_x);
    // fprintf(stderr, "Radius_y: [%g,%g]\n", min_radius_y, max_radius_y);

}

void StarPaint::Draw(int idx)
{

    float dx = (stars[idx].pX - stars[idx].lastpX);
    float dy = (stars[idx].pY - stars[idx].lastpY);


    normalizeVector(dx, dy);

    if(stars[idx].pX != stars[idx].lastpX
    && stars[idx].pY != stars[idx].lastpY)
    {
        int count = 0;
        for (float i = stars[idx].lastpX, j = stars[idx].lastpY;
        (dx != 0 && dy != 0)
        && (((stars[idx].pX - i)*dx >= 0)
        && ((stars[idx].pY - j)*dy >= 0))
        // && count < 100
        ;
        i += dx, j += dy, ++count)
        {

            for (int jj = (int)j-1; jj < (int)j+2; ++jj)
            {
                for (int ii = (int)i-1; ii < (int)i+2; ++ii)
                {
                    SDL_Color c = stars[idx].sprite[jj - (int)j + 1][ii - (int)i + 1];
                    window->DrawPixel(ii, jj, c.r, c.g, c.b);
                }
            }
        }
    }
    else
    {
        // window->DrawPixel(stars[idx].pX-1, stars[idx].pY-1, red, green, blue);
        // window->DrawPixel(stars[idx].pX, stars[idx].pY-1, red, 255, 255);
        // window->DrawPixel(stars[idx].pX+1, stars[idx].pY-1, red, green, blue);

        // window->DrawPixel(stars[idx].pX-1, stars[idx].pY, 255, green, 255);
        // // window->DrawPixel(stars[idx].pX, stars[idx].pY, red, green, blue);
        // window->DrawPixel(stars[idx].pX, stars[idx].pY, 255, 255, 255);
        // window->DrawPixel(stars[idx].pX+1, stars[idx].pY, 255, 255, blue);

        // window->DrawPixel(stars[idx].pX-1, stars[idx].pY+1, red, green, blue);
        // window->DrawPixel(stars[idx].pX, stars[idx].pY+1, 255, 255, 255);
        // window->DrawPixel(stars[idx].pX+1, stars[idx].pY+1, red, green, blue);

    }
}

void StarPaint::BigBang(int x, int y)
{
    for (int i = 0; i < stars.size(); ++i)
    {
        stars[i].pX = x;
        stars[i].pY = y;

        stars[i].lastpX = x;
        stars[i].lastpY = y;

        stars[i].velX = float(random()%60 - 30)/10;
        stars[i].velY = float(random()%60 - 30)/10;

        stars[i].accelX = 0;
        stars[i].accelY = 0;

        stars[i].SetColor(random()%256, 2*random()%256, 3*random()%256);
    }

}