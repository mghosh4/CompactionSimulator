#include <iostream>
#include <queue>
#include "filestrategy.h"
#include "../utilities/constants.h"
#include "../utilities/kwaymerge.h"

struct Comparator
{
	bool operator() (const SStable &lhs, const SStable &rhs) const
	{
		return lhs.keyCount > rhs.keyCount;
	}
};

long SizeTieredFileStrategy::compact()
{
	vector<SStable> sstables = mOpts.getSStables();
	long numFiles = mOpts.getNumFiles();
	
	long mergeCost = 0;
	
	if(sstables.size() == 0)
		return mergeCost;

	priority_queue<SStable, vector<SStable>, Comparator> fileHeap(sstables.begin(), sstables.end());
	
	while(fileHeap.size() >= 2) {
		vector<SStable> toMergeSet;

		for (int i = 0; i < COMPACTION_THRESHOLD; i++)
		{
			toMergeSet.push_back(fileHeap.top());
			fileHeap.pop();
		}
		
		SStable output = KWayFileMerge::merge(toMergeSet, numFiles++, mergeCost);
		//print_set1(toMergeSet);
		//cout << "Merging two sets of size " << f1.size() << " " << f2.size() << " to produce output of size " << output.size() << endl;
		//cout << "Merge Cost " << mergeCost << endl;
		fileHeap.push(output);
	}

	return mergeCost;
}
