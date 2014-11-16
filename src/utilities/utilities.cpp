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

void print_set(vector<long> set)
{
	for (vector<long>::iterator it = set.begin(); it != set.end(); it++)
	{
		cout << *it << ",";
	}
	cout << "\n";
}
