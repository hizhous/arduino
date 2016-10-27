void setup()
{
  Serial.begin(9600);
  Serial.println("Start: ");
  Serial.println();
}

void loop()
{
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  
  Serial.print("KEY_H:");
  Serial.println(digitalRead(7));
  
  Serial.print("KEY_L:");
  Serial.println(digitalRead(8));

}
