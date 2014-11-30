#ifndef FILESTRATEGY_H
#define FILESTRATEGY_H

#include <vector>
#include "../strategy.h"
#include "../../sstable/sstable.h"
#include "../../utilities/utilities.h"


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
		virtual cardfnptr getCardinalityFn() = 0;
	private:
		FileStrategyOptions mOpts;
};

class GreedyHLLFileStrategy : public GreedyFileStrategy
{
	public: 
		GreedyHLLFileStrategy(FileStrategyOptions& fsOpts) : GreedyFileStrategy(fsOpts) {}
		cardfnptr getCardinalityFn() {return &estimateCardinalityHL;}
};

class GreedyBFFileStrategy : public GreedyFileStrategy
{
	public: 
		GreedyBFFileStrategy(FileStrategyOptions& fsOpts) : GreedyFileStrategy(fsOpts) {}
		cardfnptr getCardinalityFn() {return &estimateCardinalityBF;}
};

class BTSizeTieredFileStrategy : public BTSizeTieredStrategy
{
	public:
		BTSizeTieredFileStrategy(FileStrategyOptions& nsOpts) : mOpts(nsOpts) {}
		virtual long compact();
	private:
		FileStrategyOptions mOpts;
};

class BTGreedyFileStrategy : public BTGreedyStrategy
{
	public:
		BTGreedyFileStrategy(FileStrategyOptions& nsOpts) : mOpts(nsOpts) {}
		virtual long compact();
	private:
		FileStrategyOptions mOpts;
};

class RandomFileStrategy : public RandomStrategy
{
	public:
		RandomFileStrategy(FileStrategyOptions& nsOpts) : mOpts(nsOpts) {}
		virtual long compact();
	private:
		FileStrategyOptions mOpts;
};

#endif
