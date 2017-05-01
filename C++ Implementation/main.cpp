#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include<exception>
#include "Network.h"
#include "AuxFunctions.h"
#include "dataset.h"
#include "output.h"

using namespace std;

int main(int argc, char** argv)
{
    const string USAGE = "[-d nodefile] [-i trainingfile] [-t testingfile] [-k numNeurons] [-n numtraining_sessions] [-l learning rate] [-e error log dest] [-o output dest]";

    // For parsing command-line options
	int opt;
    // File location for list of nodes
    const char *nodefile = "";
    // File location for training set
    const char *trainingfile = "";
    // File location for testing set
    const char *testingfile = "";
    // Number of neurons to specify in the network
    int numNeurons = 10;
    int numtraining_sessions = 10;
    double learning_rate = .05;
    const char * errorlogfile = "Output/errorlog.csv";
    const char * outputfile = "Output/output.csv";
    
    // Parse command-line options
	while ((opt = getopt(argc, argv, "d:i:t:k:n:l:e:o:vh")) != -1)
		switch (opt)
    {
        case 'd':
            nodefile = optarg;
            break;
        case 'i':
            trainingfile = optarg;
            break;
        case 't':
            testingfile = optarg;
            break;
        case 'k':
            numNeurons = atoi(optarg);
            break;
        case 'n':
            numtraining_sessions = atoi(optarg);
            break;
        case 'l':
            learning_rate = atof(optarg);
            break;
        case 'e':
            errorlogfile = optarg;
            break;
        case 'o':
            outputfile = optarg;
            break;
        default:
            cerr << "Usage: " << argv[0] << " " << USAGE << endl;
            cerr << "ERROR: Unknown option -" << char(opt) << endl;
            exit(-1);
    }
	if(strncmp(nodefile,"",1) == 0 || strncmp(trainingfile,"",1) == 0 || strncmp(testingfile,"",1) == 0 )
	{
		cerr << "Usage: " << argv[0] << " " << USAGE << endl;
		cerr << "ERROR: Must specify a node file with -d" << endl;
        cerr << "a training/input file with -i" << endl;
        cerr << "and a testing file with -t" << endl;
		exit(-1);
	}
    // Get the input/target locations for training & testing set
    char inputfilepath[200];
    strcpy(inputfilepath,trainingfile);
    strcat(inputfilepath,"/da/");
    char targetfilepath[200];
    strcpy(targetfilepath,trainingfile);
    strcat(targetfilepath,"/rt/");
    char testinputfilepath[200];
    strcpy(testinputfilepath,testingfile);
    strcat(testinputfilepath,"/da/");
    char testtargetfilepath[200];
    strcpy(testtargetfilepath,testingfile);
    strcat(testtargetfilepath,"/rt/");
    
    // Get nodes
    vector<string> nodes;
    get_nodes(nodefile, &nodes);
    cout<<"Nodes to Analyze:"<<endl;
    for(int i = 0, len = nodes.size();i<len; i++)
    {
        cout<<nodes[i]<<endl;
    }
    
    cout<<"Processing training data..."<<endl;
    Dataset dataset = Dataset(nodes, inputfilepath, targetfilepath,0);
    cout<<"Done"<<endl;
    
    cout<<"Starting Neural Net..."<<endl;
    Network net = Network(dataset.num_io, numNeurons, dataset.num_io, errorlogfile);
    net.add_dataset(dataset.inputs,dataset.targets);
    net.train(numtraining_sessions,learning_rate,true);
    cout<<"Training complete."<<endl;
    net.save_errorlist();
    cout<<"Training error log file saved in "<<errorlogfile<<endl;
    
    cout<<"Processing testing data..."<<endl;
    Dataset testdataset = Dataset(nodes, testinputfilepath, testtargetfilepath,0);
    cout<<"Done"<<endl;
    
    cout<<"Running testing set..."<<endl;
    Output output = Output(&testdataset,&net, outputfile);
    output.analyze_by_node(nodes,true);
    cout<<"test results saved in "<<outputfile<<endl;
    
    return 0;
}
