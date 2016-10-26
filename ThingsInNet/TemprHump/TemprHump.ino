/*
*/
#include <Wire.h>//声明I2C库文件
#include <DHT22.h>
#define DHT22_PIN 5

float aimTemp = 0;
float aimHumi = 0;
DHT22 myDHT22(DHT22_PIN);

const int READ_TYPE_BOUTH = 0;
const int READ_TYPE_TEMPR = 1;
const int READ_TYPE_HUMI = 2;
int read_type = READ_TYPE_BOUTH;

//初始化
void setup()
{
  Wire.begin(2);                // 加入 i2c 总线，设置从机地址为 #4
  Wire.onReceive(receiveEvent); //注册接收到主机字符的事件
  Wire.onRequest(requestEvent); // 注册主机通知从机上传数据的事件
  Serial.begin(9600);           //设置串口波特率
  Serial.println("start.."); 
}

const int char_s = 6;
char c_tempr[char_s] ;
char c_humi[char_s];

//主程序
void loop()
{
  DHT22_Read();
  Serial.print(aimTemp);
  Serial.print("C ");
  Serial.print(aimHumi);
  Serial.println("%");
  delay(2000);
}

// 当从机接收到主机字符，执行该事件
void receiveEvent(int howMany)
{/*
  Serial.println(howMany);
 char cc;
 while( Wire.available()>0) // 循环执行，直到数据包只剩下最后一个字符
 {
 cc = Wire.read(); // 作为字符接收字节
 Serial.print(cc);         // 把字符打印到串口监视器中
 }
 Serial.println();
 //接收主机发送的数据包中的最后一个字节
 */
  if(Wire.available() > 0){
    int x = Wire.read();
    if(READ_TYPE_BOUTH == x ||
      READ_TYPE_TEMPR == x ||
      READ_TYPE_HUMI == x){
      read_type = x;
    }
  }
}

//当主机通知从机上传数据，执行该事件
void requestEvent()
{
  switch(read_type){
  case READ_TYPE_TEMPR:
    Wire.write(c_tempr);
    break;
  case READ_TYPE_HUMI:
    Wire.write(c_humi);
    break;
  }
}

void putFloatIntoChar(float f,char* c){
  int i1 = (int)f;
  int i2 = f*100 - i1*100;
  char c1[char_s];
  char c2[char_s];
  for(int i = 0;i < char_s;i++){
    c1[i] = '0';
    c2[i] = '0';
  }
  itoa(i1, c1, 10);
  itoa(i2, c2, 10);
  int c_len1 = strlen(c1);
  int c_len2 = strlen(c2);
  int ind = 0;
  for(int j=0;j < c_len1;++j,++ind){
    c[ind] = c1[j];
  }
  c[ind] = '.';
  ++ind;
  for(int j = 0;j < c_len2;++j,++ind){
    c[ind] = c2[j];
  }
}

void DHT22_Read()
{ 
  DHT22_ERROR_t errorCode = myDHT22.readData();
  switch(errorCode)
  {
  case DHT_ERROR_NONE:
  case DHT_ERROR_CHECKSUM:
    aimTemp = myDHT22.getTemperatureC();
    aimHumi = myDHT22.getHumidity();
    break;
  default:
    break;
  }
  makeContent(aimTemp,aimHumi);
}

void makeContent(float tempr,float hum){
  for(int i = 0;i < char_s;++i){
    c_tempr[i] = ';';
  }

  for(int i = 0;i < char_s;++i){
    c_humi[i] = ';';
  }
  putFloatIntoChar(aimTemp,c_tempr);
  putFloatIntoChar(aimHumi,c_humi);
}










