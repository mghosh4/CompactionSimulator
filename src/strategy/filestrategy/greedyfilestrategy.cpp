#include <iostream>
#include <map>
#include "filestrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"
#include "../../utilities/timer.h"

long GreedyFileStrategy::compact()
{
	vector<SStable> sets, compactSStables;
	sets = mOpts.getSStables();
	long numFiles = mOpts.getNumFiles();
	if (sets.size() == 0)
		return 0;

	map<long, SStable> sstables;
	map<string, double> costMap;
	long lastId = 0;

	Timer tm;
	long mergeTime = 0, planTime = 0;

	vector< vector<long> > combs = generateCombs(sets.size(), consts.COMPACTION_THRESHOLD);
	int indexMap[sets.size()];
	int indexMapCount = sets.size();
	for (vector<SStable>::iterator it = sets.begin(); it != sets.end(); it++)
	{
		indexMap[lastId] = lastId;
		sstables.insert(pair<long, SStable>(lastId++, *it));
	}

	sets.clear();

	//print_set1(sstables);
	long cost = 0, count = 1;
	while (sstables.size() > 1)
	{
		vector<long> compactSet;

		tm.start();
		//print_set1(combs);
		findGreedySet(sstables, combs, compactSet, costMap, indexMap, getCardinalityFn());
		tm.stop();
		planTime += tm.duration();
		cout << "Planning Time at Iteration " << count << " is " << tm.duration() << endl;
		
		//Erasing the compacted set and adding the new set
		vector<SStable> toMerge;
		//cout << "Remove ";
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		cout << "Compact Set:";
		for (int i = 0; i < compactSet.size(); i++)
		{
			cout << sstables[indexMap[compactSet[i]]].filename << " ";
			cout << indexMap[compactSet[i]] << " ";
			toMerge.push_back(sstables[indexMap[compactSet[i]]]);
			sstables.erase(sstables.find(indexMap[compactSet[i]]));
		}
		cout << "\n";
		tm.start();
		SStable output = KWayFileMerge::merge(toMerge, numFiles++, cost);
		tm.stop();
		mergeTime += tm.duration();

		cout << "Iteration Cost:" << count++ << " " << output.keyCount << endl;
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

	cout << "GreedyStrategy Merge Time:" << mergeTime << endl;

	return cost;
}
