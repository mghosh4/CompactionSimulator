#include <iostream>
#include <queue>
#include "numberstrategy.h"
#include "../utilities/constants.h"
#include "../utilities/utilities.h"
#include "../utilities/kwaymerge.h"

long SizeTieredNumberStrategy::compact()
{
	vector< vector<long> > sstables = mOpts.getSStables();
	
	long mergeCost = 0, count = 1;
	
	if(sstables.size() == 0)
		return mergeCost;

	priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap(sstables.begin(), sstables.end());
	cout << "=====================================================\n";
	cout << "Iteration " << count << endl;
	cout << "=====================================================\n";
	print_sets(fileHeap);
	
	while(fileHeap.size() >= COMPACTION_THRESHOLD) {
		vector< vector<long> > toMergeSet;

		for (int i = 0; i < COMPACTION_THRESHOLD; i++)
		{
			toMergeSet.push_back(fileHeap.top());
			fileHeap.pop();
		}
		
		vector<long> output = KWayNumberMerge::merge(toMergeSet, mergeCost);
		//print_set1(toMergeSet);
		//cout << "Merging two sets of size " << f1.size() << " " << f2.size() << " to produce output of size " << output.size() << endl;
		cout << "Iteration Cost:" << count++ << " " << output.size() << endl;
		fileHeap.push(output);
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_sets(fileHeap);
	}

	vector< vector<long> > toMergeSet;
	while (fileHeap.size() >= 2)
	{
		toMergeSet.push_back(fileHeap.top());
		fileHeap.pop();
	}

	if (toMergeSet.size())
	{
		vector<long> output = KWayNumberMerge::merge(toMergeSet, mergeCost);
		cout << "Iteration Cost:" << count++ << " " << output.size() << endl;
		fileHeap.push(output);
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_sets(fileHeap);
	}

	return mergeCost;
}
