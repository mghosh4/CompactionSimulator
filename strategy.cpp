#include<iostream>
#include"strategy.h"
#include"constants.h"
#include"kwaymerge.h"

long GreedyStrategy::compact()
{
	YCSBParser *ycsbParser = dynamic_cast<YCSBParser *>(mFileParser);
	vector< vector<long> > sstables;
	if (ycsbParser)
		sstables = ycsbParser->getSStables();
	return 0;
}

long SizeTieredStrategy::compact()
{
	
	return 0;
}

