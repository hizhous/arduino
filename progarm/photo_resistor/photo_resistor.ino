//http://www.geek-workshop.com/thread-1340-1-1.html
int a =220;
#define pin 13
void setup ()
{
  Serial.begin(9600);
  pinMode(pin,OUTPUT);
}
void loop()
{
  int n = analogRead(A0);
  //Serial.println(n);
  if (n < a )
  {
    digitalWrite(pin,1);
  }
  else
  {
    digitalWrite(pin,0);
  }
}


