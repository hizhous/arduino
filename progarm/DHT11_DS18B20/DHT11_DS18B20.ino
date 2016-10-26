#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK               0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2

class dht11
{
public:
  int read(int pin)
  {
    // BUFFER TO RECEIVE
    uint8_t bits[5];
    uint8_t cnt = 7;
    uint8_t idx = 0;

    // EMPTY BUFFER
    for (int i=0; i< 5; i++) bits[i] = 0;

    // REQUEST SAMPLE
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    pinMode(pin, INPUT);

    // ACKNOWLEDGE or TIMEOUT
    unsigned int loopCnt = 10000;
    while(digitalRead(pin) == LOW)
      if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

    loopCnt = 10000;
    while(digitalRead(pin) == HIGH)
      if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

    // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
    for (int i=0; i<40; i++)
    {
      loopCnt = 10000;
      while(digitalRead(pin) == LOW)
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

      unsigned long t = micros();

      loopCnt = 10000;
      while(digitalRead(pin) == HIGH)
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

      if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
      if (cnt == 0)   // next byte?
      {
        cnt = 7;    // restart at MSB
        idx++;      // next byte!
      }
      else cnt--;
    }

    // WRITE TO RIGHT VARS
    // as bits[1] and bits[3] are allways zero they are omitted in formulas.
    humidity    = bits[0]; 
    temperature = bits[2]; 

    uint8_t sum = bits[0] + bits[2];  

    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
    return DHTLIB_OK;
  }
  int humidity;
  int temperature;
};

double Fahrenheit(double celsius) 
{
  return 1.8 * celsius + 32;
}

double Kelvin(double celsius)
{
  return celsius + 273.15;
}

// �ο�: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
  double A0= 373.15/(273.15 + celsius);
  double SUM = -7.90298 * (A0-1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
  SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
  SUM += log10(1013.246);
  double VP = pow(10, SUM-3) * humidity;
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558-T);
}

// �ο�: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity/100);
  double Td = (b * temp) / (a - temp);
  return Td;
}

#include <OneWire.h>

#define DHT11PIN 10
#define PIN 2

OneWire  ds(PIN);
dht11 DHT11;

char c_NO_CONTENT[] = "null";
char c_DHT11_T[] = "[DHT_T]";
char c_DHT11_H[] = "[DHT_H]";
char c_DS1820[] = "[DS_T]";
char c_TIME[] = "[Time]";

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  readDHT11();
  readDS18B20();
}

void readDHT11()
{
  Serial.print(c_TIME);
  Serial.print(":[");
  Serial.print(millis());
  Serial.print("]-");
  int chk = DHT11.read(DHT11PIN);
  switch (chk)
  {
  case DHTLIB_OK: 
    // print temperature
    Serial.print(c_DHT11_T);
    Serial.print(":[");
    Serial.print((float)DHT11.temperature, 2);
    Serial.print("]-");
    // print humidity
    Serial.print(c_DHT11_H);
    Serial.print(":[");
    Serial.print((float)DHT11.humidity, 2);
    Serial.print("]");

    break;
  default:
    // print temperature
    Serial.print(c_DHT11_T);
    Serial.print(":[");
    Serial.print(c_NO_CONTENT);
    Serial.print("]-");
    // print humidity
    Serial.print(c_DHT11_H);
    Serial.print(":[");
    Serial.print((float)DHT11.humidity, 2);
    Serial.print("]");
    break;
  }

  Serial.println();

  /*
  //Serial.print("Read sensor: ");
   switch (chk)
   {
   case DHTLIB_OK: 
   //Serial.println("OK"); 
   break;
   case DHTLIB_ERROR_CHECKSUM: 
   //Serial.println("Checksum error"); 
   break;
   case DHTLIB_ERROR_TIMEOUT: 
   //Serial.println("Time out error"); 
   break;
   default: 
   //Serial.println("Unknown error"); 
   break;
   }
   
   Serial.print("Humidity (%): ");
   Serial.println((float)DHT11.humidity, 2);
   
   Serial.print("Temperature (oC): ");
   Serial.println((float)DHT11.temperature, 2);
   
   Serial.print("Temperature (oF): ");
   Serial.println(Fahrenheit(DHT11.temperature), 2);
   
   Serial.print("Temperature (K): ");
   Serial.println(Kelvin(DHT11.temperature), 2);
   
   Serial.print("Dew Point (oC): ");
   Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));
   
   Serial.print("Dew PointFast (oC): ");
   Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));
   */
}

void readDS18B20(void) {
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  ds.reset_search();

  Serial.print(c_TIME);
  Serial.print(":[");
  Serial.print(millis());
  Serial.print("]-");
  Serial.print(c_DS1820);
  Serial.print(":[");

  if ( !ds.search(addr)) {
    Serial.print("!ds.search(addr)");//c_NO_CONTENT);
    Serial.println("]");
    //Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  /*
  Serial.print("ROM =");
   for( i = 0; i < 8; i++) {
   Serial.write(' ');
   Serial.print(addr[i], HEX);
   }*/

  if (OneWire::crc8(addr, 7) != addr[7]) {
    //Serial.println("CRC is not valid!");
    Serial.print("!= addr[7]");//c_NO_CONTENT);
    Serial.println("]");
    return;
  }
  //Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
  case 0x10:
    //Serial.println("  Chip = DS18S20");  // or old DS1820
    type_s = 1;
    break;
  case 0x28:
    //Serial.println("  Chip = DS18B20");
    type_s = 0;
    break;
  case 0x22:
    //Serial.println("  Chip = DS1822");
    type_s = 0;
    break;
  default:
    //Serial.println("Device is not a DS18x20 family device.");
    Serial.print("addr[0] default");//c_NO_CONTENT);
    Serial.println("]");
    return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present,HEX);
  //Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    //Serial.print(data[i], HEX);
    //Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(data, 8), HEX);
  //Serial.println();

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
  Serial.print((float)raw / 16.0,2);
  Serial.println("]");
  //fahrenheit = celsius * 1.8 + 32.0;
  //Serial.print("  Temperature = ");
  //Serial.print(celsius);
  //Serial.print(" Celsius, ");   
  //Serial.print(fahrenheit);
  //Serial.println(" Fahrenheit");
}








