#ifndef NUMBERSTRATEGY_H
#define NUMBERSTRATEGY_H

#include<vector>
#include "strategy.h"
using namespace std;

class NumberStrategyOptions
{
	public:
		NumberStrategyOptions(vector<vector<long> > sstables) : mSStables(sstables) {}
		vector<vector<long> > getSStables() {return mSStables;}
	private:
		vector<vector<long> > mSStables;
};

class SizeTieredNumberStrategy : public SizeTieredStrategy
{
	public:
		SizeTieredNumberStrategy(NumberStrategyOptions& nsOpts) : mOpts(nsOpts) {}
		virtual long compact();
	private:
		NumberStrategyOptions mOpts;
};

class GreedyNumberStrategy : public GreedyStrategy
{
	public: 
		GreedyNumberStrategy(NumberStrategyOptions& nsOpts) : mOpts(nsOpts) {}
		virtual long compact();
	private:
		NumberStrategyOptions mOpts;
};

class BTSizeTieredNumberStrategy : public BTSizeTieredStrategy
{
	public:
		BTSizeTieredNumberStrategy(NumberStrategyOptions& nsOpts) : mOpts(nsOpts) {}
		virtual long compact();
	private:
		NumberStrategyOptions mOpts;
};

class BTGreedyNumberStrategy : public BTGreedyStrategy
{
	public:
		BTGreedyNumberStrategy(NumberStrategyOptions& nsOpts) : mOpts(nsOpts) {}
		virtual long compact();
	private:
		NumberStrategyOptions mOpts;
};

#endif
