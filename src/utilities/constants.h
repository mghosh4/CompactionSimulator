#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
using namespace std;

class Constants
{
	public:
		int SIZE_THRESHOLD;
		int COMPACTION_THRESHOLD;
		long int OP_COUNT;
		long int RC_COUNT;
		double UPDATE_PROP;
		string sstablename;

		Constants() {}
		Constants(const char *filename);
};

extern Constants consts;
#endif
