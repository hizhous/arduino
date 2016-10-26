//http://www.arduino.cn/forum.php?mod=viewthread&tid=6386&extra=page%3D1&page=1
#include <DHT22.h>
// Only used for sprintf
#include <stdio.h>

// Data wire is plugged into port 7 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define PIN_DHT22 5          // Pin of DHT22

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("DHT22 Library Demo");
}

void loop(void)
{ 
  DHT22_Read();
}

const int DHT22_INDEX_TEMAERATURE = 0;  // Index of temperature
const int DHT22_INDEX_HUMIDITY = 1;     // Index of humidity
const int DHT_ERROR_DEFAULT = DHT_ERROR_NONE - 1;  // DHT22 default result(unknow)
const float DHT22_UNKNOW_VALUE = -1; // DHT22 can not read temperature and humidity
const long DHT22_READ_INTERVAL = 2000;// The sensor can only be read from every 1-2s
long Last_Read_time = 0 - DHT22_READ_INTERVAL;

// Setup a DHT22 instance
DHT22 dht22(PIN_DHT22);      // Code object of DHT22
int DHT22_Read_Result = DHT_ERROR_DEFAULT;   // DHT22  read result code
float DHT22_TemperatureC = DHT22_UNKNOW_VALUE;  // Temperature
float DHT22_Humidity = DHT22_UNKNOW_VALUE;      // Humidity

float temperature[10] = {0};
float humidity[10] = {0};
int index = 0;

void DHT22_Read()
{ 
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  //delay(2000);
  long nowReadTime = millis();
  if(nowReadTime - Last_Read_time >= DHT22_READ_INTERVAL)
  {
    Last_Read_time = nowReadTime;
    switch(dht22.readData())
    {
    case DHT_ERROR_NONE:
      DHT22_Read_Result = DHT_ERROR_NONE;
      DHT22_TemperatureC = dht22.getTemperatureC();
      DHT22_Humidity = dht22.getHumidity();
      return;
    case DHT_ERROR_CHECKSUM:
      DHT22_Read_Result = DHT_ERROR_CHECKSUM;
      DHT22_TemperatureC = dht22.getTemperatureC();
      DHT22_Humidity = dht22.getHumidity();  
      return;
    case DHT_BUS_HUNG:
      DHT22_Read_Result = DHT_BUS_HUNG;
      break;
    case DHT_ERROR_NOT_PRESENT:
      DHT22_Read_Result = DHT_ERROR_NOT_PRESENT;
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      DHT22_Read_Result = DHT_ERROR_ACK_TOO_LONG;
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      DHT22_Read_Result = DHT_ERROR_SYNC_TIMEOUT;
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      DHT22_Read_Result = DHT_ERROR_DATA_TIMEOUT;
      break;
    case DHT_ERROR_TOOQUICK:
      DHT22_Read_Result = DHT_ERROR_TOOQUICK;
      break;
    default:
      DHT22_Read_Result = DHT_ERROR_DEFAULT;
      break;
    }
    DHT22_TemperatureC = DHT22_UNKNOW_VALUE;
    DHT22_Humidity = DHT22_UNKNOW_VALUE;
    
    temperature[index] = DHT22_TemperatureC;
    humidity[index] = DHT22_Humidity;
    ++index;
    float aimTemp = 0;
    float aimHumi = 0;
    if(index == 10){
      index = 0;
      int count = 0;
      for(int i = 0;i < 10;++i){
        if(temperature[i] != DHT22_UNKNOW_VALUE){
          aimTemp += temperature[i];
          ++count;
        }
      }
      if(0 != count){
        aimTemp /= count;
      }else{
        aimTemp = DHT22_UNKNOW_VALUE;
      }
      count = 0;
      for(int i = 0;i < 10;++i){
        if(humidity[i] != DHT22_UNKNOW_VALUE){
          aimHumi += humidity[i];
          ++count;
        }
      }
      if(0 != count){
        aimHumi /= count;
      }else{
        aimHumi = DHT22_UNKNOW_VALUE;
      }
    }
  }
}
