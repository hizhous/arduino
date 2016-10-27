#include <SimpleSDAudio.h>

const int Light = 5; 
const int Horn = 6;  
bool Hall;
bool lastHall;
bool isON;
bool isOFF;

void setup()
{
  Serial.begin(9600);
  Serial.println("Serial");
  pinMode( 13 , OUTPUT);
  pinMode(Light,OUTPUT);  
  pinMode(Horn,OUTPUT);  
  lastHall = Hall =false;
  isON = isOFF = false;
}

void loop()
{
//  Hall = !analogRead(A0);
  Serial.println(analogRead(A0));
//  turnOnTheLight();
  if(analogRead(A0) == 0){
    Hall = true;
  }else{
    Hall = false;
  }
  if(Hall != lastHall){
    if(Hall){
      isON = true;
    }else{
      isOFF = true;
    }
    lastHall = Hall;
  }
  if(isON){
    Serial.println("isON");
    getHall();
    isON = false;
  }
  if(isOFF){
    Serial.println("isOFF");
    loseHall();
    isOFF = false;
  }
  delay(2000);
}

void turnOnTheLight(){
   for(int nowBrightness = 0;nowBrightness <= 255;nowBrightness += 1){
    digitalWrite(Light,nowBrightness);
    delay(2);
  }
}

void turnOffTheLight(){
  for(int nowBrightness = 255;nowBrightness >= 0;--nowBrightness){
    digitalWrite(Light,nowBrightness);
    delay(2);
  }
}

void playHorn(){
  digitalWrite(Horn,HIGH);
  digitalWrite(Horn,LOW);
}

void playMusic(){
  SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER);
  SdPlay.setFile("14.AFM"); 
  SdPlay.play();
}

void getHall(){
  digitalWrite( 13 , HIGH );
  playMusic();
  turnOnTheLight();
}

void loseHall(){
  digitalWrite(13,LOW);
  turnOffTheLight();
}
