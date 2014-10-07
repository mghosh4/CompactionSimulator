#include <iostream>
#include <sstream>
#include <map>
#include "numberstrategy.h"
#include "../utilities/constants.h"
#include "../utilities/kwaymerge.h"

struct classcomp {
  	bool operator() (const string& lhs, const string& rhs) const
  	{
		if (lhs.compare(rhs) < 0)
			return true;
		else
			return false;
	}
};

vector< vector<long> > generateCombs(vector<long> toCombine, int K)
{
	vector< vector<long> > combs;
	vector<long> oneComb;
	int N = toCombine.size();
    string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
 
    // print integers and permute bitmask
    do {
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) oneComb.push_back(toCombine[i]);
        }
        combs.push_back(oneComb);
		oneComb.clear();
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

	return combs;
}

string toString(vector<long> idArray)
{
	sort(idArray.begin(), idArray.end());
	stringstream sstm;
	for (vector<long>::iterator it = idArray.begin(); it != idArray.end(); it++)
		sstm << *it << ":";
	return sstm.str();	
}

void print_set2(map<string, long, classcomp> costMap)
{
	for (map<string, long, classcomp>::iterator it = costMap.begin(); it != costMap.end(); it++)
	{
		cout << it->first << " " << it->second << "\n";
	}
}

void findGreedySet(map<long, vector<long> > sstables, vector< vector<long> > combs, vector<long>& minSet, map<string, long, classcomp>& costMap)
{
	cout << "Size of costMap " << costMap.size() << " sstable size " << sstables.size() << endl;
	long minCost = INT_MAX, mergeCost;
	//long count = 0;
	for (vector< vector<long> >::iterator it = combs.begin(); it != combs.end(); it++)
	{
		vector<long> singleComb = *it;
		
		vector< vector<long> > mergeSet;
		vector<long> idArray;
		long cost = 0;
		for (vector<long>::iterator it1 = singleComb.begin(); it1 != singleComb.end(); it1++)
		{
			idArray.push_back(*it1);
			mergeSet.push_back(sstables[*it1]);
		}

		string idStr = toString(idArray);
		map<string, long, classcomp>::iterator mapIt = costMap.find(idStr);
		if (mapIt == costMap.end())
		{
			//count++;
			//cout << "Before Merge\n";
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
	//print_set2(costMap);
}

void print_set1(vector< vector<long> > sets)
{
	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		vector<long> set = *it;
		for (vector<long>::iterator bit = set.begin(); bit != set.end(); bit++)
		{
			cout << *bit << " ";
		}
		cout << "\n";
	}
}

long GreedyNumberStrategy::compact()
{
	vector< vector<long> > sets, compactSStables;
	sets = mOpts.getSStables();

	map<long, vector<long> > sstables;
	map<string, long, classcomp> costMap;
	long lastId = 0;

	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
		sstables.insert(pair<long, vector<long> >(lastId++, *it));

	sets.clear();

	//print_set1(sstables);
	long cost = 0;
	while (sstables.size() > 1)
	{
		vector<long> toCombine, compactSet;
		for (map<long, vector<long> >::iterator it = sstables.begin(); it != sstables.end(); it++)
			toCombine.push_back(it->first);

		vector< vector<long> > combs = generateCombs(toCombine, COMPACTION_THRESHOLD);
		//print_set1(combs);
		findGreedySet(sstables, combs, compactSet, costMap);
		
		//Erasing the compacted set and adding the new set
		vector< vector<long> > toMerge;
		for (int i = 0; i < compactSet.size(); i++)
		{
			toMerge.push_back(sstables[compactSet[i]]);
			sstables.erase(sstables.find(compactSet[i]));
		}
		//cout << "SStable Count:" << sstables.size() << endl;
		vector<long> output = KWayNumberMerge::merge(toMerge, cost);
		//cout << "SStable Count:" << sstables.size() << " compact set:" << compactSet.size() << " output size:" << output.size() << endl;

		sstables.insert(pair<long, vector<long> >(lastId++, output));
	}
		
	return cost;
}

