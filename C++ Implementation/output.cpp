#include "output.h"
#include <iostream>
#include "output.h"
#include <fstream>


using namespace std;

Output::Output()
{
    //ctor
}



Output::~Output()
{
    //dtor
}


Output::Output(Dataset * dataset, Network * net, const char * outputfilename)
{
    this->ds = dataset;
    this->net = net;
    this->nodes = dataset->nodes;
    this->outputfile = outputfilename;
    get_output();
    double sum =0, err;
    for(int i=0, len = ds->targets.size(); i<len; i++)
    {
        for(int j=0, leng = output[i].size(); j<24; j++)
        {
            err = (output[i][j]- ds->targets[i][j]);
            sum = sum + err*err;
        }
    }
    cout<<"output error with testing data: "<< sum / ds->targets.size()<<endl;
    //cout<<ds->targets.size()<<"  "<<ds->inputs.size()<<endl;
}

void Output::get_output()
{
    output.clear();
    for(int i=0, len=ds->inputs.size(); i<len; i++)
    {
        net->feedforward(ds->inputs[i]);
        output.push_back(net->output);
    }
}

void Output::get_output(vector<vector<double> > inputs)
{
    output.clear();
    for(int i=0, len=inputs.size(); i<len; i++)
    {
        net->feedforward(inputs[i]);
        output.push_back(net->output);
    }
}

void Output::organize_result()
{
    int input_len = ds->inputs.size();
    int target_len = ds->targets.size();
    int len = input_len;
    if(input_len > target_len ) len = target_len;
    outputplus.clear();
    //outputplus.assign(len*24, op);
    for(int i = 0; i < len; i++)
    {
        for(int j = 0 ; j < 24; j++)
        {
            struct OutputPlus op = {ds->inputs[i][j],ds->targets[i][j], output[i][j] };
            outputplus.push_back(op);
        }
    }
}

 void Output::organize_result(vector<vector<double> > inputs, vector<vector<double> > targets, vector<vector<double> > outputs)
 {
     int input_len = inputs.size();
    int target_len = targets.size();
    int len = input_len;
    if(input_len > target_len ) len = target_len;
    outputplus.clear();

    //outputplus.assign(len*24, op);
    for(int i = 0; i < len; i++)
    {
        for(int j = 0 ; j < 24; j++)
        {
            struct OutputPlus op = {inputs[i][j],targets[i][j], outputs[i][j] };
            outputplus.push_back(op);
        }
    }

 }

void Output::PL_analyzer()
{
    Daily_PL.clear();
    for(int i, len = outputplus.size(); i < len; i++)
    {
        double PL = outputplus[i].target - outputplus[i].input;
        if(outputplus[i].output <= outputplus[i].input) PL = -PL;
        Daily_PL.push_back(PL);

    }
    Overall_PL.clear();
    Overall_PL.push_back(0.0);
    for(int i = 0, len = Daily_PL.size(); i < len; i++)
    {
        Overall_PL.push_back(Daily_PL[i]+Overall_PL[i]);
    }
    for(int i = 0, len = Overall_PL.size(); i < len; i ++)
    {
        Overall_PL[i] = 100 * Overall_PL[i];
    }
}

void Output::write(string node = "", bool append = false)
{
    //cout << "# of data points: " << outputplus.size() << endl;
    ofstream out;
    double PL;
    if(append) out.open(outputfile, std::ofstream::app);
    else out.open(outputfile);
    if( node!= "") out<<node<<endl;
    out<<"Day Ahead, Real Time, NN Output, Profit/Loss"<<endl;
    int hour = 0, day = 1;
    for (int i =0, len = outputplus.size(); i < len; i++)
    {
        if(hour%24 == 0)
        {
             out<<endl<<"Day"<<day<<endl;
             day = day +1;
        }
        PL = outputplus[i].target - outputplus[i].input;
        if(outputplus[i].output <= outputplus[i].input) PL = -PL;
        out<<outputplus[i].input*100<<", "<<outputplus[i].target*100<<", "<<outputplus[i].output*100<<
        ", "<<PL*100<< endl;
        hour = hour +1;
    }
    out.close();

}

 vector<vector<double> > Output::filting_data(string node, vector<vector<double> >  * all_data )
 {
     vector<vector<double> >  result;
     int mark_pos;
     for(int i = 0, len = nodes.size(); i < len; i ++)
     {
         if(node == nodes[i]) {mark_pos = 24 + i; break;}
     }

     for(int i = 0, len = all_data->size(); i < len; i++)
     {
         vector<double> temp = all_data->at(i);
         if(temp[mark_pos] > 0) result.push_back(temp);
     }
     return result;
 }

void Output::analyze_by_node(bool Overall)
{
    for(int i = 0, len = nodes.size(); i < len; i ++)
    {
        vector<vector<double> > tempinputs = filting_data(nodes[i], &(ds->inputs) );
        vector<vector<double> > temptargets = filting_data(nodes[i], &(ds->targets) );
        get_output(tempinputs);
        organize_result(tempinputs,temptargets, output);
        PL_analyzer();
        write();
    }
}
void Output::analyze_by_node(vector<string> nodes, bool Overall = false)
{
    bool append = false;
    for(int i = 0, len = nodes.size(); i < len; i ++)
    {
        vector<vector<double> > tempinputs = filting_data(nodes[i], &(ds->inputs) );
        vector<vector<double> > temptargets = filting_data(nodes[i], &(ds->targets) );
        get_output(tempinputs);
        organize_result(tempinputs,temptargets, output);
        PL_analyzer();
        write(nodes[i], append);
        append = true;
    }
}
