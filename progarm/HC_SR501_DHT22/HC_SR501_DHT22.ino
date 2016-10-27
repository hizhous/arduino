/*
   HC-SR501:
 Working voltage: DC 4.5-20V
 Trigger mode: L = Never repeat trigger; H = Repeat tirgger
 */
/*
  DHT22:
 Working voltage: 3.3-6V
 The sensor can only be read from every 1-2s, and requires a minimum
 */
#include <DHT22.h>

#define PIN_DHT22 5          // Pin of DHT22
#define PIN_HC_SR501 2       // Pin of HC-SR501
#define PIN_HC_SR501_PRINT 3 // Pin for HC-SR501's out

const int HC_SR501_OFF = 0;  // HC-SR501's result is off
const int HC_SR501_ON = 1;   // HC-SR501's result is on
int HC_SR501_Real_Result = HC_SR501_OFF;  // HC-SR501's real result
int HC_SR501_Last_Result = HC_SR501_OFF;  // record HC-SR501 last result for calc delay time
int HC_SR501_Result = HC_SR501_OFF;       // HC-SR501's calc result,deal with delay time
const long ContinueTimeDefault = -1;      // HC_SR501 now has no continue time
const long BlockTime = 3500;              // HC-SR501's block time
long ContinueTime = ContinueTimeDefault;  // HC-SR501's on to off block time hold on the "ON" status continue time

const int DHT22_INDEX_TEMAERATURE = 0;  // Index of temperature
const int DHT22_INDEX_HUMIDITY = 1;     // Index of humidity
const int DHT_ERROR_DEFAULT = DHT_ERROR_NONE - 1;  // DHT22 default result(unknow)
const float DHT22_UNKNOW_VALUE = -1; // DHT22 can not read temperature and humidity
const long DHT22_READ_INTERVAL = 2000;// The sensor can only be read from every 1-2s
long Last_Read_time = 0 - DHT22_READ_INTERVAL;

DHT22 dht22(PIN_DHT22);      // Code object of DHT22
int DHT22_Read_Result = DHT_ERROR_DEFAULT;   // DHT22  read result code
float DHT22_TemperatureC = DHT22_UNKNOW_VALUE;  // Temperature
float DHT22_Humidity = DHT22_UNKNOW_VALUE;      // Humidity

void DHT22_Read()
{ 
  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on.
  //delay(2000);
  long nowReadTime = millis();
  if(nowReadTime - Last_Read_time >= DHT22_READ_INTERVAL)
  {
    Last_Read_time = nowReadTime;
    switch(dht22.readData())
    {
    case DHT_ERROR_NONE:
      DHT22_Read_Result = DHT_ERROR_NONE;
      DHT22_TemperatureC = dht22.getTemperatureC();
      DHT22_Humidity = dht22.getHumidity();
      return;
    case DHT_ERROR_CHECKSUM:
      DHT22_Read_Result = DHT_ERROR_CHECKSUM;
      DHT22_TemperatureC = dht22.getTemperatureC();
      DHT22_Humidity = dht22.getHumidity();  
      return;
    case DHT_BUS_HUNG:
      DHT22_Read_Result = DHT_BUS_HUNG;
      break;
    case DHT_ERROR_NOT_PRESENT:
      DHT22_Read_Result = DHT_ERROR_NOT_PRESENT;
      break;
    case DHT_ERROR_ACK_TOO_LONG:
      DHT22_Read_Result = DHT_ERROR_ACK_TOO_LONG;
      break;
    case DHT_ERROR_SYNC_TIMEOUT:
      DHT22_Read_Result = DHT_ERROR_SYNC_TIMEOUT;
      break;
    case DHT_ERROR_DATA_TIMEOUT:
      DHT22_Read_Result = DHT_ERROR_DATA_TIMEOUT;
      break;
    case DHT_ERROR_TOOQUICK:
      DHT22_Read_Result = DHT_ERROR_TOOQUICK;
      break;
    default:
      DHT22_Read_Result = DHT_ERROR_DEFAULT;
      break;
    }
    DHT22_TemperatureC = DHT22_UNKNOW_VALUE;
    DHT22_Humidity = DHT22_UNKNOW_VALUE;
  }
}

/*
 To make HC-SR501 react from off to on immediately
 we should hide the HC-SR501's on to off block time
 so we let the HC-SR501's on to off block time hold on the "ON" status during the block time
 */
