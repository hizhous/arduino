#include <Arduino.h>
#include "HCSR501.h"
#include "Tool.h"

int HC_SR501_Real_Result = HC_SR501_OFF;  // HC-SR501's real result
int HC_SR501_Last_Result = HC_SR501_OFF;  // record HC-SR501 last result for calc delay time
int HC_SR501_Result = HC_SR501_OFF;       // HC-SR501's calc result,deal with delay time
#define ContinueTimeDefault -1      // HC_SR501 now has no continue time
#define BlockTime 3500              // HC-SR501's block time
long ContinueTime = ContinueTimeDefault;  // HC-SR501's on to off block time hold on the "ON" status continue time

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

void HCSR501_Read()
{
  HC_SR501_Result = HC_SR501_GET_RESULT(digitalRead(PIN_HC_SR501));
  digitalWrite(PIN_HC_SR501_PRINT,HC_SR501_Result);
  //writeHC_SR501(getNowTime());
  writeSomething(c_HC_SR501,HC_SR501_Result);
}

boolean isPeopleCome()
{
  if(HC_SR501_Result == 1)
  {
    return true;
  }else{
    return false;
  }
}

