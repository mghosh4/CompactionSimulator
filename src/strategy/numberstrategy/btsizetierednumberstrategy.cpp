#include <iostream>
#include <queue>
#include <cmath>
#include <boost/thread.hpp>
#include "numberstrategy.h"
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

long BTSizeTieredNumberStrategy::compact()
{
	vector< vector<long> > sstables = mOpts.getSStables();
	if(sstables.size() == 0)
		return 0;

	long maxSize = (long)(floor(sstables.size() / 2.0));
	long localCost[maxSize];
	vector<long> output[maxSize];
	boost::thread threads[maxSize];
	long count = 1, mergeCost = 0;
	
	priority_queue<vector<long>, vector< vector<long> >, NumberComparator> fileHeap(sstables.begin(), sstables.end());
	cout << "=====================================================\n";
	cout << "Iteration " << count << endl;
	cout << "=====================================================\n";
	print_sets(fileHeap);
	
	while(fileHeap.size() >= consts.COMPACTION_THRESHOLD) {
		int thrCount = 0;
		
		while (fileHeap.size() >= consts.COMPACTION_THRESHOLD)
		{
			vector< vector<long> > toMergeSet;

			for (int i = 0; i < consts.COMPACTION_THRESHOLD; i++)
			{
				toMergeSet.push_back(fileHeap.top());
				fileHeap.pop();
			}

			localCost[thrCount] = 0;
			threads[thrCount] = boost::thread(&mergeNumbers, toMergeSet, boost::ref(localCost[thrCount]), boost::ref(output[thrCount]));
			thrCount++;
		}

		for (int i = 0; i < thrCount; i++)
		{
			threads[i].join();
			//cout << "Local Cost:" << localCost[i] << "\n";
			mergeCost += localCost[i];
			cout << "Iteration Cost:" << count++ << " " << output[i].size() << endl;
			fileHeap.push(output[i]);
			cout << "=====================================================\n";
			cout << "Iteration " << count << endl;
			cout << "=====================================================\n";
			print_sets(fileHeap);
		}
	}

	if (fileHeap.size() >= 2)
	{
		vector< vector<long> > toMergeSet;
		while (fileHeap.size() != 0)
		{
			toMergeSet.push_back(fileHeap.top());
			fileHeap.pop();
		}

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
