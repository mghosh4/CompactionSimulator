#ifndef FILESTRATEGY_H
#define FILESTRATEGY_H

#include <vector>
#include "strategy.h"
#include "../sstable/sstable.h"

class FileStrategyOptions
{
	public:
		FileStrategyOptions(vector<SStable> sstables, long numFiles) : mSStables(sstables), mNumFiles(numFiles) {}
		vector<SStable> getSStables() {return mSStables;}
		long getNumFiles() {return mNumFiles;}
	private:
		long mNumFiles;
		vector<SStable> mSStables;
};

class SizeTieredFileStrategy : public SizeTieredStrategy
{
	public:
		SizeTieredFileStrategy(FileStrategyOptions& fsOpts) : mOpts(fsOpts) {}
		virtual long compact();
	private:
		FileStrategyOptions mOpts;
};

class GreedyFileStrategy : public GreedyStrategy
{
	public: 
		GreedyFileStrategy(FileStrategyOptions& fsOpts) : mOpts(fsOpts) {}
		virtual long compact();
	private:
		FileStrategyOptions mOpts;
};

#endif
