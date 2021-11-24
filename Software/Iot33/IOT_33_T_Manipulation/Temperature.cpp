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

/*
 This sample code demonstrates how to use the SimpleKalmanFilter object. 
 Use a potentiometer in Analog input A0 as a source for the reference real value.
 Some random noise will be generated over this value and used as a measured value.
 The estimated value obtained from SimpleKalmanFilter should match the real
 reference value.
 SimpleKalmanFilter(e_mea, e_est, q);
 e_mea: Measurement Uncertainty 
 e_est: Estimation Uncertainty 
 q: Process Noise
 */
SimpleKalmanFilter simpleKalmanFilter(1, 1,0.1);

// Calculated value
float R1 = 9820;              // fix resistor value is 10k
float logR2, R2, T, Tf;
float estimated_value;

// Steinhart-Hart approximation (NTC)
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void temp_init(){
     
   analogReadResolution(10);
   
}

float tempSen_read_float() {
  /*
   *  Read temperature sensor with adc and return the resistance value of temperature sensor.
   */
  
  int Vo = analogRead(THERMISTOR_PIN);
  estimated_value = simpleKalmanFilter.updateEstimate(Vo);
  R2 = R1 * (1023.0 / estimated_value - 1.0); // change to 1023 if resolution change to 10 bit
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  float Tc = T - 273.15;
  //Tf = (Tc * 9.0)/ 5.0 + 32.0; 

  //Serial.print("Temperature: "); 
  //Serial.print(Tf);
  //Serial.print(" F; ");
  //Serial.print(Tc);
  //Serial.println(" C");   
  return R2;
}

#endif //__ESP32_ADC_CPP
