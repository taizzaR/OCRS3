#include <stdio.h>
#include "tools.h"
#include <math.h>

struct Neuron{		// Structure d'un neurone
	double output;
	int weightsLength;
	double bias;
	double delta;
	double inputs[2];
	double weights[2];
};

struct NeuralNetwork{			// Structure du reseau
	struct Neuron layers[2][2];
	int layersNumber;
	int inputsLength; 
	int layerLength[2]; 
	double learningRate;
};

struct Dataset	// Structure des data
{			
	double inputs[4][2]; 
	double outputs[4][1]; 
	int datasetLength;
};

double Random(double min, double max);
struct NeuralNetwork Initialization(struct NeuralNetwork Neural);
struct NeuralNetwork Train(struct NeuralNetwork Neural, struct Dataset data);
struct NeuralNetwork Input(struct NeuralNetwork Neural, double input[]);
struct NeuralNetwork Backward(struct NeuralNetwork Neural, double output[]);
struct NeuralNetwork UpdateWeights(struct NeuralNetwork Neural, int i, int j);
struct NeuralNetwork Forward(struct NeuralNetwork Neural, double (*ac)(double));
double Relu(double x);
double ReluDerivative(double x);
void printNeuron(struct NeuralNetwork NN);
void printNetwork(struct Neuron N, int i, int j);


int main()		//Programme principal
{
	struct NeuralNetwork network;
	struct Dataset data;

	double _inputs[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
	double _outputs[4][1] = {{0}, {1}, {1}, {0}};
	
	network.layersNumber = 2;
	network.layerLength[0] = 2;
	network.layerLength[1] = 1;
	network.inputsLength = 2;

	network.learningRate = 0.1;

	for (int i = 0; i < 4; i++){
		for (int j = 0; i < 2; i++)			// Initialisation de data
		{
			data.inputs[i][j] = _inputs[i][j];
		}
		data.outputs[i][0] = _outputs[i][0];
	}
	data.datasetLength = 4;

	network = Initialization(network);		// Lancement du reseau
	network = Train(network,data);

    return 0;
}

struct NeuralNetwork Initialization(struct NeuralNetwork network)	// Initialise les poids et biais de tous les neurones
{
    foreach(struct Neuron layer in network.layers)
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

struct NeuralNetwork Forward(struct NeuralNetwork Neural, double (*ac) (double)){	// Parcours du reseau classique
    int i,j,k = 0;
    double x = 0;
    for (i = 0; i < Neural.layersNumber; i++)
    {
        for (j = 0; j < Neural.layerLength[i]; j++){
	    	if (i != 0){
				for (k = 0; Neural.layers[i][j].weightsLength; k++){
		    		Neural.layers[i][j].inputs[k] = Neural.layers[i-1][j].output;	
				}
	    	}
	    	for (k = 0; k < Neural.layers[i][j].weightsLength; k++){
				x += (Neural.layers[i][j].inputs[k] * Neural.layers[i][j].weights[k]);
			}
	    	x += Neural.layers[i][j].bias;
	    	x = (*ac)(x);
	    	Neural.layers[i][j].output = x;
		}
    }
    return Neural;
}

double Relu(double x){
	return x > 0 ? x : 0;
}

struct NeuralNetwork Train(struct NeuralNetwork Neural, struct Dataset dataset)	// Entrainement du reseau
{
	int train = ;
	int i,j;
	for (i = 0; i < train; i++)
	{
		for (j = 0; i < dataset.datasetLength; j++)
		{
			Neural = Input(Neural,dataset.inputs[j]);
			Neural = Forward(Neural,Relu);
			Neural = Backward(Neural,dataset.outputs[j]);
		}
	}
	
	return Neural;
}

struct NeuralNetwork Input(struct NeuralNetwork network, double input[])	// Mis a jour des inputs au debut du reseau
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

struct NeuralNetwork Backward(struct NeuralNetwork network, double output[])	// Parcours inverse du reseau
{
    for (int i = network.layersNumber-1; i >= 0; i++)	// Modifie les neurones en fonction
    {													// du taux d'erreur
        if (i != network.layersNumber - 1)
        {
            for(int j = 0; j < network.layerLength[i]; j++)
            {
                double error = 0;
                for(int k = 0; k < network.layerLength[i+1]; k++)
                {
                    error += network.layers[i+1][k].delta * network.layers[i+1][k].weights[j];
                }
                network.layers[i][j].delta = ReluDerivative(network.layers[i][j].output) * error;
                network = UpdateWeights(network, i, j);
            }
        }
        else
        {
            int j = 0;
            foreach(struct Neuron neuron in network.layers)
            {
                neuron.delta = ReluDerivative(neuron.output) * (output[0] - neuron.output);
                network = UpdateWeights(network, network.layersNumber - 1, j);
                j++;
            }
        }
    }
    return network;
}

struct NeuralNetwork UpdateWeights(struct NeuralNetwork Neural,int i, int j){	// Modifie les poids d'un neurone
	int k = 0;
	for (k; k < Neural.layers[i][j].weightsLength; k++)
	{
		Neural.layers[i][j].weights[k] += Neural.learningRate * Neural.layers[i][j].delta * Neural.layers[i][j].inputs[k];
	}
	Neural.layers[i][j].bias += Neural.learningRate * Neural.layers[i][j].delta;
	return Neural;
}

void printNeuron(struct Neuron neuron)
{
    printf("INPUTS : %f, %f\nBIAS : %f\nOUTPUT : %f\n", neuron.inputs[0], neuron.inputs[1], neuron.bias, neuron.output);
}


void printNetwork(struct NeuralNetwork network, double finalOutput)
{
    int i;
    printNeuron(network.layers[0][0]);
    for (i = 0; i < 3; i++)
        printf("\n");
    printf("                    ");
    printNeuron(network.layers[1][0]);
    for (i = 0; i < 3; i++)
        printf("\n");
    printNeuron(network.layers[0][1]);
}