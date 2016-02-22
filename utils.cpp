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

/*
hue in [0, 360]
saturation in [0, 1]
luminance in [0, 1]

red, green and blue [0, 255]

*/
void HSLtoRGB(float hue/*degree*/, float saturation, float luminance, unsigned char &r, unsigned char &g, unsigned char &b)
{
    if(fabs(saturation) < 0.1)
    {
        r = luminance*255;
        g = luminance*255;
        b = luminance*255;
    }
    else
    {
        float temp1;

        if(luminance < 0.5)
        {
            temp1 = luminance*(1+saturation);
        }
        else
        {
            temp1 = luminance + saturation - (luminance*saturation);
        }

        float temp2;

        temp2 = 2*luminance - temp1;


        float normHue = hue/360.f;

        float tempR = normHue + 0.333;
        float tempG = normHue;
        float tempB = normHue - 0.333;

        float normR;
        float normG;
        float normB;


        if(6*tempR < 1){normR = temp2 + (temp1-temp2)*6*tempR;}
        else if(2*tempR < 1){normR = temp1;}
        else if(3*tempR < 2){normR = temp2 + (temp1-temp2)*6*(0.666 - tempR);}
        else{normR = temp2;}

        if(6*tempG < 1){normG = temp2 + (temp1-temp2)*6*tempG;}
        else if(2*tempG < 1){normG = temp1;}
        else if(3*tempG < 2){normG = temp2 + (temp1-temp2)*6*(0.666 - tempG);}
        else{normG = temp2;}

        if(6*tempB < 1){normB = temp2 + (temp1-temp2)*6*tempB;}
        else if(2*tempB < 1){normB = temp1;}
        else if(3*tempB < 2){normB = temp2 + (temp1-temp2)*6*(0.666 - tempB);}
        else{normB = temp2;}

        r = normR*255;
        g = normG*255;
        b = normB*255;

    }

}