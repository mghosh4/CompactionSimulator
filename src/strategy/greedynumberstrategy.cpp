#include <iostream>
#include <map>
#include "numberstrategy.h"
#include "../utilities/constants.h"
#include "../utilities/utilities.h"
#include "../utilities/kwaymerge.h"

void print_cost_map(map<string, long> &costMap)
{
	for (map<string, long>::iterator it = costMap.begin(); it != costMap.end(); it++)
	{
		cout << it->first << " " << it->second << "\n";
	}
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
			costMap.insert(pair<string, long>(idStr, cost));
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

void print_sets(map<long, vector<long> > sets)
{
	for (map<long, vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		cout << "Index:" << it->first << " Size: " << it->second.size() << endl;
		print_set(it->second);
	}
	cout << "\n";
}

long GreedyNumberStrategy::compact()
{
	vector< vector<long> > sets, compactSStables;
	sets = mOpts.getSStables();

	map<long, vector<long> > sstables;
	map<string, long> costMap;
	long lastId = 0;

	vector< vector<long> > combs = generateCombs(sets.size(), COMPACTION_THRESHOLD);
	int indexMap[sets.size()];
	int indexMapCount = sets.size();
	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		indexMap[lastId] = lastId;
		sstables.insert(pair<long, vector<long> >(lastId++, *it));
	}

	sets.clear();

	long cost = 0, count = 1;
	cout << "=====================================================\n";
	cout << "Iteration " << count << endl;
	cout << "=====================================================\n";
	print_sets(sstables);
	while (sstables.size() > 1)
	{
		vector<long> compactSet;
		/*cout << "Index Map:";
		for (int i = 0; i < indexMapCount; i++)
			cout << indexMap[i] << " ";
		cout << "\n";*/

		findGreedySet(sstables, combs, compactSet, costMap, indexMap);
		
		//Erasing the compacted set and adding the new set
		vector< vector<long> > toMerge;
		cout << "Compact Set:";
		for (int i = 0; i < compactSet.size(); i++)
		{
			cout << indexMap[compactSet[i]] << " "; 
			toMerge.push_back(sstables[indexMap[compactSet[i]]]);
			sstables.erase(sstables.find(indexMap[compactSet[i]]));
		}
		cout << "\n";
		//cout << "SStable Count:" << sstables.size() << endl;
		vector<long> output = KWayNumberMerge::merge(toMerge, cost);
		cout << "Iteration Cost:" << count++ << " " << cost << endl;
		//cout << "SStable Count:" << sstables.size() << " compact set:" << compactSet.size() << " output size:" << output.size() << endl;

		//cout << "Compact Set:";
		sstables.insert(pair<long, vector<long> >(lastId, output));
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
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_sets(sstables);
	}

	return cost;
}
