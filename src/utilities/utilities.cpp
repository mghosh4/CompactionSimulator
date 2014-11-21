#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include "utilities.h"
#include "../utilities/kwaymerge.h"

vector< vector<long> > generateCombs(int N, int K)
{
	vector< vector<long> > combs;
	vector<long> oneComb;
    string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
 
    // print integers and permute bitmask
    do {
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) oneComb.push_back(i);
        }
        combs.push_back(oneComb);
		oneComb.clear();
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

	return combs;
}

void findGreedySet(map<long, vector<long> > sstables, vector< vector<long> > combs, vector<long>& minSet, map<string, long>& costMap, int indexMap[])
{
	//cout << "Size of costMap " << costMap.size() << " sstable size " << sstables.size() << endl;
	long minCost = INT_MAX, mergeCost;
	//long count = 0;
	for (vector< vector<long> >::iterator it = combs.begin(); it != combs.end(); it++)
	{
		vector<long> singleComb = *it;
		
		vector< vector<long> > mergeSet;
		vector<long> idArray;
		long cost = 0;
		vector<long>::iterator it1 = singleComb.begin();
		for (; it1 != singleComb.end(); it1++)
		{
			//cout << *it1 << endl;
			if (*it1 >= sstables.size())
				break;
			idArray.push_back(indexMap[*it1]);
			mergeSet.push_back(sstables[indexMap[*it1]]);
		}

		// Case when we hit upon an index which is outside the current range of sstables
		if (it1 != singleComb.end() && *it1 >= sstables.size())
			continue;
		//cout << "Before Merge" << sstables.size() << endl;

		string idStr = toString(idArray);
		map<string, long>::iterator mapIt = costMap.find(idStr);
		if (mapIt == costMap.end())
		{
			//count++;
			//cout << "After Merge\n";
			vector<long> output = KWayNumberMerge::merge(mergeSet, cost);
			costMap.insert(pair<string, long>(idStr, output.size()));
		}

		mapIt = costMap.find(idStr);
		if (minCost == INT_MAX || minCost > mapIt->second)
		{
			minCost = mapIt->second;
			minSet = singleComb;
		}
	}
	//cout << "Size of costMap " << costMap.size() << " sstable size " << sstables.size() <<  " count " << count << endl;
	//print_cost_map(costMap);
}

void mergeSets(vector< vector<long> > toMergeSet, long &mergeCost, vector<long> &output)
{
	output = KWayNumberMerge::merge(toMergeSet, mergeCost);
	//cout << "Thread Local Cost " << mergeCost << endl;
}

string toString(vector<long> idArray)
{
	sort(idArray.begin(), idArray.end());
	stringstream sstm;
	for (vector<long>::iterator it = idArray.begin(); it != idArray.end(); it++)
		sstm << *it << " ";
	return sstm.str();	
}

vector<long> toVector(string str)
{
	istringstream iss(str);
	vector<long> tokens((istream_iterator<long>(iss)),
                      istream_iterator<long>());

	return tokens;
}

template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q) {
    struct HackedQueue : private priority_queue<T, S, C> {
        static S& Container(priority_queue<T, S, C>& q) {
            return q.*&HackedQueue::c;
        }
    };
    return HackedQueue::Container(q);
}

void print_sets(priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap)
{
	vector< vector<long> > &sets = Container(fileHeap);

	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		cout << "Count:" << it->size() << endl;
		print_set(*it);
	}
	cout << "\n";
}

void print_sets(map<long, vector<long> > sets)
{
	for (map<long, vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		cout << "Index:" << it->first << " Size: " << it->second.size() << endl;
		print_set(it->second);
	}
	cout << "\n";
}

void print_set(vector<long> set)
{
	for (vector<long>::iterator it = set.begin(); it != set.end(); it++)
	{
		cout << *it << ",";
	}
	cout << "\n";
}
