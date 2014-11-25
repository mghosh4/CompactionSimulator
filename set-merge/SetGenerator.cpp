#include <set>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>
#include "SetGenerator.h"
#include "Myset.h"
#include "RNG.h"
#include <iostream>

using namespace std;

SetGenerator::SetGenerator()
{

}

bool generate_random_set(int N, Myset& MS)
{
  RNG R;
  for(int j = 0; j < N; j++)
  {
    if(R.rand_bit())
      MS.insert(j+1);
  }

  return MS.size() > 0;
}

void SetGenerator::get_sets(int N, int M, vector<Myset>& sets)
{
  RNG R;

  for(int i = 0; i < M; i++)
  {

    Myset MS;
    do{
    }while(!generate_random_set(N,MS));

    sets.push_back(MS);
  }
}

void SetGenerator::get_sets(const char * filename, vector<Myset>& sets)
{
	ifstream fin(filename);
	string line;

	while (getline(fin, line))
	{
		Myset ms;
		istringstream iss(line);
		vector<int> tokens((istream_iterator<int>(iss)), istream_iterator<int>());

		for (vector<int>::iterator it = tokens.begin(); it != tokens.end(); it++)
			ms.insert(*it);

		sets.push_back(ms);
	}
	fin.close();
}
