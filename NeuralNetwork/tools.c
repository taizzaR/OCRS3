#include <stdio.h>

// Random(): returns a random double in [-1; 1]

double Random()
{
    return ((double)rand()) / ((double)RAND_MAX / 2) - 1;
}

// Sigmoid(double x): the classical sigmoid function

double Sigmoid(double x):
{
    return (1.0 / (1.0 + exp(-x)));
}
