#include <iostream>
#include <queue>
#include <cmath>
#include <boost/thread.hpp>
#include "filestrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"

pair<double, string> createKeyValue(const pair<string, double>& keyValue)
{
	return pair<double, string>(keyValue.second, keyValue.first);
}

long BTGreedyFileStrategy::compact()
{
	vector<SStable> sets = mOpts.getSStables();
	long numFiles = mOpts.getNumFiles();	
	if(sets.size() == 0)
		return 0;

	long maxSize = (long)(floor(sets.size() / 2.0));
	long localCost[maxSize];
	SStable output[maxSize];
	boost::thread threads[maxSize];
	long count = 1, mergeCost = 0;

	while (sets.size() >= COMPACTION_THRESHOLD)
	{
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

		vector<long> compactSet;
		findGreedySet(sstables, combs, compactSet, costMap, indexMap);
		int thrCount = 0;

		vector< pair<double, string> > costMapInv(costMap.size());

		transform(costMap.begin(), costMap.end(), costMapInv.begin(), createKeyValue);
		sort(costMapInv.begin(), costMapInv.end());

		for (vector< pair<double, string> >::iterator it = costMapInv.begin(); it != costMapInv.end(); it++)
		{
			vector<long> toMergeSet = toVector(it->second);
			vector<SStable> toMerge;
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

			cout << "=====================================================\n";
			cout << "Iteration " << thrCount + count << endl;
			cout << "=====================================================\n";
			cout << "Merging ";
			for (vector<long>::iterator vit = toMergeSet.begin(); vit != toMergeSet.end(); vit++)
			{
				cout << sstables[indexMap[*vit]].filename << " ";
				sstables.erase(sstables.find(indexMap[*vit]));
			}
			cout << "\n";

			//cout << "Merging:" << it->first << " " << it->second << "\n";
			localCost[thrCount] = 0;
			threads[thrCount] = boost::thread(&mergeFiles, toMerge, boost::ref(localCost[thrCount]), numFiles++, boost::ref(output[thrCount]));
			thrCount++;
		}

		for (map<long, SStable>::iterator mit = sstables.begin(); mit != sstables.end(); mit++)
			sets.push_back(mit->second);

		for (int i = 0; i < thrCount; i++)
		{
			threads[i].join();
			//cout << "Local Cost:" << localCost[i] << "\n";
			mergeCost += localCost[i];
			cout << "Iteration Cost:" << count++ << " " << output[i].filename << " " << output[i].keyCount << endl;
			sets.push_back(output[i]);
		}
	}

	if (sets.size() >= 2)
	{
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		cout << "Merging ";
		for (vector<SStable>::iterator vit = sets.begin(); vit != sets.end(); vit++)
			cout << vit->filename << " ";
		cout << "\n";
		SStable output = KWayFileMerge::merge(sets, numFiles++, mergeCost);
		cout << "Iteration Cost:" << count++ << " " << output.keyCount << endl;
	}

	return mergeCost;
}
