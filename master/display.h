#include <TM1637.h>

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
boolean clockPoint = true;
int seconds = 0;
int minutes = 0;

#define CLK D7
#define DIO D6
TM1637 tm1637(CLK,DIO);
