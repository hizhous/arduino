#include "DHT22.h"
#include "Tool.h"

long Last_Read_time = 0 - DHT22_READ_INTERVAL;
char nowIndex = DHT22_UNKNOW_VALUE;

float DHT22_TemperatureC = DHT22_UNKNOW_VALUE;  // Temperature
float DHT22_Humidity = DHT22_UNKNOW_VALUE;      // Humidity
float average_temp = DHT22_UNKNOW_VALUE;
float average_hum = DHT22_UNKNOW_VALUE;
float temps[ArrLength];
float hums[ArrLength];

DHT22 dht22(PIN_DHT22);      // Code object of DHT22

float getAverHum()
{
  return average_hum;
}

void DHT22_Read()
{
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  // delay(2000);
  long nowReadTime = millis();
  if(nowReadTime - Last_Read_time >= DHT22_READ_INTERVAL)
  {
    Last_Read_time = nowReadTime;
    switch(dht22.readData())
    {
    case DHT_ERROR_NONE:
    case DHT_ERROR_CHECKSUM:
      DHT22_TemperatureC = dht22.getTemperatureC();
      DHT22_Humidity = dht22.getHumidity();
      nowIndex = (++nowIndex) % ArrLength;
      temps[nowIndex] = DHT22_TemperatureC;
      hums[nowIndex] = DHT22_Humidity;
      calcAverage(average_temp,temps);
      calcAverage(average_hum,hums);
      writeSomething(c_DHT22_T_average,average_temp);
      writeSomething(c_DHT22_H_average,average_hum);
      break;
    default:
      DHT22_TemperatureC = DHT22_UNKNOW_VALUE;
      DHT22_Humidity = DHT22_UNKNOW_VALUE;
      break;
    }
  }
  writeSomething(c_DHT22_T,DHT22_TemperatureC);
  writeSomething(c_DHT22_H,DHT22_Humidity);
}

void calcAverage(float& average,float* avers)
{
  average = 0;
  int aimCount = 0;
  for(int i = 0; i < ArrLength;++i){
    if(avers[i] != 0)
    {
      average += avers[i];
      ++aimCount;
    }
  }
  if( 0 != aimCount )
  {
    average /= aimCount;
  }
  else{
    average = DHT22_UNKNOW_VALUE;
  }
}

