/*
  SD card datalogger

*/


#ifndef __Datalog_h
#define __Datalog_h

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"


void SD_init();

void SD_log(float Raw_NTC, float Raw_MQTT_Set_T,float Manual_Set_T, bool Remote);


void error(char *str);


#endif // 
