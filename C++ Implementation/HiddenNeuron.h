#ifndef HIDDENNEURON_H
#define HIDDENNEURON_H

#include "Neuron.h"


class HiddenNeuron : public Neuron
{
    public:
        void set_newweights();
        void genraw();
        void activate();
    protected:
    private:
};

#endif // HIDDENNEURON_H
