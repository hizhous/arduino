// Trig接Arduino板的Digital 1口，触发测距；Echo接Digital 2口，接收距离信号。
// HC-SR04 必须接5V VCC
int Trig=2; // 定义超声波信号发出接口
int Echo=3; // 定义超声波信号接收接口

int LED_1M=8;
int LED_2M=9;
int LED_3M=10;
int LED_4M=11;

int M_1=25;
int M_2=50;
int M_3=75;
int M_4=100;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_1M,OUTPUT);
  pinMode(LED_2M,OUTPUT);
  pinMode(LED_3M,OUTPUT);
  pinMode(LED_4M,OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}
void loop()
{
  Serial.println("Init.."); 
  digitalWrite(Trig, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  Serial.println("Send Trig.."); 
  digitalWrite(Trig, LOW); // 保持发出超声波信号接口低电平
  Serial.print("Get Des:\t");
  int distance = pulseIn(Echo, HIGH); // 读出脉冲时间
  distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
  Serial.println(distance); //输出距离值 
  Serial.println(" CM"); 
  if (distance <=M_4)
  {
    digitalWrite(LED_4M,HIGH);
    Serial.println("LED_4M ON"); 
  }
  else
  {
    digitalWrite(LED_4M,LOW);
    Serial.println("LED_4M OFF"); 
  }
  if (distance <=M_3)
  {
    digitalWrite(LED_3M,HIGH);
    Serial.println("LED_3M ON"); 
  }
  else
  {
    digitalWrite(LED_3M,LOW);
    Serial.println("LED_3M OFF"); 
  }
  if (distance <=M_2)
  {
    digitalWrite(LED_2M,HIGH);
    Serial.println("LED_2M ON"); 
  }
  else
  {
    digitalWrite(LED_2M,LOW);
    Serial.println("LED_2M OFF"); 
  }
  if (distance <=M_1)
  {
    digitalWrite(LED_1M,HIGH);
    Serial.println("LED_1M ON"); 
  }
  else
  {
    digitalWrite(LED_1M,LOW);
    Serial.println("LED_1M OFF"); 
  }
  delay(50); 
}


