//http://www.cnblogs.com/xiaowuyi/p/3390024.html
// RS：寄存器选择，H = 数据选择，L = 指令选择
// RW：读写信号，H = 读，L = 写
// EN：使能端，下降沿使能
int LCD1602_RS=12;   
int LCD1602_RW=11;   
int LCD1602_EN=10;   
int DB[] = { 6, 7, 8, 9};//D7 = 6,D6 = 7,D5 = 8,D4 = 9
char str1[]="Welcome to";
char str2[]="wikicode.net";
char str3[]="This is the";
char str4[]="4-bit interface";

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

void LCD_Write_String(int X,int Y,char *s)
{
  LCD_SET_XY( X, Y );    //设置地址
  while (*s)             //写字符串
  {
    LCD_Data_Write(*s);   
    s ++;
  }
}

char start[] = "Start..";
char num[] = "0123456789";
char content[] = "DES:     CM";
int Trig=2; // 定义超声波信号发出接口
int Echo=3; // 定义超声波信号接收接口

void setup (void)
{
  Serial.begin(9600);
  Serial.println("setup"); 
  
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  
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
  delay(50);
  
  LCD_Write_String(5,1,start);//第1行，第4个地址起
  delay(2000);
}

void loop (void)
{
  LCD_Command_Write(0x01);
  delay(50);
  
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
  if(distance >= 100)
  {
    content[5] = num[(distance / 100)];
  }else{
    content[5] = 32;
  }
  distance %= 100;
  if(content[5] == 32)
  {
    if(distance >= 10)
    {
      content[6] = num[(distance / 10)];
    }else{
      content[6] = 32;
    }
  }else{
    content[6] = num[(distance / 10)];
  }
  content[7] = num[(distance % 10)];
  //content[5] = 52;content[6] = 51;content[7] = 54;
  Serial.println(content); 
  LCD_Write_String(2,0,content);//第1行，第4个地址起
  delay(2000);
  /*
  LCD_Write_String(3,0,str1);//第1行，第4个地址起
  delay(50);
  LCD_Write_String(2,1,str2);//第2行，第2个地址起
  delay(5000);
  LCD_Command_Write(0x01);//
  delay(50);
  LCD_Write_String(0,0,str3);
  delay(50);
  LCD_Write_String(0,1,str4);
  delay(5000);*/
  
  /*
  LCD_Command_Write(0x01);
  delay(50);
  LCD_Write_String(3,0,str1);//第1行，第4个地址起
  delay(50);
  LCD_Write_String(2,1,str2);//第2行，第2个地址起
  delay(5000);
  LCD_Command_Write(0x01);//
  delay(50);
  LCD_Write_String(0,0,str3);
  delay(50);
  LCD_Write_String(0,1,str4);
  delay(5000);
*/
}

