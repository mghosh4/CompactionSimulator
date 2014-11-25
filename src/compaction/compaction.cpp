#include <iostream>
#include "compaction.h"
#include "../utilities/timer.h"
#include "../strategy/numberstrategy.h"
#include "../strategy/filestrategy.h"
using namespace std;

void Compaction::compare()
{
	Timer tm;
	/*tm.start();
	cout << "SizeTieredStrategy Compaction Cost:" << s->compact() << endl;
	tm.stop();
	cout << "Time taken to complete Size Tiered:" << tm.duration() << endl;
	tm.start();
	cout << "GreedyStrategy Compaction Cost:" << g->compact() << endl;
	tm.stop();
	cout << "Time taken to complete Greedy:" << tm.duration() << endl;*/
	tm.start();
	cout << "BalancedTreeSizeTieredStrategy Compaction Cost:" << bs->compact() << endl;
	tm.stop();
	cout << "Time taken to complete Balanced Tree:" << tm.duration() << endl;
	/*tm.start();
	cout << "BalancedTreeGreedyStrategy Compaction Cost:" << bg->compact() << endl;
	tm.stop();
	cout << "Time taken to complete Balanced Tree:" << tm.duration() << endl;*/
}

NumberCompaction::NumberCompaction(vector<vector<long> > sstables)
{
	NumberStrategyOptions nsOpts(sstables);
	s = new SizeTieredNumberStrategy(nsOpts);
	g = new GreedyNumberStrategy(nsOpts); 
	bs = new BTSizeTieredNumberStrategy(nsOpts);
	bg = new BTGreedyNumberStrategy(nsOpts);
}

FileCompaction::FileCompaction(vector<SStable> sstables, long numFiles)
{
	FileStrategyOptions fsOpts(sstables, numFiles);
	s = new SizeTieredFileStrategy(fsOpts);
	g = new GreedyFileStrategy(fsOpts); 
}
