#include <iostream>
#include "compaction.h"
#include "../utilities/timer.h"
#include "../strategy/numberstrategy/numberstrategy.h"
#include "../strategy/filestrategy/filestrategy.h"
using namespace std;

void Compaction::compare()
{
	Timer tm;
	/*tm.start();
	cout << "SizeTieredStrategy Compaction Cost:" << s->compact() << endl;
	tm.stop();
	cout << "SizeTieredStrategy Completion Time:" << tm.duration() << endl;*/
	tm.start();
	cout << "GreedyStrategy Compaction Cost:" << g->compact() << endl;
	tm.stop();
	cout << "GreedyStrategy Completion Time:" << tm.duration() << endl;
	/*if (gh)
	{
		tm.start();
		cout << "GreedyStrategy Hyperloglog Compaction Cost:" << gh->compact() << endl;
		tm.stop();
		cout << "GreedyStrategy Hyperloglog Completion Time:" << tm.duration() << endl;
	}
	tm.start();
	cout << "BalancedTreeSizeTieredStrategy Compaction Cost:" << bs->compact() << endl;
	tm.stop();
	cout << "BalancedTreeSizeTieredStrategy Completion Time:" << tm.duration() << endl;
	tm.start();
	cout << "BalancedTreeGreedyStrategy Compaction Cost:" << bg->compact() << endl;
	tm.stop();
	cout << "BalancedTreeGreedyStrategy Completion Time:" << tm.duration() << endl;
	tm.start();
	cout << "RandomStrategy Compaction Cost:" << r->compact() << endl;
	tm.stop();
	cout << "RandomStrategy Completion Time:" << tm.duration() << endl;*/
}

NumberCompaction::NumberCompaction(vector<vector<long> > sstables)
{
	NumberStrategyOptions nsOpts(sstables);
	s = new SizeTieredNumberStrategy(nsOpts);
	g = new GreedyNumberStrategy(nsOpts); 
	gh = NULL;
	bs = new BTSizeTieredNumberStrategy(nsOpts);
	bg = new BTGreedyNumberStrategy(nsOpts);
	r = new RandomNumberStrategy(nsOpts);
}

FileCompaction::FileCompaction(vector<SStable> sstables, long numFiles)
{
	FileStrategyOptions fsOpts(sstables, numFiles);
	s = new SizeTieredFileStrategy(fsOpts);
	g = new GreedyHLLFileStrategy(fsOpts); 
	gh = NULL; //new GreedyBFFileStrategy(fsOpts); 
	bs = new BTSizeTieredFileStrategy(fsOpts);
	bg = new BTGreedyFileStrategy(fsOpts);
	r = new RandomFileStrategy(fsOpts);
}
