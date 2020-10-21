#include <stdio.h>
#include "tools.h"
#include <math.h>

struct Neuron{
	double output;
	int weightsLength;
	double bias;
	double delta //Je sais pas trop ce que cest
	double inputs[2];
	double weights[2];
};

struct Neural_Network Initialization()
{
    struct Neural_Network network;
    network.biais0 = Random();
}

double init_weight() {return ((double)rand())/((double)RAND_MAX);

double xor(double x, double y):
	if (x == y){
		return 0;
	}
	return 1;
}
