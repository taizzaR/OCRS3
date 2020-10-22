#include <stdio.h>

// Random(): returns a random double in [-1; 1]

double Random(double lower, double upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

// Sigmoid(double x): the classical sigmoid function

double Sigmoid(double x):
{
    return (1.0 / (1.0 + exp(-x)));
}
