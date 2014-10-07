#ifndef COMPACTION_H
#define COMPACTION_H

#include "../strategy/strategy.h"
#include <vector>
using namespace std;

class Compaction
{
	public:
		Compaction() {}
		void compare();
	protected:
		SizeTieredStrategy *s;
		GreedyStrategy *g;
};

class NumberCompaction : public Compaction
{
	public:
		NumberCompaction(vector<vector<long> > sstables);
};

class FileCompaction : public Compaction
{
	public:
		FileCompaction(long numFiles);
};

#endif
