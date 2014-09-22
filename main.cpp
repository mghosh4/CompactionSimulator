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
	cout << "GreedyStrategy Compaction Cost:" << g.compact() << endl;
}

int main(int argc, char *argv[])
{
	// Create File Sets
	FileParser *fparser = parse_file(argv[0], argv[1]);
	compare_strategy(fparser);
}
