#include <stdio.h>
#include "tools.h"
#include <math.h>

struct Neuron{
	double output;
	int weightsLength;
	double bias;
	double delta
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

int main()
{
    return 0;
}

struct Neural_Network Initialization(struct NeuralNetwork network)
{
    for (int i = 0; i < network.layersNumber; i++)
    {
        for (int j = 0; j < network.LayerLength; j++)
        {
            for (int k = 0; k < network.layers[i][j].weightsLength; k++)
            {
                network.layers[i][j].weights[j] = Random(-0.5, 0.5);
            }

            network.layers[i][j].bias = Random(0, 1);
        }
    }

    return network;
}


double xor(double x, double y):
{
    return x == y ? 0 : 1;
}

struct NeuralNetwork Train(struct NeuralNetwork Neural){};

struct NeuralNetwork Input(struct NeuralNetwork Neural){};

struct NeuralNetwork Backward(struct NeuralNetwork Neural){};

struct NeuralNetwork Forward(struct NeuralNetwork Neural){};

struct NeuralNetwork UpdateWeights(struct NeuralNetwork Neural){};




