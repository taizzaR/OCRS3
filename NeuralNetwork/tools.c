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


// Activation(double a, double output, double bias0) : return the quantity to add to the weights

double Activation(double a, double output, double bias0)
{
    return a*(output + bias0);
}
