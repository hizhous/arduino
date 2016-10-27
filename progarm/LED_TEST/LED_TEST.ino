#define LED 11 // 11号引脚输出
//#define LEDREAD 13 //13号引脚读取11号引脚输出值

void setup()
{
  Serial.begin(9600);
  Serial.println("Start: ");
  Serial.println();
  pinMode(LED, OUTPUT);
  //pinMode(LEDREAD, INPUT);
}

void loop()
{
  Serial.println("----------------------");

  int i = 0;
  for(i = 0;i <= 255;i += 5)
  {  
    Serial.print("light:\t");
    analogWrite(LED,i);
    Serial.print("i = ");
    Serial.println(i);
    //Serial.println(analogRead(LEDREAD));
  }
  for(i = 255;i >= 0; i -= 5)
  {  
    Serial.print("down:\t");
    analogWrite(LED,i);
    Serial.print("i = ");
    Serial.println(i);
    //Serial.println(analogRead(LEDREAD));
  }

  //delay(1000);
}

