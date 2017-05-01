"""Neural Network - Supervised Learning
    David Kang and Sally He"""


'''**Standard Neural Net implementation with feedforward and backpropagation**

(Care should be taken in choosing parameter specifications. The learning rate and
associated learning curve will affect both speed and reliability of convergence, as
will the number of nodes used in the hidden layer).
'''


import random as rand
from scipy import exp, clip
import math


class Neuron:
    """Represents a single input/output process node in the model"""
    def __init__(self,inputlen):
        self.inputlen = inputlen
        self.inputs = None
        self.weights = None
        self.raw_value = None
        self.bias = None
        self.transformed_value = None
        self.error = None

    def addinputs(self, inputlist):
        if len(inputlist) == self.inputlen:
            self.inputs = inputlist
        else:
            raise NameError('InputLen_Mismatch')

    def set_testweights(self):
        self.weights = [.5 for x in range(self.inputlen)]
        
    def setnew_weights(self):
        '''Set initial weights here'''
        self.weights = [rand.uniform(-1,1) for x in range(self.inputlen)] 

    def genraw(self):
        dot = [self.inputs[x]*self.weights[x]
                          for x in range(self.inputlen)]
        self.raw_value = 0
        for x in dot:
            self.raw_value = self.raw_value + x


class HiddenNeuron(Neuron):
    def setnew_weights(self):
        self.bias = rand.uniform(-1,1)
        self.weights = [rand.uniform(-1,1) for x in range(self.inputlen)]

    def genraw(self):
        dot = [self.inputs[x]*self.weights[x]
                          for x in range(self.inputlen)]
        self.raw_value = 0
        for x in dot:
            self.raw_value = self.raw_value + x
        self.raw_value = self.raw_value + self.bias

    def activate(self):
        """Here we use the logistic/sigmoid function"""

        self.transformed_value = sigmoid(self.raw_value)
        

class OutputNeuron(Neuron):
    def activate(self):
        '''Identify'''
        self.transformed_value = self.raw_value

##### Activation/Learning Functions - Move to separate file later #####        
        
def safeExp(x):
    """ Bounded range for the exponential function (won't produce inf or NaN). """
    return exp(clip(x, -500, 500))


def sigmoid(x):
    """ Logistic sigmoid function. """
    return 1. / (1. + safeExp(-x*10))

def const_learning(x):
    return 1.

def descent_learning(x):
    """ Logistic sigmoid function. """
    if x < .1:
        return 1.
    else:
        return 1.5 - x


def sigmoidPrime(x):
    """ Derivative of logistic sigmoid. """
    tmp = sigmoid(x)
    return tmp * (1 - tmp)

#####################

class Network:
    """initiate # of Neurons per layer"""
    def __init__(self,inputlayer,hiddenlayer,outputlayer):
        self.hiddenlayer = self.addNeurons(inputlayer,hiddenlayer,HiddenNeuron)
        self.outputlayer = self.addNeurons(hiddenlayer,outputlayer,OutputNeuron)
        self.input = None
        self.target = None
        self.output = None
        

    def addNeurons(self, numInputs, numNeurons,Neurontype):
        Neurons = [Neurontype(numInputs) for x in range(numNeurons)]
        for x in Neurons:
            x.setnew_weights()
        return Neurons
        

    
    def add_dataset(self,input_dataset,target_dataset):
        '''each element of the input/output dataset must be a list'''
        assert len(input_dataset) == len(target_dataset)
        self.input = input_dataset
        self.target = target_dataset


    def feedforward(self, inputlist):
        """computes an output given an input, given current set of weights"""
        for x in self.hiddenlayer:
            x.addinputs(inputlist)
            x.genraw()
            x.activate()
        hiddenoutput = [x.transformed_value for x in self.hiddenlayer]
        for y in self.outputlayer:
            y.addinputs(hiddenoutput)
            y.genraw()
            y.transformed_value = y.raw_value
        self.output = [y.transformed_value for y in self.outputlayer]

            
    def backprop_error(self,targetlist):
        '''For each node, calculate the error using backpropagation as explained by wikipedia'''
        for y in range(len(self.outputlayer)):
            self.outputlayer[y].error = (self.outputlayer[y].transformed_value - targetlist[y])

        for x in range(len(self.hiddenlayer)):
            error = 0
            for y in self.outputlayer:
                error = error + y.weights[x]*y.error
            error = error * sigmoidPrime(self.hiddenlayer[x].raw_value)
            self.hiddenlayer[x].error = error

        
    def backprop_weight(self,learning_rate):
        '''adjusts weights in each layer by gradient descent'''
        for y in self.outputlayer:              #For each Neuron
            for i in range(y.inputlen):         #For each weight
                delta = y.error*y.inputs[i]
                y.weights[i] = -learning_rate*delta + y.weights[i]

        for x in self.hiddenlayer:
            for i in range(x.inputlen):
                delta = x.error*x.inputs[i]
                x.weights[i] = -learning_rate*delta + x.weights[i]
            x.bias = -learning_rate * x.error + x.bias

    def train_one_iteration(self,learning_rate = .05):
        '''iterates through one epoch of training'''
        for data in zip(self.input,self.target):
            self.feedforward(data[0])
            self.backprop_error(data[1])
            self.backprop_weight(learning_rate)

    def train(self,num_iterations, learning_rate = .05, learning_curve = const_learning):
        '''Train on dataset for n epochs'''
        print "Learning..."
        for x in range(num_iterations):
            iter_frac = float(x)/num_iterations
            adjusted_learning_rate = learning_rate*learning_curve(iter_frac)
            self.train_one_iteration(adjusted_learning_rate)
            print "Epoch iteration: " + str(x+1)
        print "Done."
            

            
