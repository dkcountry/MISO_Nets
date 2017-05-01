#include <math.h>
#include "AuxFunctions.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;


double const_learning(double x)
{
    return 1.0;
}

double decent_learning(double x)
{
    if(x < 0.1) return 1.0;
    return 1.5 - x;
}


double safeExp(double x)
{
    if(x > 500) return exp(500);
    if(x < -500) return exp(-500);
    return exp(x);
}


double sigmoid(double x)
{
    return 1.0/(1.0+safeExp(-x*10));
}



double sigmoidPrime(double x)
{
    double tmp = sigmoid(x);
    return tmp * (1-tmp);
}

void get_nodes(const char* nodesfilename, vector<string> *nodes)
{
    ifstream in(nodesfilename);
    string line;
    if (in.fail())
    {
        cerr << "ERROR: File not found: " << nodesfilename << endl;
        exit(-1);
    }
    nodes->clear();
    while(getline(in, line)&&in.good())
    {
       if(line == "") continue;
       csvline_populate(*nodes, line, ',');
    }
    in.close();
}

void csvline_populate(vector<string> &record, const string& line, char delimiter)
{
    int linepos=0;
    int inquotes=false;
    char c;
    int linemax=line.length();
    string curstring;

    while(line[linepos]!=0 && linepos < linemax)
    {

        c = line[linepos];

        if (!inquotes && curstring.length()==0 && c=='"')
        {
            //beginquotechar
            inquotes=true;
        }
        else if (inquotes && c=='"')
        {
            //quotechar
            if ( (linepos+1 <linemax) && (line[linepos+1]=='"') )
            {
                //encountered 2 double quotes in a row (resolves to 1 double quote)
                curstring.push_back(c);
                linepos++;
            }
            else
            {
                //endquotechar
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter)
        {
            //end of field
            record.push_back( curstring );
            curstring="";
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            record.push_back( curstring );
            return;
        }
        else
        {
            curstring.push_back(c);
        }
        linepos++;
    }
    record.push_back( curstring );
    return;

}
