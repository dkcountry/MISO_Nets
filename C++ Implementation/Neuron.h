#ifndef NEURON_H
#define NEURON_H
#include <vector>
using namespace std;

class Neuron
{
    public:
        /** Default constructor */
        Neuron();
        Neuron(int inputlen);
        /** Default destructor */
        virtual ~Neuron();
        int Getinputlen() { return inputlen; }
        void Setinputlen(int val) { inputlen = val; }
        void init(int inputlen);
        void addinputs(vector<double> inputlist);
        void set_testweights();
        void set_newweights();
        void genraw();
        double get_transformed_value() {return transformed_value;}
        void set_error(double error) {this->error = error;}
        double get_error() {return error;}
        double get_weight(int n) {return weights[n];}
        void set_weight(double val, int n) {weights[n] = val;}
        double get_input(int n) { return inputs[n];}
        void set_weights(double weight,int n) {weights[n] = weight;}
        double get_bias() {return bias;}
        void set_bias(double bias) {this->bias = bias;}
        void print();
        double get_raw_value()  {return raw_value;}

    protected:

        int inputlen;
        vector<double> inputs;
        vector<double> weights;
        double bias;
        double transformed_value;
        double raw_value;
        double error;
    private:

};

#endif // NEURON_H
