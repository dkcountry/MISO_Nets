import sys
from neuralnet import *
import datautil

if (len(sys.argv) < 7):
    raise NameError("Not enough args")

def main(nodes_file, training_file, testing_file, numNeurons, numEpochs, learning_rate):
    nodes = datautil.get_nodes(nodes_file)
    print "Nodes to Analyze\n"
    print nodes
    
    print "\nProcessing training data..."
    training_set = datautil.Dataset(nodes,training_file+"/da",training_file+"/rt")
    print "Done."

    print "\nStarting Neural Net"
    net = Network(training_set.ionum,numNeurons,training_set.ionum)     
    net.add_dataset(training_set.inputs,training_set.targets)
    net.train(numEpochs, learning_rate, learning_curve = descent_learning)
    print "Training complete."

    print "\nProcessing testing data..."
    testing_set = datautil.Dataset(nodes,testing_file+"/da",testing_file+"/rt",training_set.date)
    print "Done."

    print "\nRunning testing set..."
    output = datautil.Output(testing_set,net)
    output.analyze_by_node(Overall = False)


x = main(str(sys.argv[1]), str(sys.argv[2]), str(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5]), float(sys.argv[6]))

