#include <SimpleSDAudio.h>

const int MOSTTRAINCOUNT = 5;
const int HallSignedPin = 3;
const int HitLight = 5;
const int StartLight = 5;
const int HitMusicsAmount = 3;
char *HitMusics[] = {
  "H1.AFM","H2.AFM","H3.AFM"
};
char *HitMusics2[] = {
  "H12.AFM","H22.AFM","H32.AFM"
};
char *HitMusics3[] = {
  "H13.AFM","H23.AFM","H33.AFM"
};
char *HitMusics5[] = {
  "H15.AFM","H25.AFM","H35.AFM"
};
char *ErrorMusic = "ER2.AFM";
char *FinishMusic = "FH2.AFM";
char *ShutDownMusic = "SD2.AFM";
char *StartUpMusic = "SU.AFM";
char *ExampleMusic = "EXAMPLE.AFM";
int nowTrainCount;
bool Hall;
bool NowisOFF;
bool NowisON;

void setup()
{
  Serial.begin(9600);
  pinMode(HallSignedPin,INPUT);
  pinMode(HitLight,OUTPUT);
  pinMode(StartLight,OUTPUT);
  Hall =false;
  NowisON = false;
  NowisOFF = true;
  nowTrainCount = 0;
  SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER);
  turnOnTheLight(StartLight);
  playMusic(StartUpMusic);
  while(!SdPlay.isStopped());
  turnOffTheLight(StartLight);
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
  turnOnTheLight(HitLight);
}

void OFF(){
  NowisOFF = true;
  NowisON = false;
  turnOffTheLight(HitLight);
}
/*
void turnOnTheLight(){
  for(int nowBrightness = 0;nowBrightness <= 255;++nowBrightness){
    analogWrite(HitLight,nowBrightness);
    delay(2);
  }
}

void turnOffTheLight(){
  for(int nowBrightness = 255;nowBrightness >= 0;--nowBrightness){
    analogWrite(HitLight,nowBrightness);
    delay(2);
  }
}
*/
void turnOnTheLight(int light){
  for(int nowBrightness = 0;nowBrightness <= 255;++nowBrightness){
    analogWrite(light,nowBrightness);
    delay(2);
  }
}

void turnOffTheLight(int light){
  for(int nowBrightness = 255;nowBrightness >= 0;--nowBrightness){
    analogWrite(light,nowBrightness);
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
  return HitMusics5[randNumber];
}

void Reset(){
  nowTrainCount = 0;
}

