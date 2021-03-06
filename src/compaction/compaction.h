#ifndef COMPACTION_H
#define COMPACTION_H

#include <vector>
#include "../strategy/strategy.h"
#include "../sstable/sstable.h"
using namespace std;

class Compaction
{
	public:
		Compaction() {}
		void compare();
	protected:
		SizeTieredStrategy *s;
		GreedyStrategy *g;
		GreedyStrategy *gh;
		BTSizeTieredStrategy *bs;
		BTGreedyStrategy *bg;
		RandomStrategy *r;
};

class NumberCompaction : public Compaction
{
	public:
		NumberCompaction(vector<vector<long> > sstables);
};

class FileCompaction : public Compaction
{
	public:
		FileCompaction(vector<SStable> sstables, long numFiles);
};

#endif
