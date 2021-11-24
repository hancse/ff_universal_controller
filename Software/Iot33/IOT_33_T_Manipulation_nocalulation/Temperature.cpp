/*-------------------------------------------
  ADC Converter - function file
  Lora feather32u4 
  Trung Nguyen 2021
  version 1.00 - 07/05/2021
  
--------------------------------------------*/ 


#include <SimpleKalmanFilter.h>
#ifndef Temperature_CPP
#define Temperature_CPP
#include "Temperature.h"
#include "Arduino.h"

SimpleKalmanFilter simpleKalmanFilter(1, 1,0.1);

// Calculated value
float R1 = 9820;              // fix resistor value is 10k
float logR2, R2, T, Tf;
float estimated_value;

// Steinhart-Hart approximation (NTC)
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

/*-------------------------------------------
 Set the resolution for analog reading
--------------------------------------------*/ 

void Temp_init(){
     
   analogReadResolution(10);
   
}

/*-------------------------------------------
 Set the resolution for analog reading
--------------------------------------------*/ 


float tempSen_read_float() {
  /*
   *  Read temperature sensor with adc and return the resistance value of temperature sensor.
   */
  
  int Vo = analogRead(THERMISTOR_PIN);
  estimated_value = simpleKalmanFilter.updateEstimate(Vo);
  R2 = R1 * (1023.0 / estimated_value - 1.0); // change to 1023 if resolution change to 10 bit


  return R2;
}

#endif //__ESP32_ADC_CPP
