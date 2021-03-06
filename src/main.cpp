#include <iostream>
#include "parser/ycsbparser.h"
#include "compaction/compaction.h"
#include "utilities/constants.h"
using namespace std;

Constants consts;

Parser* parse_file(const char *type, const char *filename, long proj_elem_count)
{
	Parser *f;
	if (!strcmp(type, "ycsbnumber"))
		f = new NumberParser(filename);
	else if (!strcmp(type, "ycsbfile"))
		f = new FileParser(filename, proj_elem_count);

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
		cout << "Usage ./main <type> <filename> <updatepercentage>\n";
		cout << "Current types supported are ycsbnumber, ycsbfile\n";
	}

	Constants c(argv[3]);
	consts = c;
	// Create File Sets
	long proj_elem_count = (long) ceil(consts.RC_COUNT + consts.OP_COUNT * consts.UPDATE_PROP / 100.0);
	Parser *fparser = parse_file(argv[1], argv[2], proj_elem_count);
	//cout << "Parsing Completed\n";
	print_set(fparser);
	compare_strategy(fparser, argv[1]);
}
