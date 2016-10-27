const int Light = 5; 
const int Horn = 6;  
bool Hall;
bool lastHall;
bool isON;
bool isOFF;

void setup()
{
  Serial.begin(9600);
  pinMode( 13 , OUTPUT);
  pinMode(Light,OUTPUT);  
  pinMode(Horn,OUTPUT);  
  lastHall = Hall =false;
  isON = isOFF = false;
}

void loop()
{
  Hall = !analogRead(A0);
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
}

void turnOnTheLight(){
   for(int nowBrightness = 0;nowBrightness <= 255;nowBrightness += 5){
    digitalWrite(Light,nowBrightness);
    delay(2);
  }
}

void turnOffTheLight(){
  for(int nowBrightness = 255;nowBrightness >= 0;--nowBrightness){
    digitalWrite(Light,nowBrightness);
    delay(1);
  }
}

void playMusic(){
  digitalWrite(Horn,HIGH);
  digitalWrite(Horn,LOW);
}
void getHall(){
  digitalWrite( 13 , HIGH );
  turnOnTheLight();
  playMusic();
}

void loseHall(){
  digitalWrite(13,LOW);
  turnOffTheLight();
}
