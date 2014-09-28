#include<iostream>
#include"strategy.h"
#include"constants.h"
#include"kwaymerge.h"
#include <queue>

//test

vector< vector<long> > generateCombs(int N, int K)
{
	vector< vector<long> > combs;
	vector<long> oneComb;
    string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
 
    // print integers and permute bitmask
    do {
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) oneComb.push_back(i);
        }
        combs.push_back(oneComb);
		oneComb.clear();
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

	return combs;
}

long findGreedySet(vector< vector<long> > sstables, vector< vector<long> > combs, vector<long>& mergedSet, vector<long>& minSet)
{
	long minCost = INT_MAX, mergeCost;
	for (vector< vector<long> >::iterator it = combs.begin(); it != combs.end(); it++)
	{
		vector<long> singleComb = *it;
		vector< vector<long> > mergeSet;
		long cost = 0;
		for (vector<long>::iterator it1 = singleComb.begin(); it1 != singleComb.end(); it1++)
		{
			mergeSet.push_back(sstables[*it1]);
		}

		//cout << "Before Merge\n";
		vector<long> output = KWayNumberMerge::merge(mergeSet, cost);
		if (minCost == INT_MAX || minCost > output.size())
		{
			minCost = output.size();
			minSet = singleComb;
			mergedSet = output;
			mergeCost = cost;
		}
	}

	return mergeCost;
}

void print_set1(vector< vector<long> > sets)
{
	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		vector<long> set = *it;
		for (vector<long>::iterator bit = set.begin(); bit != set.end(); bit++)
		{
			cout << *bit << " ";
		}
		cout << "\n";
	}
}

long GreedyStrategy::compact()
{
	NumberParser *ycsbParser = dynamic_cast<NumberParser *>(mYCSBParser);
	vector< vector<long> > sstables, compactSStables;
	if (ycsbParser)
		sstables = ycsbParser->getSStables();

	//print_set1(sstables);
	long sstableCount, cost = 0;
	vector<long> output, compactSet;
	while (sstables.size() > 1)
	{
		sstableCount = sstables.size();
		compactSet.clear();
		output.clear();
		vector< vector<long> > combs = generateCombs(sstableCount, COMPACTION_THRESHOLD);
		//print_set1(combs);
		cost += findGreedySet(sstables, combs, output, compactSet);
		
		//Erasing the compacted set and adding the new set
		cout << "SStable Count:" << sstables.size() << " compact set:" << compactSet.size() << " output size:" << output.size() << endl;
		for (int i = 0; i < compactSet.size(); i++)
			sstables.erase(sstables.begin() + compactSet[i]);
		cout << "SStable Count:" << sstables.size() << endl;

		sstables.push_back(output);
	}
		
	return cost;
}

struct Comparator
{
  bool operator() (const vector<long> &lhs, const vector<long> &rhs) const
  {
    return lhs.size() > rhs.size();
  }
};

long SizeTieredStrategy::compact()
{
	YCSBParser *ycsbParser = dynamic_cast<YCSBParser *>(mFileParser);
	vector< vector<long> > sstables;
	if (ycsbParser)
		sstables = ycsbParser->getSStables();
	
	long mergeCost = 0;
	/*
	vector< vector<long> > p;
	long a[2] = {1,2};
	vector<long> a1(a, a + 2);
	long b[3] = {1,2,3};
	vector<long> b1(b, b+3);
	long c[6] = {1,2,3,4,5,6};
	vector<long> c1(c,c+6);
	p.push_back(c1);
	p.push_back(b1);
	p.push_back(a1);

	for(int i = 0; i < p.size();i++)
		cout << p[i].size() << endl;
	
	priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap(p.begin(), p.end());

	cout << "asdasd " << fileHeap.top().size()<< endl;
	fileHeap.pop();	
	cout << "asdasd " << fileHeap.top().size()<< endl;	
	fileHeap.pop();	
	cout << "asdasd " << fileHeap.top().size()<< endl;	
	fileHeap.pop();	
	*/
	
	if(sstables.size() == 0)
		return mergeCost;

	priority_queue<vector<long>, vector< vector<long> >, Comparator> fileHeap(sstables.begin(), sstables.end());
	
	while(fileHeap.size() >= 2) {
		vector< vector<long> > toMergeSet;
		long dummy_cost; //useless variable
		
		vector<long> f1 = fileHeap.top();
		fileHeap.pop();
		vector<long> f2 = fileHeap.top();
		fileHeap.pop();
		toMergeSet.push_back(f1);
		toMergeSet.push_back(f2);
		mergeCost = mergeCost + f1.size() + f2.size();
		
		vector<long> output = KWayNumberMerge::merge(toMergeSet, dummy_cost);
		fileHeap.push(output);

	}
	mergeCost += fileHeap.top().size();

	return mergeCost;
}

