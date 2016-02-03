#ifndef _PLOTTER_H_
#define _PLOTTER_H_

#include <vector>
#include "Window.h"
#include "Doodle.h"
// #include <iostream>

// using namespace std;

#define ZOOM_LIMIT 20
#define SPEED_LIMIT 20

class Plotter : public Doodle
{
public:
    float paramZoom;
    float paramSpeed;

    int mode;

    Plotter();
    ~Plotter();

    bool Initialize(InputHandler* input, Window* window);

    void InputRefresh();

    void Run();

    float CalculateFunc(float x);

    float CalculatePolarFunc(float angle);

    void PlotCartesian();

    void PlotPolar();

};

#endif