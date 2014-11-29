#include <iostream>
#include <queue>
#include <cmath>
#include <boost/thread.hpp>
#include "numberstrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"

pair<long, string> createKeyValue(const pair<string, long>& keyValue)
{
	return pair<long, string>(keyValue.second, keyValue.first);
}

long BTGreedyNumberStrategy::compact()
{
	vector< vector<long> > sets = mOpts.getSStables();
	if(sets.size() == 0)
		return 0;

	long maxSize = (long)(floor(sets.size() / 2.0));
	long localCost[maxSize];
	vector<long> output[maxSize];
	boost::thread threads[maxSize];
	long count = 1, mergeCost = 0;

	while (sets.size() >= COMPACTION_THRESHOLD)
	{
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

		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_sets(sstables);

		vector<long> compactSet;
		findGreedySet(sstables, combs, compactSet, costMap, indexMap);
		int thrCount = 0;

		vector< pair<long, string> > costMapInv(costMap.size());

		transform(costMap.begin(), costMap.end(), costMapInv.begin(), createKeyValue);
		sort(costMapInv.begin(), costMapInv.end());

		for (vector< pair<long, string> >::iterator it = costMapInv.begin(); it != costMapInv.end(); it++)
		{
			vector<long> toMergeSet = toVector(it->second);
			vector< vector<long> > toMerge;
			bool notValidComb = false;

			//cout << "Merge Set:";
			for (vector<long>::iterator vit = toMergeSet.begin(); vit != toMergeSet.end(); vit++)
			{
				//cout << *vit << " ";
				if (sstables.find(indexMap[*vit]) == sstables.end())
				{
					notValidComb = true;
					break;
				}

				toMerge.push_back(sstables[indexMap[*vit]]);
			}
			//cout << "\n";

			if (notValidComb)
				continue;

			for (vector<long>::iterator vit = toMergeSet.begin(); vit != toMergeSet.end(); vit++)
				sstables.erase(sstables.find(indexMap[*vit]));

			//cout << "Merging:" << it->first << " " << it->second << "\n";
			localCost[thrCount] = 0;
			threads[thrCount] = boost::thread(&mergeNumbers, toMerge, boost::ref(localCost[thrCount]), boost::ref(output[thrCount]));
			thrCount++;
		}

		for (map<long, vector<long> >::iterator mit = sstables.begin(); mit != sstables.end(); mit++)
			sets.push_back(mit->second);

		for (int i = 0; i < thrCount; i++)
		{
			threads[i].join();
			//cout << "Local Cost:" << localCost[i] << "\n";
			mergeCost += localCost[i];
			cout << "Iteration Cost:" << count++ << " " << output[i].size() << endl;
			sets.push_back(output[i]);
		}
	}

	if (sets.size() >= 2)
	{
		vector<long> output = KWayNumberMerge::merge(sets, mergeCost);
		cout << "Iteration Cost:" << count++ << " " << output.size() << endl;
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_set(output);
	}
	else
	{
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_set(sets[0]);
	}

	return mergeCost;
}
