#include <Arduino.h>
#include "define.h"
#include "MsTimerTPU3.h"

char state = 0b00000000;
char cmd   = 0b00000000;

unsigned short int len = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  MsTimerTPU3::set(INT_TIME, &serial_read);
  MsTimerTPU3::start();
  pinMode(PIN_SW, INPUT);
}

void serial_read(){
  unsigned short int len = Serial1.available();
  static char buff[10] = {};
  static int co = 0;
  
  for (int i = len;i;i--){
      
    buff[co] = Serial1.read();
    if(buff[co] == '\n' /*&& buff[co-2] == 'U'*/) {
      state = buff[co-1];
      for (int j = co;j;j--){
        buff[j] = 0;
      }
      co=0;
    }else;
    
    co++;
    
    if (co > 9){
      for (int j = 0;j <= 9;j++){
        buff[j] = 0;
      }
      co=0;
    }else;
  }
}

void loop() {
  int sw = !digitalRead(PIN_SW);
  static int pre_sw = 0, count = 0;
  if (pre_sw == 1 && sw == 0){
    count++;
    if (count == 1) cmd = BIT_DEP;
    if (count == 2) cmd = BIT_STOR;
    if (count == 3) cmd = BIT_INITIAL;
    if (count == 4) cmd = BIT_DOWN;
    if (count == 5) cmd = BIT_ROT;
    if (count == 6) {
      cmd = BIT_EXT;
      count = 2;
    }
  }
  else cmd = 0b00000000;
  Serial1.print('L');
  Serial1.print(cmd);
  Serial1.print('\n');
  Serial.print(state,HEX);
  Serial.print('\t');
  Serial.print(cmd,HEX);
  Serial.print('\n');
  pre_sw = sw;
  delay(100);
}
