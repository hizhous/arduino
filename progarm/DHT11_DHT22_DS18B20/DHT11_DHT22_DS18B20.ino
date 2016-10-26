//http://www.dfrobot.com.cn/community/forum.php?mod=viewthread&tid=1979&highlight=sht10
#include <DHT22.h>
#include <OneWire.h>
#define DHT11PIN 2
#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK				0
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

dht11 DHT11;
#define PIN 7
#define PIN2 6
//DHT22 DHT1(2);
DHT22 DHT2(4);
OneWire  ds(PIN);
OneWire  ds1(PIN2);
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  
  DHT22_ERROR_t errorCode;
  int chk = DHT11.read(DHT11PIN);
  Serial.print(" DHT1: ");
  //Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                Serial.print((float)DHT11.temperature,2);
                Serial.print("C ");
                Serial.print((float)DHT11.humidity,2);
                Serial.print("%");
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }

  /*
  
  errorCode = DHT1.readData();
  if(errorCode==DHT_ERROR_NONE){
    Serial.print(DHT1.getTemperatureC(),2);
    Serial.print("C ");
    Serial.print(DHT1.getHumidity(),2);
    Serial.print("%");
  }
  else {
    Serial.print("Error");
  }*/
  Serial.print(" DHT2: ");
  errorCode = DHT2.readData();
  if(errorCode==DHT_ERROR_NONE){
    Serial.print(DHT2.getTemperatureC(),2);
    Serial.print("C ");
    Serial.print(DHT2.getHumidity(),2);
    Serial.print("%");
  }
  else {
    Serial.print("Error");
  }
  readDs18b20(ds);
  readDs18b20(ds1); Serial.println("");
  delay(2000);
}

void readDs18b20(OneWire  ds){
Serial.print(" DS18B20: ");
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  ds.reset_search();

  if ( !ds.search(addr)) {
    Serial.print("Error");
    //Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(3000);
    return;
  }
  
  if (OneWire::crc8(addr, 7) != addr[7]) {
    //Serial.println("CRC is not valid!");
    Serial.print("Error");delay(3000);
    return;
  }
  
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
    Serial.print("Error");delay(3000);
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
  Serial.print(celsius,2);
    Serial.print("C ");
  
   
}