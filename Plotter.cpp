#include "Plotter.h"

Plotter::Plotter()
{
    paramZoom = 11;
    paramSpeed = 11;
    mode = 1;
}

Plotter::~Plotter(){}

void Plotter::InputRefresh()
{
    float delta = 0.1;

    if(input->keystate[SDL_SCANCODE_1]) {mode = 1; }
    if(input->keystate[SDL_SCANCODE_2]) {mode = 2; }

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

void Plotter::Run()
{
    switch(mode)
    {
        case 1:
        PlotCartesian();
        break;
        case 2:
        PlotPolar();
        break;
        default:;
    }
}

void Plotter::PlotCartesian()
{
    window->FillCanvas(0, 0, 0);

    vector<Point> funcPts;
    vector<Point>::iterator itPts;

    // Centering (0,0) and adjusting borders
    float zeroX = (float)window->GetW()/2;
    float zeroY = (float)window->GetH()/2;
    float cartYMin = (float)-window->GetH()/2;
    float cartYMax = (float)window->GetH()/2;

    window->DrawLine(0, zeroY, window->GetW(), zeroY, 1, 125, 125, 125);
    window->DrawLine(zeroX, 0, zeroX, window->GetH(), 1, 125, 125, 125);

    for (int x = 0; x < window->GetW(); x++)
    {
        float cartX = -zeroX+x;
        float cartY = CalculateFunc(cartX);

        float y = zeroY - cartY;

        // if(cartY <= cartYMax && cartY >= cartYMin)
        {
            funcPts.push_back(Point(x,y));
        }

    }

    int count = 0;
    for(itPts = funcPts.begin(); itPts < funcPts.end()-1; itPts++)
    {
        Point ptsCurrent = *itPts;
        Point ptsNext = *(itPts+1);

        // if(ptsNext.x == ptsCurrent.x+1) // this is valid for cartesian ploting
        {

            window->DrawLine(ptsCurrent.x, ptsCurrent.y, ptsNext.x, ptsNext.y, 1, 255, 255, 255);
            // PaintLine(ptsCurrent.x, ptsCurrent.y, ptsNext.x, ptsNext.y, 1, sin(ptsCurrent.x)*255, cos(ptsCurrent.x)*255, tan(ptsCurrent.x)*255);
            // PaintLine(ptsCurrent.x, ptsCurrent.y, ptsNext.x, ptsNext.y, 1, ((sin(persistInc*M_PI)+1)/2)*255, (ptsCurrent.y)*255, (ptsCurrent.x * ptsCurrent.y)*255);
        }
        count++;
    }
    return;

}

float Plotter::CalculateFunc(float x)
{
    // float result = paramSpeed*90000 - x*x + 10;
    // if(result < 0)
    // {
    //     return 10*x/fabs(x);
    // }
    // else
    // {
    //     return sqrt(result);
    // }

    static float offset = 0;
    static float inc = 0.001;
    static float persistInc = 0;

    offset += inc;
    float incLimit = 100*inc;

    if(inc > 0)
    {
        if(persistInc <= incLimit)
            persistInc += inc;
        else
            inc = -inc;
    }
    else
    {
        if(persistInc >= 0)
            persistInc += inc;
        else
            inc = -inc;
    }

    float paramX = 2*paramSpeed/SPEED_LIMIT;
    float paramY = -0.5 + paramZoom/ZOOM_LIMIT;

    const int amplitude = 400;
    const int maxFreq = 200;

    return (paramX)*x*x + -(window->GetH()/2)+(window->GetH())*(paramY);
    // return amplitude*sin(paramX*maxFreq*(x/window->GetW())*2*M_PI+ offset) + (paramY*(window->GetH()-amplitude*2));
    // return amplitude*tan(paramX*maxFreq*(x/window->GetW())*2*M_PI+ offset) + (paramY*(window->GetH()-amplitude*2));
    // return 400*(paramY)*sin((x/window->GetW())*2*M_PI*100*(paramX));
}

float Plotter::CalculatePolarFunc(float angle)
{
    static float inc = 0.001;
    static float persistInc = 0;

    float incLimit = 2*M_PI;

    if(inc > 0)
    {
        if(persistInc <= incLimit)
            persistInc += inc;
        else
            persistInc = 0;
            // inc = -inc;
    }
    else
    {
        if(persistInc >= 0)
            persistInc += inc;
        else
            inc = -inc;
    }
    // return (5*angle + angle*tan(angle*10 + 10*(paramZoom/ZOOM_LIMIT)*persistInc));
    return 2*(5*angle + angle*sin(angle*10 + 10*(paramZoom/ZOOM_LIMIT)*persistInc))*sin(angle*10 + 10*(paramZoom/ZOOM_LIMIT)*persistInc);
}

void Plotter::PlotPolar() // in the form of y = 3x + 4
{
    window->FillCanvas(0, 0, 0);

    vector<Point> plotPts;
    vector<Point> funcPts;
    vector<Point>::iterator itPts;

    float zeroX = (float)window->GetW()/2;
    float zeroY = (float)window->GetH()/2;

    float cartYMin = (float)-window->GetH()/2;
    float cartYMax = (float)window->GetH()/2;

    float cartXMin = (float)-window->GetW()/2;
    float cartXMax = (float)window->GetW()/2;

    window->DrawLine(0, zeroY, window->GetW(), zeroY, 1, 125, 125, 125);
    window->DrawLine(zeroX, 0, zeroX, window->GetH(), 1, 125, 125, 125);

    for (float angle = 0; angle <= 5*2*M_PI;)
    {
        float deltaAngle = 0.01;

        float radius = CalculatePolarFunc(angle);

        float cartX= radius*cos(angle);
        float cartY= radius*sin(angle);

        // if(cartY<= cartYMax
        //     && cartY>= cartYMin
        //     && cartX<= cartXMax
        //     && cartX>= cartXMin)
        {
            float x = -cartX+ zeroX;
            float y = zeroY - cartY;

            funcPts.push_back(Point(x,y));
        }

        angle += deltaAngle;
        if(deltaAngle > 0.0001)
            deltaAngle -= 0.0001;

    }

    int count = 0;
    float jumps = 100.0f*(float)paramSpeed/SPEED_LIMIT + 1;
    for(itPts = funcPts.begin(); itPts < funcPts.end()-1; itPts++)
    {
        if(1)
        // if(count % (int)jumps == 0)
        {
            plotPts.push_back(*itPts);
        }

        count ++;
    }

    for(itPts = plotPts.begin(); itPts < plotPts.end()-1; itPts++)
    {
        Point ptsCurrent = *itPts;
        Point ptsNext = *(itPts+1);

        // if(ptsNext.x == ptsCurrent.x+1) // this is valid for cartesian ploting
        {
            // PaintLine(ptsCurrent.x, ptsCurrent.y, ptsNext.x, ptsNext.y, 1, 255, 255, 255);
            window->DrawLine(ptsCurrent.x, ptsCurrent.y, ptsNext.x, ptsNext.y, 1, sin(ptsCurrent.x)*255, cos(ptsCurrent.x)*255, tan(ptsCurrent.x)*255);
            // PaintLine(ptsCurrent.x, ptsCurrent.y, ptsNext.x, ptsNext.y, 1, ((sin(persistInc*M_PI)+1)/2)*255, (ptsCurrent.y)*255, (ptsCurrent.x * ptsCurrent.y)*255);
        }
    }


    return;
}