#ifndef SSTABLE_H
#define SSTABLE_H

#include <string>
#include "../external/hyperloglog.hpp"
using namespace std;
using namespace hll;
typedef struct
{
	string filename;
	HyperLogLog hll;
	long keyCount;
}SStable;

#endif
