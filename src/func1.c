#include <stdio.h>
#include "func1.h"

int f_dimension()
{
    return 2;
}

double f_value(const double x[], const Sample samples[], int count)
{
    double E = 0.0;
    double a = x[0], b = x[1];

    for (int i = 0; i < count; ++i){
        double diff = samples[i].temp - (a * samples[i].alt + b);
        E += diff * diff;
    }
    
    return E;
}

void f_gradient(const double x[], const Sample samples[], int count, double g[])
{
    double a = x[0], b = x[1];
    g[0] = 0.0;
    g[1] = 0.0;

    for (int i = 0; i < count; ++i){
        double diff = samples[i].temp - (a * samples[i].alt + b);
        g[0] += -2.0 * samples[i].alt * diff;
        g[1] += -2.0 * diff;  
    }
}

int compare_altitude(const void *a, const void *b){
    const Sample *sample_a = (const Sample*)a;
    const Sample *sample_b = (const Sample*)b;
    // 標高の降順でソート
    return (sample_b->alt > sample_a->alt) - (sample_a->alt > sample_b->alt);
}

void print_locations(const Sample samples[], int count){
    printf("Location        Altitude(km)  Temperature(°C)\n");
    printf("-------------  ------------  ---------------\n");
    for (int i = 0; i < count; ++i){
        printf("%-14s %11.3f %14.1f\n",
                samples[i].loc,
                samples[i].alt,
                samples[i].temp);
    }
    printf("\n");
}