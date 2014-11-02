#include <iostream>
#include <map>
#include "filestrategy.h"
#include "../utilities/constants.h"
#include "../utilities/utilities.h"
#include "../utilities/kwaymerge.h"

void print_set2(map<string, double> costMap)
{
	for (map<string, double>::iterator it = costMap.begin(); it != costMap.end(); it++)
	{
		cout << it->first << " " << it->second << "\n";
	}
}

void findGreedySet(map<long, SStable> sstables, vector< vector<long> > combs, vector<long>& minSet, map<string, double>& costMap, int indexMap[])
{
	//cout << "Size of costMap " << costMap.size() << " sstable size " << sstables.size() << endl;
	double minCost = INT_MAX, mergeCost;
	//long count = 0;
	for (vector< vector<long> >::iterator it = combs.begin(); it != combs.end(); it++)
	{
		vector<long> singleComb = *it;
		
		vector<SStable> mergeSet;
		vector<long> idArray;
		long cost = 0;
		vector<long>::iterator it1 = singleComb.begin();
		for (; it1 != singleComb.end(); it1++)
		{
			if (*it1 >= sstables.size())
				break;
			idArray.push_back(indexMap[*it1]);
			mergeSet.push_back(sstables[indexMap[*it1]]);
		}

		// Case when we hit upon an index which is outside the current range of sstables
		if (it1 != singleComb.end() && *it1 >= sstables.size())
			continue;		

		string idStr = toString(idArray);
		map<string, double>::iterator mapIt = costMap.find(idStr);
		if (mapIt == costMap.end())
		{
			HyperLogLog unionHll;
			for (vector<SStable>::iterator it1 = mergeSet.begin(); it1 != mergeSet.end(); it1++)
				unionHll.merge(it1->hll);
			
			costMap.insert(pair<string, double>(idStr, unionHll.estimate()));
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

/*void print_set1(vector< vector<long> > sets)
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
}*/

long GreedyFileStrategy::compact()
{
	vector<SStable> sets, compactSStables;
	sets = mOpts.getSStables();
	long numFiles = mOpts.getNumFiles();

	map<long, SStable> sstables;
	map<string, double> costMap;
	long lastId = 0;

	vector< vector<long> > combs = generateCombs(sets.size(), COMPACTION_THRESHOLD);
	int indexMap[sets.size()];
	int indexMapCount = sets.size();
	for (vector<SStable>::iterator it = sets.begin(); it != sets.end(); it++)
	{
		indexMap[lastId] = lastId;
		sstables.insert(pair<long, SStable>(lastId++, *it));
	}

	sets.clear();

	//print_set1(sstables);
	long cost = 0;
	while (sstables.size() > 1)
	{
		vector<long> compactSet;

		//print_set1(combs);
		findGreedySet(sstables, combs, compactSet, costMap, indexMap);
		
		//Erasing the compacted set and adding the new set
		vector<SStable> toMerge;
		//cout << "Remove ";
		for (int i = 0; i < compactSet.size(); i++)
		{
			//cout << indexMap[compactSet[i]] << " ";
			toMerge.push_back(sstables[indexMap[compactSet[i]]]);
			sstables.erase(sstables.find(indexMap[compactSet[i]]));
		}
		//cout << "\n";
		SStable output = KWayFileMerge::merge(toMerge, numFiles++, cost);
		//cout << "SStable Count:" << sstables.size() << " compact set:" << compactSet.size() << " output size:" << output.size() << endl;

		//cout << "Compact Set:";
		sstables.insert(pair<long, SStable>(lastId, output));
		sort(compactSet.begin(), compactSet.end());
		for (int i = compactSet.size() - 1; i >= 0; i--)
		{
			//cout << compactSet[i] << " ";
			for (int j = compactSet[i]; j < indexMapCount; j++)
				indexMap[j] = indexMap[j+1];
			indexMapCount--;
		}
		//cout << "\n";

		indexMap[indexMapCount++] = lastId++;
		//cout << "SStable Count:" << sstables.size() << endl;
	}

	return cost;
}
