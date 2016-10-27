#include <Arduino.h>
#include <EEPROM.h>
#include "HCSR04.h"
#include "comm.h"
#include "Tool.h"

#define averLength 35
#define dis_err 5
#define maxDis 0.2
#define maxDataArray 5
#define calc_des_last_time 5000//5秒钟的测试时间

char calcAverageDistanceInProcess(int* storageAver)
{
  int aver[averLength]; 
  for(char i = 0;i < averLength; ++i)
  {
    aver[i] = storageAver[i];
  }
  long sum = -dis_err;
  char count = 1;
  char start = 0;
  char aimCount = 0;
  for (char i = 0; i < averLength; ++i) {
    int _aver = sum / count;
    if(aver[i] - _aver < dis_err){
      sum += aver[i];
      ++count;
    }
    else{
      start = i;
      storageAver[i] = aver[i];
      sum = aver[i];
      count = 1;
      ++aimCount;
    }
    int lastAver = _aver;
    _aver = sum / count;
    if(lastAver == _aver){
      storageAver[i] = _aver;
    }
    else{
      for(char j = start;j <= i;++j){
        storageAver[j] = _aver;
      }
    }
  }
  println("finish calc ");
  return aimCount;
}

char getResultCalc(int* aver,int res[][2],char _aimCount)
{
  char start = 0;
  for(int i = 0;i < _aimCount;++i)
  {
    res[i][0] = aver[start];
    int j = start;
    int count = 0;
    for(; j <= averLength;++j)
    {
      if(j != averLength && aver[j] == res[i][0])
      {
        ++count;
      }
      else{
        res[i][1] = count;
        start = j;
        break;
      }
    }
  }
  char maxCount = 0;
  for(int i = 0;i < _aimCount;++i)
  {
    if(res[i][1] > res[maxCount][1]){
      maxCount = i;
    }
  }
  return maxCount;
}

int getFinalResult(int* aver,char _aimCount)
{
  int res[_aimCount][2];
  char maxCount = getResultCalc(aver,res,_aimCount);
  println("finish res ");
  int result = -1;
  int errorCount = -1;
  float errorDis = 1;
  for(char i = 0;i < _aimCount;++i){
    if(i != maxCount){
      if(res[i][1] == res[maxCount][1]){
        errorCount = i;
        errorDis = 0.0;
        break;
      }
      float bigCount = (res[i][1] > res[maxCount][1])?(res[i][1]):(res[maxCount][1]);
      float smallCount = (res[i][1] < res[maxCount][1])?(res[i][1]):(res[maxCount][1]);
      errorDis = (bigCount - smallCount) / bigCount;
      if(errorDis <= maxDis){
        errorCount = i;
        break;
      }
    }
  }
  if(errorCount == -1){
    print("max count is : ");
    print(res[maxCount][0]);
    print(" - ");
    println(res[maxCount][1]);
    result = res[maxCount][0];
  }
  else{
    print("no result,error is: ");
    print(res[maxCount][0]);
    print(" - ");
    print(res[maxCount][1]);
    print(" , ");
    print(res[errorCount][0]);
    print(" - ");
    println(res[errorCount][1]);
    result = -1;
  }
  return result;
}

int calcAverageDistance()
{
  int aver[averLength];
  for (int i = 0; i < averLength; ++i) {
    aver[i] = (long)readHCSR04();
    print("Num.");
    print(i);
    print("  ");
    println(aver[i]);
  }/*
  for (int i = 0; i < 100; ++i) {
   int j = (long)readHCSR04();
   print("Num.");
   print(i);
   print("  ");
   println(j);
   }*/
  bubble(aver,averLength);
  //return -1;
  const char _aimCount = calcAverageDistanceInProcess(aver);
  //return -1;
  int result = -1;
  if(_aimCount <= maxDataArray)
  {
    return -1;
    result = getFinalResult(aver,_aimCount);
  }
  else{
    print("no result,to more data");
    result = -1;
  }
  return result;
}

int _distance = -1;
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
        int _des = calcAverageDistance();
        if((((des >= _des)?(des):(_des)) - ((des < _des)?(des):(_des))) / ((des >= _des)?(des):(_des)) <= maxDis){
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

/* 冒泡排序 : 定义两个参数：数组首地址与数组大小 */
void bubble(int a[], char n) 
{
  println("bubbleing ... ");
  int i, j, temp;
  for (i = 0; i < n - 1; i++)
    for (j = i + 1; j < n; j++)
      /* 注意循环的上下限 */
      if (a[i] > a[j]) {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
  println("bubble finish");
}

