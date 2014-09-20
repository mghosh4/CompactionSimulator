#include <iostream>
#include <stdio.h>
#include "fileparser.h"
#include "strategy.h"
using namespace std;

vector<vector<long> > parse_file(const char *type, const char *filename)
{
	FileParser *f;
	if (!strcmp(type, "YCSB"))
		f = new YCSBParser(filename);

	return f->parse();
}

void compare_strategy(vector<vector<long> > sstables)
{
	SizeTieredStrategy s(sstables);
	GreedyStrategy g(sstables);
	cout << "SizeTieredStrategy Compaction Cost:" << s.compact() << endl;
	cout << "GreedyStrategy Compaction Cost:" << g.compact() << endl;
}

int main(int argc, char *argv[])
{
	// Create File Sets
	vector<vector<long> > sstables = parse_file(argv[0], argv[1]);
	compare_strategy(sstables);
}
