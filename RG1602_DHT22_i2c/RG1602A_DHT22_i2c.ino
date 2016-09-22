#include <Wire.h>//声明I2C库文件
#include <DHT22.h>
#define DHT22_PIN 5

const int char_s = 6;
char c_tempr[char_s] ;
char c_humi[char_s];
DHT22 myDHT22(DHT22_PIN);

const int READ_TYPE_BOUTH = 0;
const int READ_TYPE_TEMPR = 1;
const int READ_TYPE_HUMI = 2;
int read_type = READ_TYPE_BOUTH;


//http://www.cnblogs.com/xiaowuyi/p/3390024.html
// RS：寄存器选择，H = 数据选择，L = 指令选择
// RW：读写信号，H = 读，L = 写
// EN：使能端，下降沿使能
int LCD1602_RS=12;   
int LCD1602_RW=11;   
int LCD1602_EN=10;   
int DB[] = { 6, 7, 8, 9};//D7 = 6,D6 = 7,D5 = 8,D4 = 9
char str3[]="Tempr   Humidity";
char str4[]="T:32.32 H:39.40%";

void LCD_Command_Write(int command)
{
  int i,temp;
  digitalWrite( LCD1602_RS,LOW);
  digitalWrite( LCD1602_RW,LOW);
  digitalWrite( LCD1602_EN,LOW);

  temp=command & 0xf0;
  for (i=DB[0]; i <= 9; i++)
  {
    digitalWrite(i,temp & 0x80);
    temp <<= 1;
  }

  digitalWrite( LCD1602_EN,HIGH);
  delayMicroseconds(1);
  digitalWrite( LCD1602_EN,LOW);

  temp=(command & 0x0f)<<4;
  for (i=DB[0]; i <= 9; i++)
  {
    digitalWrite(i,temp & 0x80);
    temp <<= 1;
  }

  digitalWrite( LCD1602_EN,HIGH);
  delayMicroseconds(1);
  digitalWrite( LCD1602_EN,LOW);
}

void LCD_Data_Write(int dat)
{
  int i=0,temp;
  digitalWrite( LCD1602_RS,HIGH);
  digitalWrite( LCD1602_RW,LOW);
  digitalWrite( LCD1602_EN,LOW);

  temp=dat & 0xf0;
  for (i=DB[0]; i <= 9; i++)
  {
    digitalWrite(i,temp & 0x80);
    temp <<= 1;
  }

  digitalWrite( LCD1602_EN,HIGH);
  delayMicroseconds(1);
  digitalWrite( LCD1602_EN,LOW);

  temp=(dat & 0x0f)<<4;
  for (i=DB[0]; i <= 9; i++)
  {
    digitalWrite(i,temp & 0x80);
    temp <<= 1;
  }

  digitalWrite( LCD1602_EN,HIGH);
  delayMicroseconds(1);
  digitalWrite( LCD1602_EN,LOW);
}

void LCD_SET_XY( int x, int y )
{
  int address;
  if (y ==0)    address = 0x80 + x;
  else          address = 0xC0 + x;
  LCD_Command_Write(address);
}

void LCD_Write_Char( int x,int y,int dat)
{
  LCD_SET_XY( x, y );
  LCD_Data_Write(dat);
}

// 第几个位置 第几行（从0开始） 内容
void LCD_Write_String(int X,int Y,char *s)
{
  LCD_SET_XY( X, Y );    //设置地址
  while (*s)             //写字符串
  {
    LCD_Data_Write(*s);   
    s ++;
  }
}

void setup (void)
{
  
  Wire.begin(2);                // 加入 i2c 总线，设置从机地址为 #4
  Wire.onReceive(receiveEvent); //注册接收到主机字符的事件
  Wire.onRequest(requestEvent); // 注册主机通知从机上传数据的事件
  Serial.begin(9600);           //设置串口波特率
  Serial.println("start.."); 
  
  int i = 0;
  for (i=6; i <= 12; i++)
  {
    pinMode(i,OUTPUT);
  }
  delay(100);
  LCD_Command_Write(0x28);//4线 2行 5x7
  delay(50);
  LCD_Command_Write(0x06);
  delay(50);
  LCD_Command_Write(0x0c);
  delay(50);
  LCD_Command_Write(0x80);
  delay(50);
  LCD_Command_Write(0x01);
  delay(500);

}

void loop (void)
{
  
  DHT22_Read();
  Serial.print(c_tempr);
  Serial.print("C ");
  Serial.print(c_humi);
  Serial.println("%");
  
  LCD_Command_Write(0x01);//
  delay(50);
  LCD_Write_String(0,0,str3);
  delay(50);
  LCD_Write_String(0,1,str4);
  
  delay(2000);

}

// 当从机接收到主机字符，执行该事件
void receiveEvent(int howMany)
{
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
  float aimTemp = myDHT22.getTemperatureC();
  float aimHumi = myDHT22.getHumidity();
  switch(errorCode)
  {
  case DHT_ERROR_NONE:
  case DHT_ERROR_CHECKSUM:
    aimTemp = myDHT22.getTemperatureC();
    aimHumi = myDHT22.getHumidity();
    makeContent(aimTemp,aimHumi);
    break;
  default:
    break;
  }
}

void makeContent(float tempr,float hum){
  for(int i = 0;i < char_s;++i){
    c_tempr[i] = '0';
  }

  for(int i = 0;i < char_s;++i){
    c_humi[i] = '0';
  }
  putFloatIntoChar(tempr,c_tempr);
  putFloatIntoChar(hum,c_humi);
  
  str4[0] = 'T';
  str4[1] = ':';
  
  for(int i = 0;i < 5;++i){
    str4[i+2] = c_tempr[i];
  }
  str4[7] = ' ';
  str4[8] = 'H';
  str4[9] = ':';
  for(int i = 0;i < 5;++i){
    str4[i+10] = c_humi[i];
  }
  str4[15] = '%';
}


