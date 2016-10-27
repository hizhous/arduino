#include <Arduino.h>
#include "Tool.h"
#include "DS3231.h"
#include "comm.h"

extern boolean isFanOn;
boolean fileIsOpen = false;
void(* resetFunc) (void) = 0; //制造重启命令

void reset()
{
  println("reset..");
  delay(2000);
  resetFunc();
}

void openFan()
{
  writeSomething(c_FAN,"open");
  if(!isFanOn)
  {
    println("open fan");
    digitalWrite(PIN_FAN,HIGH);
    isFanOn = true;
  }
}

void closeFan()
{
  writeSomething(c_FAN,"close");
  if(isFanOn)
  {
    println("close fan");
    digitalWrite(PIN_FAN,LOW);
    isFanOn = false;
  }
}

void writeTime(char* time)
{
  if(fileIsOpen && WriteToFile){
    myFile.print(c_TIME);
    myFile.print(":[");
    myFile.print(getNowTime());
    myFile.print("]-");
  }
  print(c_TIME);
  print(":[");
  print(getNowTime());
  print("]-");
}
void check_IfWriteToFile(boolean openfile)
{
  if(digitalRead(Pin_WriteToFile) == 1)
  {
    WriteToFile = true;
    if(openfile && !fileIsOpen)
    {
      openFile();
    }
    println("set Write to file true");
  }
  else{
    WriteToFile = false;
    if(fileIsOpen)
    {
      closeFile();
    }
    println("set Write to file false");
  }
}
void check_IfWriteToFile()
{
  check_IfWriteToFile(true);
}

void dateHasChange()
{
  println("date has change");
  if(WriteToFile && fileIsOpen)
  {
    closeFile();
    openFile();
  }
}

boolean openFile()
{
  if(WriteToFile && !fileIsOpen){
    println("open file");
    char c_file[20];
    sprintf(c_file,"%s.txt",getNowDate());
    println(c_file);
    if (!myFile.open(c_file, O_RDWR | O_CREAT | O_AT_END)) {
      println(c_file);
      println("opening data.txt for write failed");
      fileIsOpen = false;
      //写入日志失败，自动重启
      reset();
    }
    else{
      fileIsOpen = true;
    }
  }
  return fileIsOpen;
}

void closeFile()
{
  if(fileIsOpen)
  {
    println("close file");
    myFile.close();
    fileIsOpen = false;
  }
}

void writeSomething(char* key,char* value)
{
  writeTime(getNowTime());
  if(fileIsOpen && WriteToFile)
  {
    myFile.print(key);
    myFile.print(":[");
    myFile.print(value);
    myFile.print("]");
    myFile.println();
  }
  print(key);
  print(":[");
  print(value);
  print("]");
  println();
}

void writeSomething(char* key,float value)
{
  writeTime(getNowTime());
  if(fileIsOpen && WriteToFile)
  {
    myFile.print(key);
    myFile.print(":[");
    myFile.print(value);
    myFile.print("]");
    myFile.println();
  }
  print(key);
  print(":[");
  print(value);
  print("]");
  println();
}

void writeSomething(char* key,int value)
{
  writeTime(getNowTime());
  if(fileIsOpen && WriteToFile)
  {
    myFile.print(key);
    myFile.print(":[");
    myFile.print(value);
    myFile.print("]");
    myFile.println();
  }
  print(key);
  print(":[");
  print(value);
  print("]");
  println();
}

char* getNowTime()
{
  return DS3231_getTime();
}

char* getNowDate()
{
  return DS3231_getDate();
}

void print(char* c)
{
  if(OpenSerial)
  {
    Serial.print(c);
  }
}

void println(char* c)
{
  if(OpenSerial)
  {
    Serial.println(c);
  }
}

void println()
{
  println("");
}

void print(long l)
{
  if(OpenSerial)
  {
    Serial.print(l);
  }
}

void println(long l)
{
  print(l);
  println();
}

void print(float f)
{
  if(OpenSerial)
  {
    Serial.print(f);
  }
}

void println(float f)
{
  print(f);
  println();
}

void print(double d)
{
  if(OpenSerial)
  {
    Serial.print(d);
  }
}

void println(double d)
{
  print(d);
  println();
}

void print(int i)
{
  if(OpenSerial)
  {
    Serial.print(i);
  }
}

void println(int i)
{
  print(i);
  println();
}

void print(boolean b)
{
  if(OpenSerial)
  {
    Serial.print(b);
  }
}

void println(boolean b)
{
  print(b);
  println();
}


