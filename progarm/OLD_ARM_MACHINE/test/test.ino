#include <windows.h>
const int Light = 5; // Define light as pin 5
const int Horn = 6;  // Define horn as pin 6
bool Hall;  // This variable shows if get Hall signal,true for get,false for not
int nowBrightness;

void setup()
{
  Serial.begin(9600);
  pinMode( 13 , OUTPUT);
  pinMode(Light,OUTPUT);  //  Make light as output signal
  pinMode(Horn,OUTPUT);   //  Make horn as output signal
  Hall =false;
  nowBrightness = 0;
  pinMode(3,INPUT);
}

void loop(){
  Serial.println(digitalRead(3));
  delay(500);
}
