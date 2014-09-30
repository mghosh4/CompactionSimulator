#include <iostream>
#include <stdio.h>
#include "fileparser.h"
#include "strategy.h"
#include "timer.h"
using namespace std;

YCSBParser* parse_file(const char *type, const char *filename)
{
	YCSBParser *f;
	if (!strcmp(type, "YCSB"))
		f = new NumberParser(filename);

	f->parse();
	return f;
}

void compare_strategy(YCSBParser *f)
{
	SizeTieredStrategy s(f);
	GreedyStrategy g(f);
	Timer tm;
	tm.start();
	cout << "SizeTieredStrategy Compaction Cost:" << s.compact() << endl;
	tm.stop();
	cout << "Time taken to complete Size Tiered:" << tm.duration() << endl;
	tm.start();
	cout << "GreedyStrategy Compaction Cost:" << g.compact() << endl;
	tm.stop();
	cout << "Time taken to complete Greedy:" << tm.duration() << endl;
}

void print_set(YCSBParser *fp)
{
	NumberParser *yp = dynamic_cast<NumberParser *>(fp);
	vector< vector<long> > sets = yp->getSStables();
	for (vector< vector<long> >::iterator it = sets.begin(); it != sets.end(); it++)
	{
		vector<long> set = *it;
		for (vector<long>::iterator bit = set.begin(); bit != set.end(); bit++)
		{
			cout << *bit << " ";
		}
		cout << "\n";
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Usage ./main YCSB <filename>\n";
		cout << "Current type supported is YCSB\n";
	}

	// Create File Sets
	YCSBParser *fparser = parse_file(argv[1], argv[2]);
	//print_set(fparser);
	compare_strategy(fparser);
}
