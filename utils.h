#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <cstdio>

using namespace std;

// FILE* log_fd;
// int log_count = 0;

#define cerr(x) cerr << x << endl;
#define cerrv(x) cerr <<  #x << " : " << x << endl;
// #define logg(x) fprintf(log_fd, "%i >>>",log_count);fprintf(log_fd, x);fprintf(log_fd, "\n");
// #define loggv(x) fprintf(log_fd, "%i >>>",log_count);fprintf(log_fd, #x);fprintf(log_fd, " : %2.2f\n", (float) x);

#define CHANGE_MIN_MAX( sample, min, max)   \
{                                           \
    float abs_sample = fabs(sample);        \
    if(abs_sample > (max))                  \
    {                                       \
        (max) = abs_sample;                 \
    }                                       \
    else if(abs_sample < (min))             \
    {                                       \
        (min) = abs_sample;                 \
    }                                       \
}

void KILL_DENORMAL_BY_QUANTIZATION (float &val);

#endif