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

struct NeuralNetwork{
	struct Neuron layers[2][2];
	int layersNumber;
	int inputsLength; // 2
	int layerLength[2]; // {2,1};
	double learningRate;
}

struct Dataset{
	double inputs[4][2]; // {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double outputs[4][1]; // {{0}, {1}, {1}, {0}};
	int datasetLength; // 2;
}

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

struct NeuralNetwork Train(struct NeuralNetwork Neural){};

struct NeuralNetwork Input(struct NeuralNetwork Neural){};

struct NeuralNetwork Backward(struct NeuralNetwork Neural){};

struct NeuralNetwork Forward(struct NeuralNetwork Neural){};

struct NeuralNetwork UpdateWeights(struct NeuralNetwork Neural){};


struct NeuralNetwork Train(struct NeuralNetwork Neural){};

struct NeuralNetwork Input(struct NeuralNetwork Neural){};

struct NeuralNetwork Backward(struct NeuralNetwork Neural){};

struct NeuralNetwork Forward(struct NeuralNetwork Neural){};

struct NeuralNetwork UpdateWeights(struct NeuralNetwork Neural){};




