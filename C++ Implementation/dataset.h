#ifndef DATASET_H
#define DATASET_H
#include <string>
#include <vector>
using namespace std;


class Dataset
{
    public:
        /** Default constructor */
        Dataset();
        Dataset(vector<string> nodes, const char* inputfilename, const char* targetfilename, int startdate);

        /** Default destructor */
        virtual ~Dataset();
        int num_io;
        vector<vector<double> > inputs;
        vector<vector<double> > targets;
        vector<string> nodes;
    protected:
    private:

        int date;
        //'''num of neurons required in input and output layer = #nodes + 24 hrs + 1 datetime'''

        vector<vector<double> >  extract(vector<string> nodes, const char* pathname, int startdate);
};

#endif // DATASET_H
