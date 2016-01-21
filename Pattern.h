#ifndef _PATTERN_H_
#define _PATTERN_H_

#include "Doodle.h"

#define ZOOM_LIMIT 20
#define SPEED_LIMIT 20

class Pattern : public Doodle
{
public:
    float paramZoom;
    float paramSpeed;

    Pattern();
    ~Pattern();

    bool Initialize(InputHandler* input, Window* window);

    void InputRefresh();

    void Run();
};

#endif