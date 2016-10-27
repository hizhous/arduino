#include <SimpleSDAudio.h>

const int MOSTTRAINCOUNT = 3;
const int HallSignedPin = 3;
const int Light = 5;
char *HitMusic = "14.AFM"; 
char *FinishedMusic = "EXAMPLE.AFM";
int nowTrainCount;
bool Hall;
bool NowisOFF;
bool NowisON;

void setup()
{
  pinMode(HallSignedPin,INPUT);
  pinMode(Light,OUTPUT);
  SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER);
  Hall =false;
  NowisON = false;
  NowisOFF = true;
  nowTrainCount = 0;
}

void loop(){
  Hall = digitalRead(HallSignedPin) ? false : true;
  Serial.println(Hall);
  if(Hall){
    if(NowisOFF){
       ON();
    }
  }else{
    if(NowisON){
      OFF();
      if(nowTrainCount == MOSTTRAINCOUNT){
        TrainFinished();
      }
    }
  }
}

void ON(){
  NowisON = true;
  NowisOFF = false;
  playMusic(HitMusic);
  turnOnTheLight();
  ++nowTrainCount;
}

void OFF(){
  NowisOFF = true;
  NowisON = false;
  turnOffTheLight();
}

void turnOnTheLight(){
   for(int nowBrightness = 0;nowBrightness <= 255;++nowBrightness){
    analogWrite(Light,nowBrightness);
    delay(2);
  }
}

void turnOffTheLight(){
  for(int nowBrightness = 255;nowBrightness >= 0;--nowBrightness){
    analogWrite(Light,nowBrightness);
    delay(2);
  }
}

void TrainFinished(){
  playMusic(FinishedMusic);
  Reset();
}

void playMusic(char* music){
  if(!SdPlay.isStopped()){
    SdPlay.stop();
  }
  SdPlay.setFile(music); 
  SdPlay.play();
}

void Reset(){
  nowTrainCount = 0;
}

