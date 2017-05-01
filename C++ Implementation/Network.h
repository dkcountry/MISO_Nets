#ifndef NETWORK_H
#define NETWORK_H

#include "Neuron.h"
#include "HiddenNeuron.h"
#include "OutputNeuron.h"


class Network
{
    public:
        /** Default constructor */
        Network(int num_inputlayer, int num_hiddenlayer, int num_outputlayer, const char * errologfilename);
        /** Default destructor */
        virtual ~Network();

        void add_dataset(vector<vector<double> > input_dataset, vector<vector<double> > target_dataset);
        void print(); //!< for debug, print out network detail;
        void printoutputlayer(); //!<for debug, print output layer detail
        void printoutput(vector<vector<double> > input_dataset, vector<vector<double> > target_dataset); //!< for debug , print output and target
        void train(int num_iteration, double learning_rate, bool decent_learning);
        void feedforward(vector<double>  inputlist);
        void save_errorlist();

        vector<double> output;
        double square_error;  //!< square_error = {sum of ( sum of square error of each output )}/length of dataset
        vector<double> errorlist; //!<store square error along the training
        const char * errorlogfie; //!< file to save the error log

    protected:
    private:
        HiddenNeuron * hiddenlayer; //!<pointer to an array of neuron in hiddenlayer
        OutputNeuron * outputlayer; //!< pointer to an array of outputlayer
        int num_inputlayer;
        int num_hiddenlayer;
        int num_outputlayer;
        vector<double> hidden_output;
        vector<vector<double> > input_dataset;
        vector<vector<double> > target_dataset;

        void backprop_error(vector<double>  targetlist);
        void backprop_weight(double learning_rate);
        void train_one_iteration(double learning_rate);



};


#endif // NETWORK_H
