#include "HiddenNeuron.h"
#include <stdlib.h>
#include "AuxFunctions.h"

void HiddenNeuron::set_newweights()
{
    /*
     def setnew_weights(self):
        self.bias = rand.uniform(-1,1)
        self.weights = [rand.uniform(-1,1) for x in range(self.inputlen)]
    */
    for(int i = 0; i < inputlen; i ++ )
    {
        weights[i] =  1.0 - 2.0 * ((double)rand())/RAND_MAX;
    }
    bias = 1.0 - 2.0 * ((double)rand())/RAND_MAX;

}

void HiddenNeuron::genraw()
{
    /*
     def genraw(self):
        dot = [self.inputs[x]*self.weights[x]
                          for x in range(self.inputlen)]
        self.raw_value = 0
        for x in dot:
            self.raw_value = self.raw_value + x
        self.raw_value = self.raw_value + self.bias
    */

    double sum = 0;
    for(int i = 0; i< inputlen; i++)
    {
        sum = sum + inputs[i] * weights[i];
    }
    raw_value = sum + bias;
}

void HiddenNeuron::activate()
{
    /*
     def activate(self):
         """Here we use the logistic/sigmoid function"""
        self.transformed_value = sigmoid(self.raw_value)

    */
    transformed_value = sigmoid(raw_value);
}





