#include <SimpleSDAudio.h>

const int chipSelect = 4;
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

/* For SDCard Info*/
#include <SD.h>
Sd2Card card;
SdVolume volume;
SdFile root;

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
  CardInfo();
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
  Serial.println("--------------------\nON:");
  NowisON = true;
  NowisOFF = false;
  Serial.print("Hit count:\t");
  Serial.println(++nowTrainCount);
  playMusic(RandomMusic());
  turnOnTheLight();
}

void OFF(){
  Serial.println("OFF:");
  NowisOFF = true;
  NowisON = false;
  turnOffTheLight();
}

void turnOnTheLight(){
  Serial.print("Turning on the light...");
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
}

void playMusic1(char* music){
  if(!SdPlay.isStopped()){
    SdPlay.stop();
  }
  SdPlay.setFile(music); 
  SdPlay.play();
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
//    playMusic(ErrorMusic);
//    return;
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

/* For SDCard Info*/
void CardInfo(){
  Serial.print("\nInitializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("nitialization failed. Things to check:");
    Serial.println("* Is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}
