#ifndef __MYSET_H__
#define __MYSET_H__
#include <map>
#include <string>
#include <vector>

using namespace std;

class Myset {

  private:
    map <int,bool> setmap;
    int sz;
  public:
    Myset();
    void insert(int x);
    string to_string();
    int size();
    Myset union_with(Myset& S);
    static string key(Myset& S);
    static string key(vector<Myset>& sets);
};
#endif
