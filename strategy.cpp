#include<iostream>
#include"strategy.h"
#include"constants.h"
#include"kwaymerge.h"

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
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

	return combs;
}

vector<long> findGreedySet(vector< vector<long> > sstables, vector< vector<long> > combs, vector< vector<long> > compactSStables)
{
	long minCost = INT_MAX;
	vector<long> minSet;
	for (vector< vector<long> >::iterator it = combs.begin(); it != combs.end(); it++)
	{
		vector<long> singleComb = *it;
		vector< vector<long> > mergeSet;
		long cost = 0;
		for (vector<long>::iterator it1 = singleComb.begin(); it1 != singleComb.end(); it1++)
		{
			mergeSet.push_back(sstables[*it1]);
		}

		vector<long> output = KWayNumberMerge::merge(mergeSet, cost);
		if (minCost == INT_MAX || minCost > output.size())
		{
			compactSStables = mergeSet;
			minCost = output.size();
			minSet = singleComb;
		}
	}

	return minSet;
}

long GreedyStrategy::compact()
{
	YCSBParser *ycsbParser = dynamic_cast<YCSBParser *>(mFileParser);
	vector< vector<long> > sstables, compactSStables;
	if (ycsbParser)
		sstables = ycsbParser->getSStables();

	long sstableCount, cost = 0;

	while (sstables.size() > 1)
	{
		sstableCount = sstables.size();
		compactSStables.clear();
		vector< vector<long> > combs = generateCombs(sstableCount, COMPACTION_THRESHOLD);
		vector<long> compactSet = findGreedySet(sstables, combs, compactSStables);
		vector<long> output = KWayNumberMerge::merge(compactSStables, cost);
		
		//Erasing the compacted set and adding the new set
		for (int i = 0; i < compactSet.size(); i++)
			sstables.erase(sstables.begin() + compactSet[i]);

		sstables.push_back(output);
	}
		
	return cost;
}

long SizeTieredStrategy::compact()
{
	cout << "O!! Implement me baby!! :)" << endl;
	return 0;
}