int HC_SR501_GET_RESULT(int result)
{
  HC_SR501_Real_Result = result;// record real result
  if(HC_SR501_Last_Result != HC_SR501_Real_Result)//hc-sr501's status has change
  {
    if(HC_SR501_Real_Result == HC_SR501_OFF)// from on to off
    {
      if(ContinueTime == ContinueTimeDefault)// start calc continue time
      {
        ContinueTime = millis();
      }
      if(millis() - ContinueTime >= BlockTime)// continue time catch up with block time
      {
        HC_SR501_Last_Result = HC_SR501_Real_Result;// change hc-sr501's status from on to off
      }
    }
    else // from off to on, no need to deal with block time, change status immediately
    {
      HC_SR501_Last_Result = HC_SR501_Real_Result;
    }
  }
  else // hc-sr501's status has no change
  {
    ContinueTime = ContinueTimeDefault;  // reset continue time to null
  }
  return HC_SR501_Last_Result;// return calc result
}

const int chipSelect = 4;
#include <SdFat.h>
SdFat sd;
SdFile myFile;

const int TestPinInput = 8;
const int TestPinOutput = 7;

void setup()
{
  Serial.begin(9600);
  Serial.println("Application start:");
  pinMode(PIN_HC_SR501,INPUT); //设置人体红外接口为输入状态
  pinMode(PIN_HC_SR501_PRINT,OUTPUT); //设置蜂鸣器接口为输出状态
  pinMode(TestPinInput,INPUT); 
  pinMode(TestPinOutput,OUTPUT);
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();

}

char c_NO_CONTENT[] = "null";
char c_DHT22_T[] = "[DHT_T]";
char c_DHT22_H[] = "[DHT_H]";
char c_HC_SR501[] = "[HC-SR501]";
char c_TIME[] = "[Time]";

void loop()
{
  boolean write_to_file = false;
  if (!myFile.open("data.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening data.txt for write failed");
    Serial.println("opening data.txt for write failed");
    return;
  }
  else{
    write_to_file = true;
  }
  long time = millis();
  if(write_to_file){
    myFile.print(c_TIME);
    myFile.print(":[");
    myFile.print(time);
    myFile.print("]-");
    Serial.println("write time success");
  }
  else{
    Serial.print(c_TIME);
    Serial.print(":[");
    Serial.print(time);
    Serial.print("]-");
  }

  DHT22_Read();
  if(write_to_file)
  {
    myFile.print(c_DHT22_T);
    myFile.print(":[");
    myFile.print(DHT22_TemperatureC);
    myFile.print("]-");
    myFile.print(c_DHT22_H);
    myFile.print(":[");
    myFile.print(DHT22_Humidity);
    myFile.print("]");
    myFile.println();
    Serial.println("write dht22 success");
  }
  else{
    Serial.print(c_DHT22_T);
    Serial.print(":[");
    Serial.print(DHT22_TemperatureC);
    Serial.print("]-");
    Serial.print(c_DHT22_H);
    Serial.print(":[");
    Serial.print(DHT22_Humidity);
    Serial.print("]");
    Serial.println();
  }

  if(write_to_file){
    myFile.print(c_TIME);
    myFile.print(":[");
    myFile.print(time);
    myFile.print("]-");
    Serial.println("write time success");
  }
  else{
    Serial.print(c_TIME);
    Serial.print(":[");
    Serial.print(time);
    Serial.print("]-");
  }
  HC_SR501_Result = HC_SR501_GET_RESULT(digitalRead(PIN_HC_SR501));
  if(write_to_file)
  {
    myFile.print(c_HC_SR501);
    myFile.print(":[");
    myFile.print(HC_SR501_Result);
    myFile.print("]");
    myFile.println();
    Serial.println("write hc-sr501 success");
  }
  else{
    Serial.print(c_HC_SR501);
    Serial.print(":[");
    Serial.print(HC_SR501_Result);
    Serial.print("]");
    Serial.println();
  }
  digitalWrite(PIN_HC_SR501_PRINT,HC_SR501_Result);
  if(write_to_file){
    myFile.close();
  }

  int testInpu = digitalRead(TestPinInput);
  Serial.print("TestPinInput:");
  Serial.println(testInpu);
  digitalWrite(TestPinOutput,testInpu);
  delay(100); //延时100毫秒
}










