#ifndef FUNC1_H
#define FUNC1_H

#define MAX_NAME 32

typedef struct {
    char loc[MAX_NAME];
    double alt;
    double temp;
} Sample;

int f_dimension();
double f_value(const double x[], const Sample samples[], int count);
void f_gradient(const double x[], const Sample samples[], int count, double g[]);
int compare_altitude(const void *a, const void *b);
void print_locations(const Sample samples[], int count);

#endif

