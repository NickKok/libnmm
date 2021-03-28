#include "Optimization.hh"

#define __OPTIMIZATION_HH__

#ifdef OPTIMIZATION
#include <iostream>
optimization::Webots &fake = optimization::Webots::Instance();
optimizer tryGetOpti(){
	return fake;
}
#else 
//#error this is fake
FakeOpti FakeOpti::fake;
optimizer tryGetOpti(){
	return FakeOpti::Instance();
}
#endif


/* Return 1 if the difference is negative, otherwise 0.  */
int Timer::timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void Timer::timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}
void Timer::Start(){
    // begin
    gettimeofday(&tvBegin, NULL);
    //timeval_print(&tvBegin);
}
void Timer::End(){
    //end
    gettimeofday(&tvEnd, NULL);
    //timeval_print(&tvEnd);
}

void Timer::Diff(){
    // diff
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
}
