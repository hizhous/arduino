#include <DS3231.h>
#include <Wire.h>
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte year, month, date, DoW, hour, minute, second,temperature;
void setup() {
 // 启动I2C(IIC)接口
 Wire.begin();
        //以下部分是初始化时间，每次板子通电之后都会初始化成这个时间，只是测试用，以后可以删除。
/*        Clock.setSecond(50);//Set the second 
        Clock.setMinute(59);//Set the minute 设置分钟
        Clock.setHour(11);  //Set the hour 设置小时
        Clock.setDoW(5);    //Set the day of the week 设置星期几
        Clock.setDate(31);  //Set the date of the month 设置月份
        Clock.setMonth(5);  //Set the month of the year 设置一年中的月份
        Clock.setYear(13);  //Set the year (Last two digits of the year) 设置年份(在今年的最后两位数——比如2013年最后的13)
 */// Start the serial interface
 Serial.begin(9600);
}
void ReadDS3231()
{
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  DoW = Clock.getDoW();
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
  temperature=Clock.getTemperature();
  
  Serial.print("20");
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(DoW,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');
  Serial.print("Temperature=");  //这里是显示温度
  Serial.print(temperature); 
  Serial.print('\n');
}
void loop() 
{
  ReadDS3231();
  delay(1000);  //间隔1000ms(1000ms=1秒)循环一次。
}
