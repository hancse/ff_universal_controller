/*-------------------------------------------------------------
  ADC Thermistor reading - header file
  IOT33
  Trung Nguyen 
  version 1.00 - 05/07/2021
--------------------------------------------------------------*/


#ifndef __Temperature_h
#define __Temperature_h


#define THERMISTOR_PIN 2
#define Manual_Control_PIN 7
//void ADC_init();

void Temp_init();

float tempSen_read_float();

float Manual_Control();


// ------ Public variable -------------------------------------

#endif // 
