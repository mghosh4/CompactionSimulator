#include<iostream>
#include"strategy.h"
#include"constants.h"
#include"kwaymerge.h"

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

long SizeTieredStrategy::compact()
{
	cout << "O!! Implement me baby!! :)" << endl;
	return 0;
}

