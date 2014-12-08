#include <iostream>
#include <queue>
#include <cstdlib>
#include "filestrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"

long RandomFileStrategy::compact()
{
	vector<SStable> sstables = mOpts.getSStables();
	long numFiles = mOpts.getNumFiles();
	
	long mergeCost = 0, count = 1, index;
	
	if(sstables.size() == 0)
		return mergeCost;

	srand(time(NULL));
	
	while(sstables.size() >= consts.COMPACTION_THRESHOLD) {
		vector<SStable> toMergeSet;

		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		cout << "Merging ";
		for (int i = 0; i < consts.COMPACTION_THRESHOLD; i++)
		{
			index = rand() % sstables.size();
			cout << sstables[index].filename << " ";
			toMergeSet.push_back(sstables[index]);
			sstables.erase(sstables.begin() + index);
		}
		cout << "\n";

		SStable output = KWayFileMerge::merge(toMergeSet, numFiles++, mergeCost);
		//print_set1(toMergeSet);
		//cout << "Merging two sets of size " << f1.size() << " " << f2.size() << " to produce output of size " << output.size() << endl;
		cout << "Iteration Cost:" << count++ << " " << output.filename << " " << output.keyCount << endl;
		sstables.push_back(output);
	}

	if (sstables.size() > 1)
	{
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		cout << "Merging ";
		for (vector<SStable>::iterator it = sstables.begin(); it != sstables.end(); it++)
			cout << it->filename << " ";
		cout << "\n";

		SStable output = KWayFileMerge::merge(sstables, numFiles++, mergeCost);
		cout << "Iteration Cost:" << count++ << " " << output.filename << " " << output.keyCount << endl;
	}

	return mergeCost;
}
