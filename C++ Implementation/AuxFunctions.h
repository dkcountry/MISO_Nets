#ifndef AUXFUNCTIONS_H_INCLUDED
#define AUXFUNCTIONS_H_INCLUDED
#include <string>
#include <vector>
#include <list>
using namespace std;



double const_learning(double x);

double decent_learning(double x);


double safeExp(double x);


double sigmoid(double x);

double sigmoidPrime(double x);

void get_nodes(const char * nodesfilename, vector<string> *nodes);

void csvline_populate(vector<string> &record, const string& line, char delimiter);

#endif // AUXFUNCTIONS_H_INCLUDED
