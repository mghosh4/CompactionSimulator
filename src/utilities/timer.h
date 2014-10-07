#include <sys/time.h>

class Timer
{
    timeval timer[2];

  public:

    timeval start();

    timeval stop();
    
	int duration() const;
};
