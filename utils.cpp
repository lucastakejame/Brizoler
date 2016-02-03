#include "utils.h"

void KILL_DENORMAL_BY_QUANTIZATION(float &val)
{
    static const float anti_denormal = 1e-18;
    val += anti_denormal;
    val -= anti_denormal;
}

void rotateVector(float& x, float& y, float rad)
{
    float temp_x = x;
    float temp_y = y;
    float cos_rad = cos(rad);
    float sin_rad = sin(rad);

    x = temp_x*cos_rad - temp_y*sin_rad;
    y = temp_x*sin_rad - temp_y*cos_rad;
}

void normalizeVector(float& x, float& y)
{
    if( isnormal(x) && isnormal(y))
    {
        float module = sqrt(x*x+y*y);
        x /= module;
        y /= module;

    }
    else if(x == 0 && y != 0)
    {
        y /= fabs(y);
    }
    else if(y == 0 && x != 0)
    {
        x /= fabs(x);
    }
}