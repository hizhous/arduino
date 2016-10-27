#ifndef _TOOL_
#define _TOOL_

#include <Arduino.h>
#include <SdFat.h>

extern boolean OpenSerial;
extern SdFile myFile;
extern boolean fileIsOpen;
extern boolean WriteToFile;

void print(char*);
void println(char*);
void println();

void print(int);
void println(int);
void print(long);
void println(long);
void print(float);
void println(float);
void print(double);
void println(double);
void print(boolean);
void println(double);
void reset();
void openFan();
void closeFan();

void writeTime(char*);
void check_IfWriteToFile();
void check_IfWriteToFile(boolean);
boolean openFile();
void closeFile();
void dateHasChange();
char* getNowTime();
char* getNowDate();
void writeSomething(char* key,char* value);
void writeSomething(char* key,float value);
void writeSomething(char* key,int value);
#endif
