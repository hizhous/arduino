#include <SimpleSDAudio.h>

const int MOSTTRAINCOUNT = 3;
const int HallSignedPin = 3;
const int Light = 5;
const int HitMusicsAmount = 3;
char *HitMusics[] = {
  "HIT_01.AFM","HIT_02.AFM","HIT_03.AFM"
};
char *ErrorMusic = "ERROR.AFM";
char *FinishMusic = "FINISH.AFM";
char *ShutDownMusic = "SHUTDOWN.AFM";
char *StartUpMusic = "STARTUP.AFM";
char *ExampleMusic = "EXAMPLE.AFM";
int nowTrainCount;
bool Hall;
bool NowisOFF;
bool NowisON;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting up machine...");
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
  Serial.println("--------------------\n[ON]:");
  NowisON = true;
  NowisOFF = false;
  Serial.print("Hit count:\t");
  Serial.println(++nowTrainCount);
  playMusic(RandomMusic());
  turnOnTheLight();
}

void OFF(){
  Serial.println("[OFF]:");
  NowisOFF = true;
  NowisON = false;
  turnOffTheLight();
}

void turnOnTheLight(){
  Serial.print("Turning on the light...\t");
   for(int nowBrightness = 0;nowBrightness <= 255;++nowBrightness){
    analogWrite(Light,nowBrightness);
    delay(2);
  }
  Serial.println("\tLight on!");
}

void turnOffTheLight(){
  Serial.print("Turning off the light...");
  for(int nowBrightness = 255;nowBrightness >= 0;--nowBrightness){
    analogWrite(Light,nowBrightness);
    delay(2);
  }
  Serial.println("\tLight off!");
}

void TrainFinished(){
  Serial.println("Train finished!");
  while(!SdPlay.isStopped());
  delay(500);
  playMusic(FinishMusic);
  Reset();
  Serial.println("--------****--------");
}

void playMusic(char* music){
  Serial.print("Playing Music:\t");
  Serial.println(music);
  if(!SdPlay.isStopped()){
    SdPlay.stop();
  }
  if(!SdPlay.setFile(music)) {
    Serial.print("Not found on card! Error code:\t");
    Serial.println(SdPlay.getLastError());
    playMusic(ErrorMusic);
    return;
  }
  SdPlay.play();
}

char* RandomMusic(){
  randomSeed(analogRead(HallSignedPin));
  int randNumber = random(0, HitMusicsAmount);
  return HitMusics[randNumber];
}

void Reset(){
  nowTrainCount = 0;
}

