#ifndef FILESTRATEGY_H
#define FILESTRATEGY_H

#include "strategy.h"

class FileStrategyOptions
{
	public:
		FileStrategyOptions(long numFiles) : mNumFiles(numFiles) {}
		long getNumFiles() {return mNumFiles;}
	private:
		long mNumFiles;
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
