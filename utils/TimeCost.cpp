#include "TimeCost.h"

TimeCost::TimeCost()
{
#if __cplusplus >= 201103l
bTime = Clock::now();
#elif _POSIX_C_SOURCE >= 199903l
clock_gettime( CLOCK_MONOTONIC, &bTime);
#else
bTime = clock();
#endif


}
TimeCost::~TimeCost()
{
#if __cplusplus >= 201103l
using namespace std::chrono;
printf("it elapsed %.8lf\n", duration_cast<Second>( Clock::now() - bTime));
#elif _POSIX_C_SOURCE >= 199903l
struct timespec eTime;
clock_gettime( CLOCK_MONOTONIC, &eTime);

long secs = (long) (eTime.tv_sec - bTime.tv_sec);
long nsec = 0;
if (eTime.tc_nsec < bTime.tv_nsec) {
    nsec += 1000000000l;
}
nsec += (eTime.tv_nsec - bTime.tv_nsec);

double seconds = secs + (double) nsec / 1000000000;

printf("it elapsed %.8lf\n", seconds);

#else
printf("it elapsed %.6f\n", clock() - bTime / CLOCKS_PER_SEC);
#endif
}


