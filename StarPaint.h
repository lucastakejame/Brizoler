#ifndef _STARPAINT_H_
#define _STARPAINT_H_

#include "Star.h"
#include "Doodle.h"
#include "utils.h"
#include <vector>

class StarPaint : public Doodle
{
public:
    int mode;
    vector<Star> stars;

    SDL_Surface* previewCanvas;
    SDL_Surface* drawCanvas;

    bool painting;
    bool gravitateMouse;
    bool bounce;

    bool moveCanvas;

    bool boostVel;
    bool shrinkVel;

    int mX;
    int mY;

    int canvasLimitX1;
    int canvasLimitX2;
    int canvasLimitY1;
    int canvasLimitY2;

    StarPaint();
    ~StarPaint();

    bool Initialize(Window* window, InputHandler* input);

    void InputRefresh();

    void Run();

    void Draw(int idx);

    void Show(int idx);

    void BigBang(int x, int y);
};

#endif