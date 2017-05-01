#include "Network.h"
#include "HiddenNeuron.h"
#include "OutputNeuron.h"
#include <iostream>
#include "AuxFunctions.h"
#include <fstream>

using namespace std;

Network::Network(int num_inputlayer, int num_hiddenlayer, int num_outputlayer, const char * errorlogfilename)
{
    this->num_inputlayer = num_inputlayer;
    this->num_hiddenlayer = num_hiddenlayer;
    this->num_outputlayer = num_outputlayer;
    this->errorlogfie = errorlogfilename;
    outputlayer = new OutputNeuron[num_outputlayer];
    for(int i = 0; i< num_outputlayer; i++)
    {
        outputlayer[i].init(num_hiddenlayer);
        outputlayer[i].Setinputlen(num_hiddenlayer);
        outputlayer[i].set_newweights();
    }
    hiddenlayer = new HiddenNeuron[num_hiddenlayer];
    for(int i = 0; i< num_hiddenlayer; i++)
    {
        hiddenlayer[i].init(num_inputlayer);
        hiddenlayer[i].Setinputlen(num_inputlayer);
        hiddenlayer[i].set_newweights();
    }
    hidden_output.assign(num_hiddenlayer,0);
    output.assign(num_outputlayer,0);
}

Network::~Network()
{
    //dtor
}

void Network::add_dataset(vector<vector<double> > input_dataset, vector<vector<double> > target_dataset)
{
    this->input_dataset = input_dataset;
    this->target_dataset = target_dataset;
}

void Network::feedforward(vector<double> inputlist)
{
    /*
      def feedforward(self, inputlist):
        """computes an output given an input, given current set of weights"""

    */
    for(int i = 0; i<num_hiddenlayer; i++)
    {
        hiddenlayer[i].addinputs(inputlist);
        hiddenlayer[i].genraw();
        hiddenlayer[i].activate();
        hidden_output[i] = hiddenlayer[i].get_transformed_value();
    }

    for(int i = 0; i<num_outputlayer; i++)
    {
        outputlayer[i].addinputs(hidden_output);
        outputlayer[i].genraw();
        outputlayer[i].activate();
        output[i] = outputlayer[i].get_transformed_value();
        //output[i]=inputlist[i];

    }
}

void  Network::backprop_error(vector<double> targetlist)
{
   /*
    def backprop_error(self,targetlist):
        '''For each node, calculate the error using backpropagation as explained by wikipedia'''

   */
   for(int i = 0; i<num_outputlayer; i++)
   {
       double err = output[i] - targetlist[i];
       outputlayer[i].set_error(err);
       // Compute MSE for first 24 outputs, ie the prices
       if (i < 24)
       square_error = square_error + err*err;
   }
   for(int i = 0; i< num_hiddenlayer;i++)
   {
       double err = 0;
       for(int j = 0; j< num_outputlayer; j++)
       {
           err = err + outputlayer[j].get_weight(i) * outputlayer[j].get_error();
       }
       err = err * sigmoidPrime(hiddenlayer[i].get_raw_value());
       hiddenlayer[i].set_error(err);
   }
}

void Network::backprop_weight(double learning_rate)
{
    for(int i=0; i<num_outputlayer; i++)
    {
        for(int j=0; j<outputlayer[i].Getinputlen();j++)
        {
            double delta = outputlayer[i].get_error() * outputlayer[i].get_input(j);
            double weights = -learning_rate * delta + outputlayer[i].get_weight(j);
            outputlayer[i].set_weights(weights, j);
        }
    }
    for(int i=0; i<num_hiddenlayer;i++)
    {
        for(int j=0; j<hiddenlayer[i].Getinputlen();j++)
        {
            double delta = hiddenlayer[i].get_error() * hiddenlayer[i].get_input(j);
            double weights = -learning_rate * delta + hiddenlayer[i].get_weight(j);
            hiddenlayer[i].set_weights(weights,j);
        }
        double bias = -learning_rate * hiddenlayer[i].get_error() + hiddenlayer[i].get_bias();
        hiddenlayer[i].set_bias(bias);
    }
}

void Network::train_one_iteration(double learning_rate)
{
    square_error = 0;
    for(int i=0, len = input_dataset.size(); i<len; i++)
    {

        feedforward(input_dataset[i]);
        backprop_error(target_dataset[i]);
        backprop_weight(learning_rate);
    }
    cout<<"total error: "<<square_error/input_dataset.size()<<endl;
    errorlist.push_back(square_error/input_dataset.size());

}

void Network::train(int num_iteration, double learning_rate, bool decent_learn)
{
    std::cout<<"learning"<<endl;
    double adjusted_learning_rate;
    for(int i=0;i<num_iteration;i++)
    {
        double iter_frac = ((double)i)/num_iteration;
        if (decent_learn == true)
        {
            adjusted_learning_rate = learning_rate * decent_learning(iter_frac);
        }
        else
        {
            adjusted_learning_rate = learning_rate * const_learning(iter_frac);
        }
    train_one_iteration(adjusted_learning_rate);
    std::cout<<endl<<"Epoch iteration:"<<i+1<<endl;
    }
    cout<<"done"<<endl;
}

void Network::print()
{
    cout<<"outputlayer"<<endl;
     for(int i =0; i< num_outputlayer; i++)
    {
        outputlayer[i].print();
    }
    cout<<endl;
    cout<<"hiddenlayer"<<endl;
    for(int i =0; i< num_hiddenlayer; i++)
    {
        hiddenlayer[i].print();
    }
    cout<<endl;

}

void Network::printoutputlayer()
{
    cout<<"outputlayer"<<endl;
     for(int i =0; i< num_outputlayer; i++)
    {
        outputlayer[i].print();
    }
    cout<<endl;

}

void Network::printoutput(vector<vector<double> > input_dataset, vector<vector<double> > target_dataset)
{
    add_dataset(input_dataset,target_dataset);

    for(int i=0, len = input_dataset.size(); i< len; i++)
    {
        feedforward(input_dataset[i]);
        for(int j=0;j<num_outputlayer;j++)
        {
            cout<<"data "<<i<<": ";
            cout<<"output"<<j<<":"<<output[j]<<", target"<<j<<": "<<target_dataset[i][j]<<endl;
        }
        cout<<endl;

    }
}

void Network::save_errorlist()
{
    ofstream out(errorlogfie);
    for (int i =0, len = errorlist.size(); i < len; i++)
    {
        out<<errorlist[i]<<endl;
    }
    out.close();

}
