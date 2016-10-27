/*
   HC-SR501:
 Working voltage: DC 4.5-20V
 Trigger mode: L = Never repeat trigger; H = Repeat tirgger
 */
/*
  DHT22:
 Working voltage: 3.3-6V
 The sensor can only be read from every 1-2s, and requires a minimum
 */
#include <EEPROM.h>
#include <DHT22.h>
#include <DS3231.h>
#include <Wire.h>
#include <SdFat.h>
#include "Tool.h"
#include "DHT22.h"
#include "DS3231.h"
#include "HCSR04.h"
#include "HCSR501.h"
#include "comm.h"

SdFat sd;
SdFile myFile;
Distance distance;
boolean OpenSerial = true;
boolean WriteToFile = false;
boolean hasSetupHCSR04 = false;

void setup()
{
  Serial.begin(9600);
  println("Application start:");
  read_st_HCSR04();
  if(hasSetupHCSR04)
  {
    print("HC_SR04 has setup! Distance is: ");
    println(distance.dis);
  }
  else{
    println("HC_SR04 has not setup!");
  }
  Wire.begin();
  pinMode(PIN_HC_SR501,INPUT); //设置人体红外接口为输入状态
  pinMode(PIN_HC_SR501_PRINT,OUTPUT); //设置人体红外输出为输出状态
  pinMode(Pin_TestInput,INPUT); 
  pinMode(Pin_TestOutput,OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(PIN_FAN,OUTPUT);
  //pinMode(Pin_HC_SR04,OUTPUT);
  pinMode(SwitchPin,INPUT);
  pinMode(Pin_WriteToFile,INPUT);
  check_IfWriteToFile(false);
  if(WriteToFile)
  {
    if (!sd.begin(chipSelect, SPI_HALF_SPEED)){
      sd.initErrorHalt();
    }
    else{
      println("init file...");
    }
  }
}
boolean isFanOn = false;
boolean hasAimHumidity = false;
long inLoopTime = DEFAULT_TIME;
long Fan_AbleStartTime = DEFAULT_TIME;
long Fan_AbleCloseTime = DEFAULT_TIME;
long CutLineStartTime = DEFAULT_TIME;
long CutLineCloseTime = DEFAULT_TIME;
boolean isWetOpenFan = false;
boolean isCutLineOpenFan = false;
void loop()
{
  ReadDS3231();// 读取时间
  check_SetupHCSR04();// 检查是否设置HC_SR04
  check_IfWriteToFile();// 检查是否写入日志
  if(millis() - inLoopTime > LoopTimeDelay)//延时100毫秒
  {
    inLoopTime = millis();
    long time = millis();
    HCSR501_Read();

    if(hasSetupHCSR04)
    {
      int dis = readHCSR04();
      isSomethingCutLine(dis);
      if(DEFAULT_TIME != CutLineStartTime && millis() - CutLineStartTime > TIGGER_OPEN_FAN_TIME)
      {
        isCutLineOpenFan = true;
      }
      if(DEFAULT_TIME != CutLineCloseTime && millis() - CutLineCloseTime > TIGGER_CLOSE_FAN_TIME)
      {
        isCutLineOpenFan = false;
      }
    }
    DHT22_Read();
    isTooWetNeedOpenFan();
    if(DEFAULT_TIME != Fan_AbleStartTime && millis() - Fan_AbleStartTime > TIGGER_OPEN_FAN_TIME)
    {
      isWetOpenFan = true;
    }
    if(DEFAULT_TIME != Fan_AbleCloseTime && millis() - Fan_AbleCloseTime > TIGGER_CLOSE_FAN_TIME)
    {
      isWetOpenFan = false;
    }
    if(isCutLineOpenFan || isWetOpenFan)
    {
      openFan();
    }
    else{
      closeFan();
    }
    machineIsNormal();
  }
}


void isSomethingCutLine(int dis)
{
  if(dis > 0 && distance.dis - dis > TIGGER_OPEN_FAN_DISTANCE)
  {
    if(DEFAULT_TIME == CutLineStartTime)
    {
      print("something cut line.");
      println(getNowTime());
      CutLineStartTime = millis();
    }
    CutLineCloseTime = DEFAULT_TIME;
  }else{
    if(DEFAULT_TIME == CutLineCloseTime)
    {
      print("something leave line.");
      println(getNowTime());
      CutLineCloseTime = millis();
    }
    CutLineStartTime = DEFAULT_TIME;
  }
}

void isTooWetNeedOpenFan()
{
  boolean peopleCome = isPeopleCome();
  if(!peopleCome)
  {
    if(DEFAULT_TIME == Fan_AbleCloseTime)
    {
      Fan_AbleCloseTime = millis();
    }
    Fan_AbleStartTime = DEFAULT_TIME;
  }
  else{
    float aver_hum = getAverHum();
    if(DHT22_UNKNOW_VALUE != aver_hum)
    {
      if(aver_hum > THRESHOLD_HUMIDITY)
      {
        print("people come and aim humidity.");
        println(getNowTime());
        hasAimHumidity = true;
        Fan_AbleCloseTime = DEFAULT_TIME;
        if(DEFAULT_TIME == Fan_AbleStartTime)
        {
          Fan_AbleStartTime = millis();
        }
      }
      else{
        hasAimHumidity = false;
        if(DEFAULT_TIME == Fan_AbleCloseTime)
        {
          print("people come but miss humidity.");
          Fan_AbleCloseTime = millis();
        }
        Fan_AbleStartTime = DEFAULT_TIME;
      }
    }
  }
}

void machineIsNormal()
{
  int testInput = digitalRead(Pin_TestInput);
  digitalWrite(Pin_TestOutput,testInput);
}

