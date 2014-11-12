#include <iostream>
#include <sstream>
#include <vector>
#include "utilities.h"

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

string toString(vector<long> idArray)
{
	sort(idArray.begin(), idArray.end());
	stringstream sstm;
	for (vector<long>::iterator it = idArray.begin(); it != idArray.end(); it++)
		sstm << *it << ":";
	return sstm.str();	
}

void print_set(vector<long> set)
{
	for (vector<long>::iterator it = set.begin(); it != set.end(); it++)
	{
		cout << *it << ",";
	}
	cout << "\n";
}
