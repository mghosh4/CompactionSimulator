#include <iostream>
#include "parser/ycsbparser.h"
#include "compaction/compaction.h"
using namespace std;

Parser* parse_file(const char *type, const char *filename)
{
	Parser *f;
	if (!strcmp(type, "ycsbnumber"))
		f = new NumberParser(filename);
	else if (!strcmp(type, "ycsbfile"))
		f = new FileParser(filename);

	f->parse();
	return f;
}

void compare_strategy(Parser *f, const char *type)
{
	Compaction *comp;
	if (!strcmp(type, "ycsbnumber"))
	{
		NumberParser *np = dynamic_cast<NumberParser *>(f);
		comp = new NumberCompaction(np->getSStables());
	}
	else if (!strcmp(type, "ycsbfile"))
	{
		FileParser *fp = dynamic_cast<FileParser *>(f);
		comp = new FileCompaction(fp->getSStables(), fp->getNumFiles());
	}

	comp->compare();
}

void print_set(Parser *fp)
{
	NumberParser *yp = dynamic_cast<NumberParser *>(fp);
	if (!yp)
		return;
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
		cout << "Usage ./main <type> <filename>\n";
		cout << "Current types supported are ycsbnumber, ycsbfile\n";
	}

	// Create File Sets
	Parser *fparser = parse_file(argv[1], argv[2]);
	cout << "Parsing Completed\n";
	//print_set(fparser);
	compare_strategy(fparser, argv[1]);
}
