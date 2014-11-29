#ifndef STRATEGY_H
#define STRATEGY_H

class Strategy
{
	public:
		Strategy() {}
		virtual long compact() = 0;
};

class GreedyStrategy : public Strategy
{
	public:
		GreedyStrategy() {}
		virtual long compact() = 0;
};

class SizeTieredStrategy : public Strategy
{
	public:
		SizeTieredStrategy() {}
		virtual long compact() = 0;
};

class BTSizeTieredStrategy : public Strategy
{
	public:
		BTSizeTieredStrategy() {}
		virtual long compact() = 0;
};

class BTGreedyStrategy : public Strategy
{
	public:
		BTGreedyStrategy() {}
		virtual long compact() = 0;
};

class RandomStrategy : public Strategy
{
	public:
		RandomStrategy() {}
		virtual long compact() = 0;
};
#endif
