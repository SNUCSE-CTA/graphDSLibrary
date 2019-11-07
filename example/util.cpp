#include "util.h"

Timer::Timer()
{
}

void Timer::start()
{
	start_clock = clock();
}

double Timer::end()
{
	end_clock = clock();
	return ( (end_clock - start_clock) /(double)CLOCKS_PER_SEC );
}
