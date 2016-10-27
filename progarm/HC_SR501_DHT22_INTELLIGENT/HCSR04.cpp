#include <Arduino.h>
#include <EEPROM.h>
#include "HCSR04.h"
#include "comm.h"
#include "Tool.h"

#define averLength 20
#define dis_err 5
#define maxDis 0.2
#define maxDataArray 5
#define calc_des_last_time 5000//5秒钟的测试时间

int calcAverageDistance()
{
  long startTime = millis();
  int des = -1;
  do{
    println("calc des start:");
    des = 0;
    int lastDes = -1;
    for(char i = 0;i < averLength;++i)
    {
      int tempDes = readHCSR04();
      print("[");
      print(i);
      print("]=");
      println(tempDes);
      if(-1 == lastDes)
      {
        des += tempDes;
      }
      else{
        int bigDes = (tempDes > lastDes)?(tempDes):(lastDes);
        int smallDes = (tempDes < lastDes)?(tempDes):(lastDes);
        float errorDis = (bigDes - smallDes) / (float)bigDes;
        if(errorDis > (float)maxDis){
          print("miss des! errorDis is ");
          println(errorDis);
          des = -1;
          break;
        }
        else{
          des += tempDes;
          print("aim des! sum des is ");
          println(des);
        }
      }
      lastDes = tempDes;
    }
    if(-1 != des)
    {
      des /= averLength;
      print("aim average des is : ");
      println(des);
    }
  }
  while(-1 == des && millis() - startTime < calc_des_last_time);
  return des;
}

int _distance = -1;
/*
int callCount = 0;
 int readHCSR04() {
 int des = 141;
 callCount %= 40;
 switch (callCount) {
 case 0:
 case 2:
 case 3:
 des = 5;
 break;
 case 1:
 des = 80;
 break;
 case 4:
 des = 100;
 break;
 }
 ++callCount;
 return des;
 }
 */
int readHCSR04()
{
  digitalWrite(Trig, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10); 
  digitalWrite(Trig, LOW); // 保持发出超声波信号接口低电平
  _distance = pulseIn(Echo, HIGH); // 读出脉冲时间
  _distance= _distance/58; // 将脉冲时间转化为距离（单位：厘米）
  //_distance = (_distance*(331*sqrt(1+average_temp/273)/1000))/2;
  writeSomething(c_HC_SR04,_distance);
  return _distance;
}

void check_SetupHCSR04()
{
  if(digitalRead(SwitchPin) == 1)
  {
    println("setting up HCSR04");
    clearHCSR04();
    println("adjust location...");
    while(digitalRead(SwitchPin) == 1);//等待调整位置
    println("checking distance");
    //位置确认完，开始测试距离
    int des = -1;
    char calcCount = 0;
    // 计算距离...
    do{
      ++calcCount;
      int sumDes = 0;
      des = calcAverageDistance();
      for(char i = 0;i < 3;++i)
      {  
        print("[");
        print(i);
        println("]");
        int _des = calcAverageDistance();
        int bigDes = (_des > des)?(_des):(des);
        int smallDes = (_des < des)?(_des):(des);
        float errorDis = (bigDes - smallDes) / (float)bigDes;
        if(errorDis <= (float)maxDis){
          sumDes += _des;
        }
        else{
          sumDes = -1;
          break;
        }
      }
      if(sumDes != -1)
      {
        des = sumDes / 3;
      }
      else{
        des = -1;
      }
    }
    while(-1 == des && calcCount < 3);
    print("Distance is: ");
    println(des);
    if(-1 != des)
    {
      // 测试计算完距离，记录在EEPROM中
      setupHCSR04(des);
    }
    // 重新读出数据
    read_st_HCSR04();
  }
}

void clearHCSR04()
{
  char* st_HCSR04 = STRING_ST_HCSR04;
  const int c_length = sizeof(st_HCSR04);
  for(int i = 0;i < sizeof(st_HCSR04);++i)
  {
    EEPROM.write(i,0);
  }
  for(int i = c_length;i < c_length + sizeof(distance.dis);++i)
  {
    EEPROM.write(i, 0);
  }
}

void read_st_HCSR04()
{
  char st_HCSR04_content[sizeof(STRING_ST_HCSR04)];
  char* st_HCSR04 = STRING_ST_HCSR04;
  const int c_length = sizeof(st_HCSR04);
  hasSetupHCSR04 = true;
  for(int i = 0;i < c_length;++i)
  {
    st_HCSR04_content[i] = EEPROM.read(i);
    if(st_HCSR04_content[i] != st_HCSR04[i])
    {
      hasSetupHCSR04 = false;
    }
  }
  for(int i = c_length;i < c_length + sizeof(distance.dis);++i)
  {
    distance.b_dis[i - c_length] = EEPROM.read(i);
  }
}

void setupHCSR04(int _distance)
{
  print("setup HCSR04:");println(_distance);
  char* st_HCSR04 = STRING_ST_HCSR04;
  distance.dis = _distance;
  const int c_length = sizeof(st_HCSR04);
  for(int i = 0;i < c_length;++i)
  {
    EEPROM.write(i,st_HCSR04[i]);
  }
  for(int i = c_length;i < c_length + sizeof(distance.dis);++i)
  {
    EEPROM.write(i, distance.b_dis[i - c_length]);
  }
}
