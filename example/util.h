#ifndef TIMER_H
#define TIMER_H
#include <time.h>

class Timer
{
	long long start_clock;
	long long end_clock;
public:
	Timer();
	void start();
	double end();
};
#endif
