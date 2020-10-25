#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Random(): returns a random double in [-1; 1]

double Random(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

double Relu(double x)
{
    return x > 0 ? x : 0;
}

double ReluDerivative(double x)
{
    return x > 0 ? 1 : 0;
}

double xor(double x, double y)
{
    return x == y ? 0 : 1;
}