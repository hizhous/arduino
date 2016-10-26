#include <OneWire.h>
#include <DallasTemperature.h>

// ����DS18B20����⣬ʹ��DS18B20���򵥷���   DallasTemperature.zip (18.23 KB, ���ش���: 4348)
// ����DS18B20���ݿ�����arduino��2��IO��
#define ONE_WIRE_BUS 2
 
// ��ʼ�����ڵ������ϵĵ������豸
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
void setup(void)
{
  // ���ô���ͨ�Ų�����
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
 
  // ��ʼ��
  sensors.begin();
}
 
void loop(void)
{ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // ���������ȡ�¶�
  Serial.println("DONE");
   
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));  
}