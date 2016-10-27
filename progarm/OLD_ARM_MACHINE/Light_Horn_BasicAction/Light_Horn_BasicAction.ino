const int Light = 5; // Define light as pin 5
const int Horn = 6;  // Define horn as pin 6
bool Hall;  // This variable shows if get Hall signal,true for get,false for not

void setup()
{
  pinMode( 13 , OUTPUT);
  pinMode(Light,OUTPUT);  //  Make light as output signal
  pinMode(Horn,OUTPUT);   //  Make horn as output signal
  Hall = false;
}

void loop()
{
  Hall = !analogRead(A7);
  if(Hall){
    //if get Hall signal,do something in getHall()
    getHall();
  }else{
    //if lose Hall signal,do another things in noHall();
    noHall();
  }
}

//this function define something that when get the Hall signal 
void getHall(){
  digitalWrite( 13 , HIGH );
  analogWrite(Light,255);
  analogWrite(Horn,HIGH);
}

//this function define something that when no Hall signal
void noHall(){
  digitalWrite(13,LOW);
  analogWrite(Light,LOW);
  analogWrite(Horn,LOW);
}
