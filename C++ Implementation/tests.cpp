#include <getopt.h>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include "tests.h"
#include "Network.h"
#include "AuxFunctions.h"
#include "Dataset.h"
#include "output.h"




using namespace std;

void testNN()
{
    int dataset_len = 4;
    int input_len =2;
    int output_len = 2;
    double input[] ={0,0,0,1,1,0,1,1};
    const char * errorlogfile = "D:/CS/cs3/nn/errorlog.csv";
    vector<vector<double> > inputdata(dataset_len);
    for(int i=0; i<dataset_len; i++)
    {
        //inputdata[i].reserve(input_len);
        vector<double> temp(input_len);
        for(int j=0; j<input_len; j++)
        {
            temp[j] =input[i*input_len+j];
        }
        inputdata[i]=temp;
    }
    
    double target[] ={1,0,1,0,1,0,0,1};
    vector<vector<double> > targetdata(dataset_len);
    for(int i=0; i<dataset_len; i++)
    {
        vector<double> temp(input_len);
        for(int j=0; j<output_len; j++)
        {
            temp[j] = target[i*output_len+j];
        }
        targetdata[i] = temp;
    }
    
    Network * net = new Network(input_len, 100, output_len, errorlogfile);
    net->add_dataset(inputdata,targetdata);
    net->train(50,0.02,true);
    net->printoutput(inputdata,targetdata);
    //net->printoutputlayer();
}

void test_getnodes()
{
    const char * filename = "D:/CS/cs3/nn/node.txt";
    vector<string> nodes;
    get_nodes(filename, &nodes);
    
}

void testTraining()
{
    const char * filename = "files/node.txt";
    const char * inputfilepath = "files/training/da/";
    const char * targetfilepath = "files/training/rt/";
    const char * testinputfilepath = "files/testing/da/";
    const char * testtargetfilepath = "files/testing/rt/";
    const char * errorlogfile = "files/errorlog.csv";
    const char * outputfile = "files/outputfie.csv";
    vector<string> nodes;
    get_nodes(filename, &nodes);
    cout<<"Nodes to Analyze:"<<endl;
    for(int i = 0, len = nodes.size();i<len; i++)
    {
        cout<<nodes[i]<<endl;
    }
    cout<<"Processing training data..."<<endl;
    Dataset dataset = Dataset(nodes, inputfilepath, targetfilepath,0);
    cout<<"Done"<<endl;
    
    cout<<"Starting Neural Net..."<<endl;
    Network net = Network(dataset.num_io, 10, dataset.num_io, errorlogfile);
    net.add_dataset(dataset.inputs,dataset.targets);
    net.train(20,0.05,true);
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
}


