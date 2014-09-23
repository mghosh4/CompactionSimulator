#include <iostream>
#include <stdio.h>
#include "fileparser.h"
#include "strategy.h"
using namespace std;

FileParser* parse_file(const char *type, const char *filename)
{
	FileParser *f;
	if (!strcmp(type, "YCSB"))
		f = new YCSBParser(filename);

	f->parse();
	return f;
}

void compare_strategy(FileParser *f)
{
	SizeTieredStrategy s(f);
	GreedyStrategy g(f);
	cout << "SizeTieredStrategy Compaction Cost:" << s.compact() << endl;
	// cout << "GreedyStrategy Compaction Cost:" << g.compact() << endl;
}

void print_set(FileParser *fp)
{
	YCSBParser *yp = dynamic_cast<YCSBParser *>(fp);
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
	FileParser *fparser = parse_file(argv[1], argv[2]);
	// print_set(fparser);
	compare_strategy(fparser);
}
