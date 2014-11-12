#ifndef UTILITIES_H
#define UTILITIES_H

/*#include <boost/archive/binary_oarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp>

template <typename ClassTo> 
int Save(const string fname, const ClassTo &c)
{ 
    ofstream f(fname.c_str(), ios::binary);
    if (f.fail()) return -1;
    boost::archive::binary_oarchive oa(f); 
    oa << c; 
    return 0;
}*/
#include <string>
using namespace std;

vector< vector<long> > generateCombs(int N, int K);

string toString(vector<long> idArray);

void print_set(vector<long> set);

#endif
