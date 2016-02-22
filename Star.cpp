#include "Star.h"

Star::Star()
{
    pX = 0;
    pY = 0;

    lastpX = 0;
    lastpY = 0;

    velX = 0;
    velY = 0;

    accelX = 0;
    accelY = 0;

    dumping = 0;

    for (int j = 0; j < SPRITE_SIZE; ++j)
    {
        for (int i = 0; i < SPRITE_SIZE; ++i)
        {
            this->sprite[j][i].r = 0;
            this->sprite[j][i].g = 0;
            this->sprite[j][i].b = 0;
            this->sprite[j][i].a = 255;
        }
    }

}

Star::~Star(){}

void Star::Move()
{
    if((isnormal(accelX) || accelX == 0)
    && (isnormal(accelY) || accelY == 0))
    {
        velX += accelX*(1.f-dumping);
        velY += accelY*(1.f-dumping);

        if((isnormal(pX) || pX == 0)
        && (isnormal(pY) || pY == 0))
        {
            lastpX = pX;
            lastpY = pY;
            pX += velX;
            pY += velY;
        }
    }
}


void Star::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;

    // Sets color in an arbitrary way.
    this->sprite[0][0] = {red, green, blue, 255};
    this->sprite[1][0] = {red, 255, 255, 255};
    this->sprite[2][0] = {red, green, blue, 255};

    this->sprite[0][1] = {255, green, 255, 255};
    // this->sprite[1][1] = {red, green, blue, 255};
    this->sprite[1][1] = {255, 255, 255, 255};
    this->sprite[2][1] = {255, 255, blue, 255};

    this->sprite[0][2] = {red, green, blue, 255};
    this->sprite[1][2] = {255, 255, 255, 255};
    this->sprite[2][2] = {red, green, blue, 255};

/*    // Sets color raw.
    this->sprite[0][0] = {red, green, blue, 255};
    this->sprite[1][0] = {red, green, blue, 255};
    this->sprite[2][0] = {red, green, blue, 255};

    this->sprite[0][1] = {red, green, blue, 255};
    this->sprite[1][1] = {red, green, blue, 255};
    this->sprite[2][1] = {red, green, blue, 255};

    this->sprite[0][2] = {red, green, blue, 255};
    this->sprite[1][2] = {red, green, blue, 255};
    this->sprite[2][2] = {red, green, blue, 255};*/

}