#include "Neuron.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

Neuron::Neuron()
{
    this->inputlen = 0;//ctor
}

Neuron::Neuron(int inputlen)
{
    this->inputlen = inputlen;//ctor
}

Neuron::~Neuron()
{
    //dtor
}

void Neuron::init(int inputlen)
{
    this->inputlen = inputlen;//ctor
    this->weights.assign(inputlen,0.0);
    this->inputs.assign(inputlen,0.0);
    this->bias = 0.5;
    this->raw_value = 0.0;
    this->transformed_value = 0.0;

}

void Neuron::addinputs(vector<double> inputlist)
{
    if(inputlist.size() != inputlen) {cout<<inputlist.size()<<"input length mismatch"<<inputlen<<endl; return;}
    this->inputs = inputlist;
}

void Neuron::set_testweights()
{
    // self.weights = [.5 for x in range(self.inputlen)]
    for(int i = 0; i<inputlen; i++)
     {
         weights[i]=0.5;
     }
}

void Neuron::set_newweights()
{
    /*
     '''Set initial weights here'''
        self.weights = [rand.uniform(-1,1) for x in range(self.inputlen)]
    */
     for(int i = 0; i<inputlen; i++)
     {
         weights[i]= 1.0 - 2*((double)rand())/RAND_MAX;
     }
}

void Neuron::print()
{
     cout<<"inputs-weights :";
     for(int i = 0, len = inputs.size();i<len;  i++)
     {
         cout<<inputs[i]<<"-"<<weights[i]<<"; ";
     }
     cout<<endl;
     cout<<"bias:"<<bias<<"; rawval:"<<raw_value<<"; transformed:"<<transformed_value<<"; error:"<<error<<endl;
}

void Neuron::genraw()
{
    /*
    dot = [self.inputs[x]*self.weights[x]
                          for x in range(self.inputlen)]
        self.raw_value = 0
        for x in dot:
            self.raw_value = self.raw_value + x
    */
    double sum = 0;
    for(int i = 0; i< inputlen; i++)
    {
        sum = sum + inputs[i] * weights[i];
    }
    raw_value = sum;
}



