#ifndef KWAYMERGE_H
#define KWAYMERGE_H

#include<vector>
using namespace std;

class KWayNumberMerge
{
	private:
		KWayNumberMerge() {}
	public:
		static vector<long>  merge(vector< vector<long> > sets, long &cost);
};

#endif
