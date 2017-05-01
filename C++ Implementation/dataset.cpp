#include "dataset.h"
#include <iostream>
#include <dirent.h>
#include "AuxFunctions.h"
#include <fstream>
#include <string.h>
#include <stdlib.h>

using namespace std;

Dataset::Dataset()
{
    //ctor
}

Dataset::Dataset(vector<string> nodes, const char* inputfilename, const char* targetfilename, int startdate)
{
    date = 0;
    this->nodes = nodes;
    inputs = extract(this->nodes, inputfilename,startdate);
    targets = extract(this->nodes, targetfilename,startdate);

}
Dataset::~Dataset()
{
    //dtor
}


vector<vector<double> >  Dataset::extract(vector<string> nodes, const char* pathname, int startdate)
{
    vector<string> files;
    DIR*     dir;
    dirent*  pdir;
    dir = opendir(pathname);
    while ((pdir = readdir(dir)))
    {
        char * filename = pdir->d_name;
        if(filename[0] == '2')
        {
             files.push_back( filename );

        }

    }
    closedir(dir);

    vector<vector<double> > cleandata;
    int dateindex = 0;
    for(int i=0, len=files.size(); i<len; i++)
    {
        dateindex++;
        const char* filename = files[i].c_str();
        char fullname[200];
        strcpy(fullname, pathname);
        strcat(fullname, filename);
        ifstream in(fullname);
        string line;
        if (in.fail())
        {
            cerr << "ERROR: File not found: " << fullname << endl;
            exit(-1);
            return cleandata;
        }
        vector<string> temp;
        vector<double> temp_double;
        while(getline(in, line)&&in.good())
        {
            if(line == "") continue;
            temp.clear();
            temp_double.clear();
            csvline_populate(temp, line, ',');
            //for(int j = 0, leng = temp.size(); j<leng; j++) cout<<temp[j]<<"/t";
            for(int j = 0, leng = nodes.size(); j<leng; j++)
            {
                if(temp[0]==nodes[j])
                {
                    if(temp[2]=="LMP")
                    {
                        temp.erase(temp.begin(),temp.begin()+3);
                        for(int k = 0, length = temp.size(); k < length; k++)
                        {
                            temp_double.push_back(atof(temp[k].c_str())/100.0);
                        }
                        for(int k = 0, length = nodes.size(); k<length; k++)    //add a mark [ 1, 0, 0, 0, 0] for node 0
                        {
                            if(k==j) temp_double.push_back(1.0);
                            else  temp_double.push_back(0.0);
                        }
                        temp_double.push_back(dateindex);
                        //temp_double.push_back(0.0);
                        cleandata.push_back(temp_double);

                    }
                }
            }
        }
        in.close();
    }
    this->date = dateindex;
    this->num_io = cleandata[0].size();
    //for(int j=0, leng = cleandata.size(); j<leng; j++) cout<<cleandata[j].at(29-1)<<" "<<cleandata[j].at(0)<<endl;
    return cleandata;
}

