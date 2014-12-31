#include <iostream>
#include <queue>
#include "filestrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"
#include "../../utilities/timer.h"

long SizeTieredFileStrategy::compact()
{
	vector<SStable> sstables = mOpts.getSStables();
	long numFiles = mOpts.getNumFiles();
	
	Timer tm;
	long mergeCost = 0, count = 1;
	int mergeTime = 0;
	
	if(sstables.size() == 0)
		return mergeCost;

	priority_queue<SStable, vector<SStable>, FileComparator> fileHeap(sstables.begin(), sstables.end());
	
	while(fileHeap.size() >= consts.COMPACTION_THRESHOLD) {
		vector<SStable> toMergeSet;

		for (int i = 0; i < consts.COMPACTION_THRESHOLD; i++)
		{
			toMergeSet.push_back(fileHeap.top());
			fileHeap.pop();
		}
		
		tm.start();
		SStable output = KWayFileMerge::merge(toMergeSet, numFiles++, mergeCost);
		tm.stop();
		mergeTime += tm.duration();
		//print_set1(toMergeSet);
		//cout << "Merging two sets of size " << f1.size() << " " << f2.size() << " to produce output of size " << output.size() << endl;
		//cout << "Merge Cost " << mergeCost << endl;
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		cout << "Merging ";
		for (int i = 0; i < consts.COMPACTION_THRESHOLD; i++)
			cout << toMergeSet[i].filename << " ";
		cout << "--->" << output.filename << "\n";
		cout << "Iteration Cost:" << count++ << " " << output.keyCount << endl;
		fileHeap.push(output);
	}

	if (fileHeap.size() >= 2)
	{
		vector<SStable> toMergeSet;
		while (fileHeap.size() != 0)
		{
			toMergeSet.push_back(fileHeap.top());
			fileHeap.pop();
		}

		tm.start();
		SStable output = KWayFileMerge::merge(toMergeSet, numFiles++, mergeCost);
		tm.stop();
		mergeTime += tm.duration();
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		cout << "Merging ";
		for (int i = 0; i < consts.COMPACTION_THRESHOLD; i++)
			cout << toMergeSet[i].filename << " ";
		cout << "--->" << output.filename << "\n";
		cout << "Iteration Cost:" << count++ << " " << output.keyCount << endl;
		fileHeap.push(output);
	}
	cout << "SizeTieredStrategy Merge Time:" << mergeTime << endl;

	return mergeCost;
}
