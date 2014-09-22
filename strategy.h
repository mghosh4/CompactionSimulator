#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include "fileparser.h"
using namespace std;
class Strategy
{
	public:
		Strategy(FileParser *fp) : mFileParser(fp) {}
		virtual long compact() = 0;
	protected:
		FileParser *mFileParser;
};

class GreedyStrategy : public Strategy
{
	public:
		GreedyStrategy(FileParser *fp) : Strategy(fp) {}
		virtual long compact();
};

class SizeTieredStrategy : public Strategy
{
	public:
		SizeTieredStrategy(FileParser *fp) : Strategy(fp) {}
		virtual long compact();
};

#endif
