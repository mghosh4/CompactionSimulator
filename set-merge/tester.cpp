#include "SetGenerator.h"
#include "Myset.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "RNG.h"
#include <time.h>
#include <cstdlib>
#include <queue>

using namespace std;

map < string, pair < int, queue< string > > > mmap;

void print_set(Myset& MS)
{

  cout << MS.to_string();
  cout << endl;

}

void print_log(queue<string>& log)
{
  while(!log.empty())
  {
    string s = log.front();
    cout << s << endl;
    log.pop();
  }
}

void find_greedy_indexes(vector<Myset>& sets, int& j, int& k)
{
  j = 0; k = 1;
  int usize = sets[j].union_with(sets[k]).size();
  for(int x = 0; x < sets.size(); x++)
  {
    for(int y = x+1; y < sets.size(); y++)
    {

      int csize = sets[x].union_with(sets[y]).size();
      if(csize < usize)
      {
        j = x; k = y; usize = csize;
      }

    }
  }
}
int compute_greedy_solution(vector<Myset>& sets, queue<string>& log)
{
  int M = sets.size();
  if(M <= 1) return 0;
  if(M == 2)
  {
    Myset C = sets[0].union_with(sets[1]);
    stringstream ss;
    ss << "Merge " << sets[0].to_string() << " and " << sets[1].to_string() << " result set = "
       << C.to_string() << " cost charged = " << C.size();
    log.push(ss.str());
    return C.size();
  }

  int cost = 0;
  for(int i = 0; i < M - 1; i++)
  {
    int j, k;
    find_greedy_indexes(sets, j, k); //recall j < k
    Myset C = sets[j].union_with(sets[k]);
    stringstream ss;
    ss << "Merge " << sets[j].to_string() << " and " << sets[k].to_string() << " result set = " 
       << C.to_string() << " cost charged = " << C.size();
    log.push(ss.str());
    cost += C.size();    

    sets.erase(sets.begin() + j);
    sets.erase(sets.begin() + k - 1);

    sets.push_back(C);
  }
  return cost;
}

int compute_optimal_cost(vector<Myset>& sets, queue<string>& log)
{
  if(sets.size() <= 1)
    return 0;

  if(sets.size() == 2)
  {   
    Myset C = sets[0].union_with(sets[1]); 
    stringstream ss;
    ss << "Merge " << sets[0].to_string() << " and " << sets[1].to_string() << " result set = "
       << C.to_string() << " cost charged = " << C.size();
    log.push(ss.str());
    return C.size();
  }

  string instance_key = Myset::key(sets);

  //see if this found in the memoization map
  map < string , pair < int, queue < string > > > :: iterator it = mmap.find(instance_key);

  if(it != mmap.end())
  {
    queue < string > tlog = (it->second).second;
    while(!tlog.empty())
    {
      log.push(tlog.front());
      tlog.pop();
    }
    return (it->second).first;
  }

  //else call recursively
  int min_cost = -1;
  queue < string > min_log;

  for(int x = 0; x < sets.size(); x++)
  {


    for(int y = x + 1; y < sets.size(); y++)
    {
      vector<Myset> sets_XY;
      queue < string > clog;
      Myset C = sets[x].union_with(sets[y]);
      stringstream ss; 
      ss << "Merge " << sets[x].to_string() << " and " << sets[y].to_string() << " result set = "         << C.to_string() << " cost charged = " << C.size();
      clog.push( ss.str() );
      int cost1 = C.size();

      for(int z = 0; z < sets.size(); z++)
      {
        if(z != x && z != y)
          sets_XY.push_back(sets[z]);
      }

      sets_XY.push_back(C);

      int ccost = cost1 + compute_optimal_cost(sets_XY, clog);

      if(min_cost == -1 || ccost < min_cost)
      {
        min_cost = ccost; min_log = clog;
      }

    }

  }

  //store the solution
  mmap[instance_key] = make_pair ( min_cost, min_log );

  while(!min_log.empty())
  {
    log.push(min_log.front());
    min_log.pop();
  }

  return min_cost;
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  SetGenerator SGen;
  vector<Myset> sets;
  int N = 10; int M = 7;

  if (argc > 1)
  {
	SGen.get_sets(argv[1], sets);
  }
  else
  {
  	printf("Enter N [sets are from {1 .. N}, suggested N <= 10] : ");
  	scanf("%d",&N);

  	printf("Enter M [number of sets, suggested M <= 8]: ");
  	scanf("%d",&M);

  	cout << "Generating (random) sets ... " << endl;

  	SGen.get_sets(N,M,sets);
  }

  vector < Myset > greedy_sets = sets;
  vector < Myset > optimal_sets = sets;
  M = sets.size();
  N = sets[0].size();

  cout << "The sets are ... " << endl;
  for(int i = 0; i < M; i++)
  {
    cout << sets[i].to_string() << endl;
  }
  cout << endl << endl;


  //cout << "Running greedy ... " << endl << endl;
  queue<string> greedy_log, optimal_log;
  //int greedy_cost = compute_greedy_solution(greedy_sets,greedy_log);
 
  //cout << "The greedy cost is " << greedy_cost << endl; 
  //print_log(greedy_log);

  //cout << endl << endl << endl;

  cout << "Running opt ... " << endl << endl;
  int optimal_cost = compute_optimal_cost(optimal_sets,optimal_log);
  cout << "The optimal cost is " << optimal_cost << endl;
  print_log(optimal_log);

  return 0;
}
