#include "RNG.h"
#include "stdlib.h"


RNG::RNG()
{
}

bool RNG::rand_bit()
{
  if(rand() % 2 == 1)
    return true;
  else
    return false;
}

