// Trig接Arduino板的Digital 1口，触发测距；Echo接Digital 2口，接收距离信号。
// HC-SR04 必须接5V VCC
int Trig=7; // 定义超声波信号发出接口
int Echo=8; // 定义超声波信号接收接口

#include <OneWire.h>

void setup()
{
  Serial.begin(9600);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
}
int calcAverageDistance()
{
  const int averLength = 100;
  const int dis_err = 50;
  const double maxDis = 0.2;
  const int maxDataArray = 5;
  long aver[averLength];
  long storageAver[averLength];
  for (int i = 0; i < averLength; ++i) {
    aver[i] = readHCSR04();
  }
  bubble(aver,averLength);
  long sum = -dis_err;
  long count = 1;
  long start = 0;
  long aimCount = 0;
  for (int i = 0; i < averLength; ++i) {
    long _aver = sum / count;
    if(aver[i] - _aver < dis_err){
      sum += aver[i];
      ++count;
    }
    else{
      start = i;
      storageAver[i] = aver[i];
      sum = aver[i];
      count = 1;
      ++aimCount;
    }
    long lastAver = _aver;
    _aver = sum / count;
    if(lastAver == _aver){
      storageAver[i] = _aver;
    }
    else{
      for(int j = start;j <= i;++j){
        storageAver[j] = _aver;
        //Serial.print(j);
        //Serial.print("-");
        //Serial.println(storageAver[j]);
      }
    }
  }
  const int _aimCount = aimCount;
  int result = -1;
  if(_aimCount <= maxDataArray)
  {
    int res[_aimCount][2];
    start = 0;
    for(int i = 0;i < _aimCount;++i)
    {
      res[i][0] = storageAver[start];
      int j = start;
      int count = 0;
      for(; j <= averLength;++j)
      {
        if(j != averLength && storageAver[j] == res[i][0])
        {
          ++count;
        }
        else{
          res[i][1] = count;
          start = j;
          break;
        }
      }
    }
    int maxCount = 0;
    for(int i = 0;i < aimCount;++i)
    {
      if(res[i][1] > res[maxCount][1]){
        maxCount = i;
      }
      //Serial.print(res[i][0]);
      //Serial.print("-");
      //Serial.println(res[i][1]);
    }
    int errorCount = -1;
    double errorDis = 1;
    for(int i = 0;i < aimCount;++i){
      if(i != maxCount){
        if(res[i][1] == res[maxCount][1]){
          errorCount = i;
          errorDis = 0.0;
          break;
        }
        double bigCount = (res[i][1] > res[maxCount][1])?(res[i][1]):(res[maxCount][1]);
        double smallCount = (res[i][1] < res[maxCount][1])?(res[i][1]):(res[maxCount][1]);
        errorDis = (bigCount - smallCount) / bigCount;
        if(errorDis <= maxDis){
          errorCount = i;
          break;
        }
      }
    }
    if(errorCount == -1){
      char c[100];
      sprintf(c,"max count is : %d - %d",res[maxCount][0],res[maxCount][1]);
      Serial.println(c);
      result = res[maxCount][0];
    }
    else{
      char c[100];
      sprintf(c,"no result,error is: %d - %d , %d - %d , %f",
      res[maxCount][0],res[maxCount][1],
      res[errorCount][0],res[errorCount][1],
      errorDis);
      Serial.println(c);
      result = -1;
    }
  }
  else{
    Serial.print("no result,to more data");
    result = -1;
  }
  return result;
}

boolean a = true;
void loop()
{
  if(a){
    //int res = calcAverageDistance();
    //Serial.print("result: ");
    //Serial.println(res);
    calcAverDis();
    a = false;
  }
}
void calcAverDis()
{
  Serial.println("calc average start...");
  long averDis = 0;
  const int maxValidCalcAverageDisCount = 5;//有效的计算平均值次数上限
  const int maxAllCalcAverageDisCount = 10;//总共计算次数上限
  int validCalcAverageDisCount = 0;//有效的计算平均值次数
  int allCalcAverageDisCount = 0;//总共计算次数
  while(validCalcAverageDisCount < maxValidCalcAverageDisCount 
    &&  allCalcAverageDisCount < maxAllCalcAverageDisCount)
  {
    allCalcAverageDisCount++;
    Serial.println("now calcing...");
    Serial.print("max Count is: ");
    Serial.print(allCalcAverageDisCount);
    Serial.print("\tvalid Count is: ");
    Serial.println(validCalcAverageDisCount);
    int tempDis = -1;
    int calcCount = 0;
    const int maxCalcCount = 3;//单独一次计算最大次数
    do
    {
      ++calcCount;
      Serial.print("temp calc time is: ");
      Serial.println(calcCount);
      tempDis = calcAverageDistance();
      Serial.print("\t result is: ");
      Serial.println(tempDis);
    }
    while(tempDis == -1 || calcCount < maxCalcCount);
    if(tempDis != -1)
    {
      ++validCalcAverageDisCount;
      averDis += tempDis;
      Serial.print("averDisSum is: ");
      Serial.print(averDis);
      Serial.print("\t valid count is: ");
      Serial.println(validCalcAverageDisCount);
    }
  }
  if(0 != validCalcAverageDisCount)
  {
    averDis /= validCalcAverageDisCount;
  }
  else{
    averDis = -1;
  }
  Serial.print("averDis is: ");
  Serial.println(averDis);
}

int readHCSR04()
{
  digitalWrite(Trig, LOW); // 使发出发出超声波信号接口低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW); // 保持发出超声波信号接口低电平
  long distance = pulseIn(Echo, HIGH); // 读出脉冲时间
  //  getTT();
  //Serial.println(distance);
  distance= distance /58; // 将脉冲时间转化为距离（单位：厘米）

  delay(50); 
  return distance;
}
OneWire  ds(10);  // Á¬½Óarduino10Òý½Å
float getTemp()
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return -1;
  }

  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1;
  }
  Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
  case 0x10:
    Serial.println("  Chip = DS18S20");  // or old DS1820
    type_s = 1;
    break;
  case 0x28:
    Serial.println("  Chip = DS18B20");
    type_s = 0;
    break;
  case 0x22:
    Serial.println("  Chip = DS1822");
    type_s = 0;
    break;
  default:
    Serial.println("Device is not a DS18x20 family device.");
    return -1;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present,HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // convert the data to actual temperature

  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");   
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  return celsius;

}

float temp = -1;
double getTT()
{
  if(temp == -1){
    temp = getTemp();
  }
  char cc[100];
  double sqr = sqrt((float)1+((float)((float)temp)/273));
  sprintf(cc,"%f",sqr);
  Serial.print(sqr,HEX);
  Serial.print("\t");
  double i = ((331*sqr/2000));
  Serial.print(i,HEX);
  Serial.print("\t");
  return i;
}

/* 定义两个参数：数组首地址与数组大小 */
void bubble(long a[], long n) 
{
  int i, j, temp;
  for (i = 0; i < n - 1; i++)
    for (j = i + 1; j < n; j++)
      /* 注意循环的上下限 */
      if (a[i] > a[j]) {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
}













