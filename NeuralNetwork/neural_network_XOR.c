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
    foreach (struct Neuron layer in network.layers)
    {
        foreach (struct Neuron neuron in layer)
        {
            foreach (double weight in neuron.weights)
            {
                weight = Random(-0.5, 0.5);
            }

            neuron.bias = Random(0, 1);
        }
    }

    return network;
}


double xor(double x, double y):
{
    return x == y ? 0 : 1;
}

struct NeuralNetwork Forward(struct NeuralNetwork Neural, double (*ac) (double)){
    int i,j,k = 0
    double ac = 0;
    for (i = 0; i < Neural.layernumbers; i++)
    {
        for (for j = 0,; j < Neural.layerLength[i]; j++){
	    	if (i != 0){
				for (k = 0; Neural.layers[i][j].weightsLength; k++){
		    		Neural.layers[i][j].inputs[k] = Neural.layers[i-1][j].output[k];	
				}
	    	}
	    	for (k = 0; k < Neural.layers[i][j].weightLength; k++){
				y += (Neural.layers[i][j].inputs[k] * Neural.layers[i][j].weights[k]);
			}
	    	y += Neural[i][j].bias;
	    	y = (*ac)(y);
	    	Neural.layers[i][j].output = y;
		}
    }
    return Neural;
};

struct NeuralNetwork Train(struct NeuralNetwork Neural){};

struct NeuralNetwork Input(struct NeuralNetwork network, double input[])
{
	foreach (struct Neuron neuron in network.layers[0])
	{
		for(int i = 0; i < inputsLength; i++)
		{
			neuron = input[i];
		}
	}

	return network;
}

struct NeuralNetwork Backward(struct NeuralNetwork Neural){};

struct NeuralNetwork UpdateWeights(struct NeuralNetwork Neural){};




