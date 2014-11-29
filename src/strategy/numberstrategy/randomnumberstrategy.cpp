#include <iostream>
#include <queue>
#include <cstdlib>
#include "numberstrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"

long RandomNumberStrategy::compact()
{
	vector< vector<long> > sstables = mOpts.getSStables();
	
	long mergeCost = 0, count = 1, index;
	
	if(sstables.size() == 0)
		return mergeCost;

	cout << "=====================================================\n";
	cout << "Iteration " << count << endl;
	cout << "=====================================================\n";
	print_sets(sstables);
	srand(time(NULL));
	
	while(sstables.size() >= COMPACTION_THRESHOLD) {
		vector< vector<long> > toMergeSet;

		for (int i = 0; i < COMPACTION_THRESHOLD; i++)
		{
			index = rand() % sstables.size();
			toMergeSet.push_back(sstables[index]);
			sstables.erase(sstables.begin() + index);
		}

		vector<long> output = KWayNumberMerge::merge(toMergeSet, mergeCost);
		//print_set1(toMergeSet);
		//cout << "Merging two sets of size " << f1.size() << " " << f2.size() << " to produce output of size " << output.size() << endl;
		cout << "Iteration Cost:" << count++ << " " << output.size() << endl;
		sstables.push_back(output);
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_sets(sstables);
	}

	if (sstables.size() > 1)
	{
		vector<long> output = KWayNumberMerge::merge(sstables, mergeCost);
		cout << "Iteration Cost:" << count++ << " " << output.size() << endl;
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_sets(sstables);
	}

	return mergeCost;
}
