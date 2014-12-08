#include <iostream>
#include <map>
#include "numberstrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"

long GreedyNumberStrategy::compact()
{
	vector< vector<long> > sets, compactSStables;
	sets = mOpts.getSStables();
	if (sets.size() == 0)
		return 0;

	map<long, vector<long> > sstables;
	map<string, long> costMap;
	long lastId = 0;

	vector< vector<long> > combs = generateCombs(sets.size(), consts.COMPACTION_THRESHOLD);
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
		cout << "Iteration Cost:" << count++ << " " << output.size() << endl;
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
