#include<vector>
using namespace std;
class Strategy
{
	public:
		Strategy(){}
		virtual long compact() = 0;
};

class GreedyStrategy : public Strategy
{
	public:
		GreedyStrategy(vector< vector<long> > sstables) : mSSTables(sstables) {}
		virtual long compact();
	private:
		vector< vector<long> > mSSTables;
};

class SizeTieredStrategy : public Strategy
{
	public:
		SizeTieredStrategy(vector< vector<long> > sstables) : mSSTables(sstables) {}
		virtual long compact();
	private:
		vector< vector<long> > mSSTables;
};
