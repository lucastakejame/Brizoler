#ifndef _STAR_H_
#define _STAR_H_

#include <SDL2/SDL.h>
#include <iostream>
#include "CEvent.h"
#include "stdio.h"
#include "math.h"
#include "Window.h"

using namespace std;

#define SPRITE_SIZE 3

struct Star
{
    SDL_Color sprite[SPRITE_SIZE][SPRITE_SIZE];

    unsigned char red;
    unsigned char green;
    unsigned char blue;

    float pX;
    float pY;

    float lastpX;
    float lastpY;

    float velX;
    float velY;

    float accelX;
    float accelY;

    float dumping;

    Star();
    ~Star();

    void Move();

    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

};

#endif