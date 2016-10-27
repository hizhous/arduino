#ifndef _COMM_
#define _COMM_

#define PIN_HC_SR501 2       // Pin of HC-SR501
#define PIN_HC_SR501_PRINT 3 // Pin for HC-SR501's out
#define chipSelect 4
#define PIN_DHT22 5          // Pin of DHT22
#define Trig 7// 定义超声波信号发出接口
#define Echo 8 // 定义超声波信号接收接口
#define PIN_FAN 9 // 风扇开关

#define SwitchPin 14//A0
#define Pin_WriteToFile 15// A1 // 控制是否写入日志
#define Pin_TestInput 16//A2
#define Pin_TestOutput 17//A3

//#define Pin_HC_SR04 A2//A2 HCSR04 电源端控制脚

#define HC_SR501_OFF 0// HC-SR501's result is off
#define HC_SR501_ON 1// HC-SR501's result is on

#define DHT22_INDEX_TEMAERATURE 0  // Index of temperature
#define DHT22_INDEX_HUMIDITY 1     // Index of humidity
#define DHT_ERROR_DEFAULT (DHT_ERROR_NONE - 1)  // DHT22 default result(unknow)

#define LoopTimeDelay 100

#define DEFAULT_TIME -1
#define HC_SR501_PEOPLE_COME 1
#define HC_SR501_PEOPLE_NO_COME -1
#define TIGGER_OPEN_FAN_TIME 30*1000 //保持状态10秒钟触发开启风扇
#define TIGGER_CLOSE_FAN_TIME 180000 //延迟10分钟关闭风扇
#define TIGGER_OPEN_FAN_DISTANCE 10//触犯开启风扇的距离

#define c_HC_SR04 "[HC-SR04]"
#define c_DHT22_T "[DHT_T]"
#define c_DHT22_H "[DHT_H]"
#define c_DHT22_T_average "[DHT_T_A]"
#define c_DHT22_H_average "[DHT_H_A]"
#define STRING_ST_HCSR04 "already_setup_hcsr04"
#define c_HC_SR501 "[HC-SR501]"
#define c_TIME "[Time]"
#define c_FAN "[FAN]"

#endif
