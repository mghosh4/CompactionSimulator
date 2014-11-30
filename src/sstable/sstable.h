#ifndef SSTABLE_H
#define SSTABLE_H

#include <iostream>
#include <string>
#include "../external/hyperloglog.hpp"
#include "../external/bloom_filter.hpp"
using namespace std;
using namespace hll;
class SStable
{
	public:
		SStable(): keyCount(0), hll(), bf(bloom_parameters()) {}

		SStable(long elementCount): keyCount(0), hll(), proj_key_count(elementCount)
		{
			bloom_parameters p;
			//cout << "Element Count:" << elementCount << endl;
			p.projected_element_count = elementCount;
			p.false_positive_probability = 1.0 / elementCount;
			p.random_seed = random_seed++;

			p.compute_optimal_parameters();

			bf = bloom_filter(p);
		}

		SStable(const SStable &s)
		{
			filename = s.filename;
			keyCount = s.keyCount;
			proj_key_count = s.proj_key_count;
			bf = s.bf;
			hll = s.hll;
			//hll.swap(*(const_cast< HyperLogLog * >(&s.hll)));
		}

		static long long int random_seed;
		string filename;
		HyperLogLog hll;
		bloom_filter bf;
		long keyCount;
		long proj_key_count;
};

#endif
