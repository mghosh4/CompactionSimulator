#include<iostream>
#include"strategy.h"
#include"constants.h"
#include"kwaymerge.h"
#include <queue>

struct Comparator
{
	bool operator() (const vector<long> &lhs, const vector<long> &rhs) const
	{
		return lhs.size() > rhs.size();
	}
};

long SizeTieredStrategy::compact()
{
	NumberParser *ycsbParser = dynamic_cast<NumberParser *>(mYCSBParser);
	vector< vector<long> > sstables;
	if (ycsbParser)
		sstables = ycsbParser->getSStables();
	
	long mergeCost = 0;
	
	if(sstables.size() == 0)
		return mergeCost;

	priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap(sstables.begin(), sstables.end());
	
	while(fileHeap.size() >= 2) {
		vector< vector<long> > toMergeSet;
		
		vector<long> f1 = fileHeap.top();
		fileHeap.pop();
		vector<long> f2 = fileHeap.top();
		fileHeap.pop();
		toMergeSet.push_back(f1);
		toMergeSet.push_back(f2);
		
		vector<long> output = KWayNumberMerge::merge(toMergeSet, mergeCost);
		//print_set1(toMergeSet);
		//cout << "Merging two sets of size " << f1.size() << " " << f2.size() << " to produce output of size " << output.size() << endl;
		//cout << "Merge Cost " << mergeCost << endl;
		fileHeap.push(output);
	}

	return mergeCost;
}

