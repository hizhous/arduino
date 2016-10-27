#ifndef _DHT22_
#define _DHT22_

#include <DHT22.h>
#include <SdFat.h>

#include "Tool.h"
#include "comm.h"

#define ArrLength 5
#define DHT22_UNKNOW_VALUE -1 // DHT22 can not read temperature and humidity
#define DHT22_READ_INTERVAL 2000// The sensor can only be read from every 1-2s
#define THRESHOLD_HUMIDITY 75

extern SdFile myFile;

void DHT22_Read();
void calcAverage(float& ,float* );
float getAverHum();

#endif
