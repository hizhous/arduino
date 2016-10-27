#ifndef _HCSR04_
#define _HCSR04_
#include "comm.h"

int calcAverageDistance();
int readHCSR04();
void check_SetupHCSR04();
void read_st_HCSR04();
void clearHCSR04();
void setupHCSR04(int);
union Distance
{
  int dis;
  byte b_dis[sizeof(int)];
};
extern Distance distance;

extern boolean hasSetupHCSR04;

#endif

