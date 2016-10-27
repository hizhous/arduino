#include <SimpleSDAudio.h>

const int MOSTTRAINCOUNT = 3;
const int HallSignedPin = 3;
const int Light = 5;
const int HitMusicsAmount = 3;
char *HitMusics[] = {
  "HIT_01.AFM","HIT_02.AFM","HIT_03.AFM"
};
char *HitMusics_500[] = {
  "01_500.AFM","02_500.AFM","03_500.AFM"
};
char *ErrorMusic = "ERROR.AFM";
char *FinishMusic = "FINISH5.AFM";
char *ShutDownMusic = "SHUTDOWN.AFM";
char *StartUpMusic = "STARTUP5.AFM";
char *ExampleMusic = "EXAMPLE.AFM";
int nowTrainCount;
bool Hall;
bool NowisOFF;
bool NowisON;

void setup()
{
  Serial.begin(9600);
  pinMode(HallSignedPin,INPUT);
  pinMode(Light,OUTPUT);
  Hall =false;
  NowisON = false;
  NowisOFF = true;
  nowTrainCount = 0;
  SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER);
  playMusic(StartUpMusic);
}

void loop(){
  Hall = digitalRead(HallSignedPin) ? false : true;
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
  ++nowTrainCount;
  playMusic(RandomMusic());
  turnOnTheLight();
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
  while(!SdPlay.isStopped());
  delay(500);
  playMusic(FinishMusic);
  Reset();
}

void playMusic1(char* music){
  if(!SdPlay.isStopped()){
    SdPlay.stop();
  }
  SdPlay.setFile(music); 
  SdPlay.play();
}

void playMusic(char* music){
  if(!SdPlay.isStopped()){
    SdPlay.stop();
  }
  SdPlay.setFile(music);
  SdPlay.play();
}

char* RandomMusic(){
  randomSeed(analogRead(HallSignedPin));
  int randNumber = random(0, HitMusicsAmount);
  return HitMusics_500[randNumber];
}

void Reset(){
  nowTrainCount = 0;
}

