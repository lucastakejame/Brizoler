#include "utils.h"

void KILL_DENORMAL_BY_QUANTIZATION(float &val)
{
    static const float anti_denormal = 1e-18;
    val += anti_denormal;
    val -= anti_denormal;
}
