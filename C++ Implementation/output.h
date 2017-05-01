#ifndef OUTPUT_H
#define OUTPUT_H
#include "Network.h"
#include "dataset.h"
#include <vector>
#include <string>


struct OutputPlus
{
   double input;
   double target;
   double output;
};

class Output
{
    public:
        /** Default constructor */
        Output();
        Output(Dataset * dataset, Network * net, const char * outputfilename);
        /** Default destructor */
        virtual ~Output();
        void analyze_by_node(bool Overall);
        void analyze_by_node(vector<string> nodes, bool Overall);

    protected:
    private:
        Network * net;
        Dataset * ds;
        vector<string>  nodes;
        vector<vector<double> > output;
        vector<struct OutputPlus> outputplus;
        vector<double> Daily_PL;
        vector<double> Overall_PL;
        const char * outputfile;

        void get_output();
        void get_output(vector<vector<double> > inputs);
        void organize_result();
        void organize_result(vector<vector<double> > inputs, vector<vector<double> > targets, vector<vector<double> > outputs);
        void PL_analyzer();
        void write(string node, bool appende);
        //! process the dataset to extract data associated with a node
        vector<vector<double> > filting_data(string node, vector<vector<double> >  * all_data );

};

#endif // OUTPUT_H
