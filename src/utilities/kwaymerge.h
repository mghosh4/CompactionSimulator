#ifndef KWAYMERGE_H
#define KWAYMERGE_H

#include <vector>
#include "../sstable/sstable.h"

using namespace std;

class KWayNumberMerge
{
	private:
		KWayNumberMerge() {}
	public:
		static vector<long>  merge(vector< vector<long> > sets, long &cost);
};

class KWayFileMerge
{
	private:
		KWayFileMerge() {}
	public:
		static SStable merge(vector<SStable> sets, long numFiles, long &cost);
};

#endif
