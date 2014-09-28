#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include "fileparser.h"
using namespace std;
class Strategy
{
	public:
		Strategy(YCSBParser *fp) : mYCSBParser(fp) {}
		virtual long compact() = 0;
	protected:
		YCSBParser *mYCSBParser;
};

class GreedyStrategy : public Strategy
{
	public:
		GreedyStrategy(YCSBParser *fp) : Strategy(fp) {}
		virtual long compact();
};

class SizeTieredStrategy : public Strategy
{
	public:
		SizeTieredStrategy(YCSBParser *fp) : Strategy(fp) {}
		virtual long compact();
};

#endif
