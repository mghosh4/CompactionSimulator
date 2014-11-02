#include <stdio.h>
#include "murmur3.h"

void MurmurHash3Wrpr( const void * key, int len, uint32_t seed, void * out )
{
	MurmurHash3_x86_32(key, len, seed, out);
}

