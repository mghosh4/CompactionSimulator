#include <boost/archive/binary_oarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp>

template <typename ClassTo> 
int Save(const string fname, const ClassTo &c)
{ 
    ofstream f(fname.c_str(), ios::binary);
    if (f.fail()) return -1;
    boost::archive::binary_oarchive oa(f); 
    oa << c; 
    return 0;
} 
