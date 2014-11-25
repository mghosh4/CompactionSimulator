#include "Myset.h"
#include <string>
#include <sstream>


string Myset::to_string()
{
  stringstream ss;
  ss << "{ ";

  map < int, bool > :: iterator it;
  for(it = setmap.begin(); it != setmap.end();)
  {
    ss << it->first;
    if(++it != setmap.end())
      ss << ", ";
  }
  ss << " }";
  return ss.str();
}

Myset::Myset()
{
  sz = 0;
}

void Myset::insert(int x)
{
  map <int,bool> :: iterator it = setmap.find(x);

  if(it == setmap.end())
  {
    setmap[x] = true;
    sz++;
  }
}

int Myset::size()
{
  return sz;
}

Myset Myset::union_with(Myset& S)
{
  Myset A = S;
  map < int, bool> :: iterator it;

  for(it = setmap.begin(); it != setmap.end(); it++)
    A.insert(it->first);

  return A;
}

string Myset::key(Myset& S)
{
  stringstream ss;
  map<int,bool> :: iterator it;
  //recall map iterator sorts keys -- hence key is unique for each set
  for(it = S.setmap.begin(); it != S.setmap.end(); )
  {
    ss << it->first;
    if(++it != S.setmap.end())
      ss << ":";
  }
  return ss.str();
}

string Myset::key(vector<Myset>& sets)
{
  vector<string> keys;
  for(int i = 0; i < sets.size(); i++)
    keys.push_back(Myset::key(sets[i]));

  sort(keys.begin(), keys.end());

  stringstream ss;
  for(int i = 0; i < keys.size(); i++ )
  {
    ss << keys[i];
    if(i < keys.size() - 1)
      ss << "$";
  }

  return ss.str();
}
