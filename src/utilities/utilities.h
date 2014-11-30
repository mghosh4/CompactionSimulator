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
#include "../sstable/sstable.h"
using namespace std;

typedef double(*cardfnptr)(vector<SStable>);

struct NumberComparator
{
	bool operator() (const vector<long> &lhs, const vector<long> &rhs) const
	{
		return lhs.size() > rhs.size();
	}
};

struct FileComparator
{
	bool operator() (const SStable &lhs, const SStable &rhs) const
	{
		return lhs.keyCount > rhs.keyCount;
	}
};

vector< vector<long> > generateCombs(int N, int K);

void findGreedySet(map<long, vector<long> > sstables, vector< vector<long> > combs, vector<long>& minSet, map<string, long>& costMap, int indexMap[]);

void findGreedySet(map<long, SStable> sstables, vector< vector<long> > combs, vector<long>& minSet, map<string, double>& costMap, int indexMap[], cardfnptr c);

double estimateCardinalityBF(vector<SStable> mergeSet);
double estimateCardinalityHL(vector<SStable> mergeSet);

string toString(vector<long> idArray);

vector<long> toVector(string str);

void print_sets(priority_queue<vector<long>, vector< vector<long> >, NumberComparator> fileHeap);

void print_sets(map<long, vector<long> > sets);

void print_sets(vector< vector<long> > sets);

void print_set(vector<long> set);

void print_cost_map(map<string, long> &costMap);

void print_cost_map(map<string, double> &costMap);

void mergeNumbers(vector< vector<long> > toMergeSet, long &mergeCost, vector<long> &output);

void mergeFiles(vector<SStable> toMergeSet, long &mergeCost, long numFiles, SStable &output);
#endif
