#include "DS3231.h"
#include "Tool.h"
#define c_size 8

DS3231 Clock;
char c_date[c_size];
char c_time[c_size];

extern boolean OpenSerial;
extern boolean WriteToFile;

void ReadDS3231()
{
  bool Century=false;
  bool h12;
  bool PM;
  byte year, month, date, DoW, hour, minute, second,temperature;

  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  DoW = Clock.getDoW();
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  temperature=Clock.getTemperature();
  char c_now_date[c_size];
  for(int i = 0;i < c_size;++i)
  {
    c_now_date[i] = 0;
  }
  sprintf(c_now_date,"%d-%d-%d",year,month,date);
  //println(c_date);
  //println(c_now_date);
  boolean hasChangeDate = false;
  for(int i = 0;i < c_size;++i)
  {
    if(c_date[i] != c_now_date[i])
    {
      print(i);print("\t");print(c_date[i]);print("\t");println(c_now_date[i]);
      hasChangeDate = true;
      break;
    }
  }
  sprintf(c_date,"%d-%d-%d",year,month,date);
  sprintf(c_time,"%d:%d:%d",hour,minute,second);
  if(hasChangeDate)
  {
    dateHasChange();
  }
  //sprintf(c_file,"%s.txt",c_date);
}

char* DS3231_getTime()
{
  return c_time;
}

char* DS3231_getDate()
{
  return c_date;
}

