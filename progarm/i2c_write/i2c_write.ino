 /*主机向从机循环发送字符串"light is "和字节x，x为1或0
从机接收到主机发来的数据后，当主机通知从机向它上传数据时
会把x值再上传回主机，然后赋值给变量c。
当主机程序判断c为1，则点亮LED，否则熄灭LED。*/
#include <Wire.h>//声明I2C库文件
#define LED 13
byte x = 0;//变量x决定LED的亮灭
//初始化
void setup()
{
  Serial.begin(9600);           
  Wire.begin(); // 加入 i2c 总线，作为主机
  pinMode(LED,OUTPUT);//设置数字端口13为输出
}
//主程序
void loop()
{
  Wire.beginTransmission(4); //发送数据到设备号为4的从机
  Wire.write("light is ");        // 发送字符串"light is "
  Wire.write(x);              // 发送变量x中的一个字节  
  Wire.endTransmission();    // 停止发送

  x++;//变量x加1
  if(x==2)//如果变量x的值为2，则把x值转为0
  x=0;
  delay(1000);//延时1s
  
  Wire.requestFrom(4, 10);    //通知4号从机上传1个字节
  while(Wire.available()>0)    // 当主机接收到从机数据时
  { 
    byte c = Wire.read(); //接收一个字节赋值给c
    //判断c为1，则点亮LED，否则熄灭LED。
    if(c==1)
    {digitalWrite(LED,LOW);}
    else
     {digitalWrite(LED,HIGH);}
     
    Serial.print(c);         // 把字符打印到串口监视器中
  }
  Serial.println();
  delay(1000);//延时1s
}