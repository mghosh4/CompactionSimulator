#include <iostream>
#include <queue>
#include "numberstrategy.h"
#include "../utilities/constants.h"
#include "../utilities/kwaymerge.h"

struct Comparator
{
	bool operator() (const vector<long> &lhs, const vector<long> &rhs) const
	{
		return lhs.size() > rhs.size();
	}
};

long SizeTieredNumberStrategy::compact()
{
	vector< vector<long> > sstables = mOpts.getSStables();
	
	long mergeCost = 0;
	
	if(sstables.size() == 0)
		return mergeCost;

	priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap(sstables.begin(), sstables.end());
	
	while(fileHeap.size() >= 2) {
		vector< vector<long> > toMergeSet;

		for (int i = 0; i < COMPACTION_THRESHOLD; i++)
		{
			toMergeSet.push_back(fileHeap.top());
			fileHeap.pop();
		}
		
		vector<long> output = KWayNumberMerge::merge(toMergeSet, mergeCost);
		//print_set1(toMergeSet);
		//cout << "Merging two sets of size " << f1.size() << " " << f2.size() << " to produce output of size " << output.size() << endl;
		//cout << "Merge Cost " << mergeCost << endl;
		fileHeap.push(output);
	}

	return mergeCost;
}

