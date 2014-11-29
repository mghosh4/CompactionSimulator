#include <iostream>
#include <queue>
#include <cmath>
#include <boost/thread.hpp>
#include "filestrategy.h"
#include "../../utilities/constants.h"
#include "../../utilities/utilities.h"
#include "../../utilities/kwaymerge.h"

template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q) {
    struct HackedQueue : private priority_queue<T, S, C> {
        static S& Container(priority_queue<T, S, C>& q) {
            return q.*&HackedQueue::c;
        }
    };
    return HackedQueue::Container(q);
}

long BTSizeTieredFileStrategy::compact()
{
	vector<SStable> sstables = mOpts.getSStables();
	long numFiles = mOpts.getNumFiles();
	if(sstables.size() == 0)
		return 0;

	long maxSize = (long)(floor(sstables.size() / 2.0));
	long localCost[maxSize];
	SStable output[maxSize];
	boost::thread threads[maxSize];
	long count = 1, mergeCost = 0;
	
	priority_queue<SStable, vector<SStable>, FileComparator> fileHeap(sstables.begin(), sstables.end());
	
	while(fileHeap.size() >= COMPACTION_THRESHOLD) {
		int thrCount = 0;
		
		while (fileHeap.size() >= COMPACTION_THRESHOLD)
		{
			vector<SStable> toMergeSet;

			for (int i = 0; i < COMPACTION_THRESHOLD; i++)
			{
				toMergeSet.push_back(fileHeap.top());
				fileHeap.pop();
			}

			cout << "=====================================================\n";
			cout << "Iteration " << thrCount + count << endl;
			cout << "=====================================================\n";
			cout << "Merging ";
			for (int i = 0; i < COMPACTION_THRESHOLD; i++)
				cout << toMergeSet[i].filename << " ";
			cout << "\n";

			localCost[thrCount] = 0;
			threads[thrCount] = boost::thread(&mergeFiles, toMergeSet, boost::ref(localCost[thrCount]), numFiles++, boost::ref(output[thrCount]));
			thrCount++;
		}

		for (int i = 0; i < thrCount; i++)
		{
			threads[i].join();
			//cout << "Local Cost:" << localCost[i] << "\n";
			mergeCost += localCost[i];
			cout << "Iteration Cost:" << count++ << " " << output[i].filename << " " << output[i].keyCount << endl;
			fileHeap.push(output[i]);
		}
	}

	if (fileHeap.size() >= 2)
	{
		vector<SStable> toMergeSet;
		while (fileHeap.size() != 0)
		{
			toMergeSet.push_back(fileHeap.top());
			fileHeap.pop();
		}

		SStable output = KWayFileMerge::merge(toMergeSet, numFiles, mergeCost);
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		cout << "Merging ";
		for (int i = 0; i < COMPACTION_THRESHOLD; i++)
			cout << toMergeSet[i].filename << " ";
		cout << "Iteration Cost:" << count++ << " " << output.filename << " " << output.keyCount << endl;
		fileHeap.push(output);
	}

	return mergeCost;
}
