#ifndef __SETGENERATOR_H__
#define __SETGENERATOR_H__
#include <vector>
#include "Myset.h"

using namespace std;
class SetGenerator {
  public:

    SetGenerator();

    void get_sets(int N, int M, vector<Myset>& sets);

	void get_sets(const char * filename, vector<Myset>& sets);
};

#endif
