#ifndef _DS3231_
#define _DS3231_

#include <DS3231.h>
#include <Wire.h>
#include "comm.h"

void ReadDS3231();
char* DS3231_getTime();
char* DS3231_getDate();

#endif
