#include <iostream>
#include <queue>
#include "numberstrategy.h"
#include "../utilities/constants.h"
#include "../utilities/utilities.h"
#include "../utilities/kwaymerge.h"

struct Comparator
{
	bool operator() (const vector<long> &lhs, const vector<long> &rhs) const
	{
		return lhs.size() > rhs.size();
	}
};

template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q) {
    struct HackedQueue : private priority_queue<T, S, C> {
        static S& Container(priority_queue<T, S, C>& q) {
            return q.*&HackedQueue::c;
        }
    };
    return HackedQueue::Container(q);
}

void print_sets(priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap)
{
	vector< vector<long> > &sets = Container(fileHeap);

	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		cout << "Count:" << it->size() << endl;
		print_set(*it);
	}
	cout << "\n";
}

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
		cout << "Iteration Cost:" << count++ << " " << mergeCost << endl;
		fileHeap.push(output);
		cout << "=====================================================\n";
		cout << "Iteration " << count << endl;
		cout << "=====================================================\n";
		print_sets(fileHeap);
	}

	return mergeCost;
}
