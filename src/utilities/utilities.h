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
#include <map>
using namespace std;

struct Comparator
{
	bool operator() (const vector<long> &lhs, const vector<long> &rhs) const
	{
		return lhs.size() > rhs.size();
	}
};


vector< vector<long> > generateCombs(int N, int K);

void findGreedySet(map<long, vector<long> > sstables, vector< vector<long> > combs, vector<long>& minSet, map<string, long>& costMap, int indexMap[]);

string toString(vector<long> idArray);

vector<long> toVector(string str);

void print_sets(priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap);

void print_sets(map<long, vector<long> > sets);

void print_set(vector<long> set);

void mergeSets(vector< vector<long> > toMergeSet, long &mergeCost, vector<long> &output);

#endif
