#include <Ultrasonic.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13
#define BUF_PIN      7

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup()
{
  Serial.begin(9600);
  pinMode(BUF_PIN, OUTPUT);
}

void loop()
{
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();

  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  if(cmMsec < 20) {
    for(int i = 0; i < 80; i++) {
      digitalWrite(BUF_PIN, HIGH);
      delay(1);
      digitalWrite(BUF_PIN, LOW);
      delay(1);
    }
  }  
  Serial.print("MS: ");
  Serial.print(microsec);
  Serial.print(", CM: ");
  Serial.print(cmMsec);
  Serial.print(", IN: ");
  Serial.println(inMsec);
  delay(1000);
}


