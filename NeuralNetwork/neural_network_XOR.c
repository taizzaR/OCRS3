#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


struct Neuron {         // Structure d'un neurone
	double output;
	int weightsLength;
	double bias;
	double delta;
	double inputs[2];
	double weights[2];
};


struct NeuralNetwork {              // Structure du reseau
	struct Neuron layers[2][2];
	int layerNumber;
	int inputsLength; 
	int layerLength[2]; 
	double learningRate;
};


struct Dataset {                     // Structure des data
	double inputs[4][2]; 
	double outputs[4][1]; 
	int datasetLength; 
};

double Random(double min, double max);
struct NeuralNetwork InitNetwork(struct NeuralNetwork network);
struct NeuralNetwork Train(struct NeuralNetwork network, struct Dataset dataset);
struct NeuralNetwork Input(struct NeuralNetwork network, double input[]);
struct NeuralNetwork Backward(struct NeuralNetwork network, double output[]);
struct NeuralNetwork UpdateWeights(struct NeuralNetwork network, int i, int j);
struct NeuralNetwork Forward(struct NeuralNetwork network, double (*ac)(double));
int PrintXOR(struct NeuralNetwork network);
int PrintNeuron(struct Neuron N, int i, int j);
double Xor(double x, double y);
double Sigmoid(double x);
double SigmoidDerivative(double x);

int main ()         //Programme principal
{	
	struct NeuralNetwork network;
	struct Dataset data;

	network.layerLength[0] = 2;
	network.layerLength[1] = 1;
	network.inputsLength = 2;

	network.layerNumber = 2;
	
	network.learningRate = 0.1;
	
	double _inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double _outputs[4][1] = {{0}, {1}, {1}, {0}};
	
	int i, j;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 2; j++)
		{
			data.inputs[i][j] = _inputs[i][j];
		}
		data.outputs[i][0] = _outputs[i][0];
	}
	data.datasetLength = 4;

	network = InitNetwork(network);
	network = Train(network, data);
	return 0;
}

double Random(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}



struct NeuralNetwork InitNetwork(struct NeuralNetwork network)
{
    int i, j, k;
    for(i = 0; i < network.layerNumber; i++)
    {
        for(j = 0; j < network.layerLength[i]; j++)
        {
			network.layers[i][j].weightsLength = i > 0 ? network.layerLength[i-1] : network.inputsLength;
			for(k = 0; k < network.layers[i][j].weightsLength; k++)
			{
				network.layers[i][j].weights[k] = Random(-0.5, 0.5);
			}
            network.layers[i][j].bias = Random(0, 1);
        }
    }

    return network;
}


struct NeuralNetwork Train(struct NeuralNetwork network, struct Dataset dataset)
{
	int i;
	int test = 0;
	{
		while (test != 1)
		{
			for(i = 0; i < dataset.datasetLength; i++)
			{
				network = Input(network, dataset.inputs[i]);
				network = Forward(network, Sigmoid);
				PrintXOR(network);
				network.layers[1][0].output = network.layers[1][0].bias;
				if (network.layers[1][0].output != Xor(dataset.inputs[i][0], dataset.inputs[i][1]))
					network = Backward(network, dataset.outputs[i]);
				else 
					test = 1;
				PrintXOR(network);
			}
		}

	}
    return network;
}



struct NeuralNetwork Input(struct NeuralNetwork network, double input[])
{
	int j, k;

	for(j = 0; j < network.layerLength[0]; j++)
	{
		for(k = 0; k < network.inputsLength; k++)
		{
			network.layers[0][j].inputs[k] = input[k];
		}
	}

    return network;
}


struct NeuralNetwork Backward(struct NeuralNetwork network, double output[])
{
	int i, j, k;
	for(i = network.layerNumber-1; i >= 0; i--)
	{
		if(i != network.layerNumber-1)
		{
			for(j = 0; j < network.layerLength[i]; j++)
			{
				double error = 0;
				for(k = 0; k < network.layerLength[i+1]; k++)
				{
					error += network.layers[i+1][k].delta * network.layers[i+1][k].weights[j];
				}
				network.layers[i][j].delta = SigmoidDerivative(network.layers[i][j].output) * error;
				network = UpdateWeights(network, i, j);
			}
		}
		else
		{
			for(j = 0; j < network.layerLength[i]; j++)
			{
				network.layers[i][j].delta = SigmoidDerivative(network.layers[i][j].output) * (output[0] - network.layers[i][j].output);
				network = UpdateWeights(network, i, j);
			}
		}
	}
	return network;
}
	

struct NeuralNetwork UpdateWeights(struct NeuralNetwork network, int i, int j)
{
	int k;
	for(k = 0; k < network.layers[i][j].weightsLength; k++) 
	{
		network.layers[i][j].weights[k] += network.learningRate * network.layers[i][j].delta * network.layers[i][j].inputs[k];
	}
	
	network.layers[i][j].bias += network.learningRate * network.layers[i][j].delta;
	return network;
}



struct NeuralNetwork Forward(struct NeuralNetwork network, double (*ac)(double))
{
	int i, j, k;
	double y = 0;

	for(i = 0; i < network.layerNumber; i++)
	{
		for(j = 0; j < network.layerLength[i]; j++)
		{
			if(i != 0)
			{
				for(k = 0; k < network.layers[i][j].weightsLength; k++)
				{
					network.layers[i][j].inputs[k] = network.layers[i-1][k].output;
				}
			}
			
			y = 0;
			for(k = 0; k < network.layers[i][j].weightsLength; k++)
			{
				y += (network.layers[i][j].inputs[k] * network.layers[i][j].weights[k]);
			}
			y += network.layers[i][j].bias;
			y = (*ac)(y);
			network.layers[i][j].output = y;
		}
	}
    return network;
}



double Sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double SigmoidDerivative(double x)
{
	return x * (1.0 - x);
}

double Xor(double x, double y)
{
	return x == y ? 0 : 1;
}

int PrintXOR(struct NeuralNetwork network)
{
	int i, j;
	
	printf("--------------------------\n");
	for(i = 0; i < network.layerNumber; i++)
	{
		for(j = 0; j < network.layerLength[i]; j++) 
		{
			PrintNeuron(network.layers[i][j], i, j);
		}
	}
	
	printf("--------------------------\n");
	return 0;
}

int PrintNeuron(struct Neuron N, int i, int j)
{
	printf("\nNeuron[%i][%i] =\n", i, j);
	int k;
	for(k = 0; k < N.weightsLength; k++)
	{
		printf("inputs[%i] = %f\n", k, N.inputs[k]);
	}
	for(k = 0; k < N.weightsLength; k++)
	{
		printf("weights[%i] = %f\n", k, N.weights[k]);
	}
	printf("bias = %f\n", N.bias);
	printf("output = %f\n\n", N.output);
	return 0;
}
