#include <iostream>
#include <fstream>
#include "constants.h"

Constants::Constants(const char *filename)
{
	sstablename = "logs/sstablefile";
	ifstream fin(filename);
	string line;

	while (getline(fin, line))
	{
		size_t equals = line.find("=");
		string lhs = line.substr(0,equals);
		string rhs = line.substr(equals + 1);

		if (lhs.compare("oc") == 0)
		{
			OP_COUNT = atol(rhs.c_str());
		}
		else if(lhs.compare("fs") == 0)
		{
			SIZE_THRESHOLD = atol(rhs.c_str());
		}
		else if(lhs.compare("rc") == 0)
		{
			RC_COUNT = atol(rhs.c_str());
		}
		else if(lhs.compare("up") == 0)
		{
			UPDATE_PROP = atof(rhs.c_str());	
		}
		else if(lhs.compare("fi") == 0)
		{
			COMPACTION_THRESHOLD = atol(rhs.c_str());
		}
	}
}
