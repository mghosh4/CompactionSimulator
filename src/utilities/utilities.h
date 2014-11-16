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
#include <vector>
#include <queue>
using namespace std;

struct Comparator
{
	bool operator() (const vector<long> &lhs, const vector<long> &rhs) const
	{
		return lhs.size() > rhs.size();
	}
};


vector< vector<long> > generateCombs(int N, int K);

string toString(vector<long> idArray);

void print_sets(priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap);

void print_set(vector<long> set);

#endif
