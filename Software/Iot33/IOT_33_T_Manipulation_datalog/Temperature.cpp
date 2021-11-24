/*-------------------------------------------
  ADC Converter - function file
  IOT33
  Trung Nguyen 2021
  version 1.00 - 07/05/2021
  
--------------------------------------------*/ 


#include <SimpleKalmanFilter.h>
#ifndef Temperature_CPP
#define Temperature_CPP
#include "Temperature.h"
#include "Arduino.h"

SimpleKalmanFilter simpleKalmanFilter(1, 1,0.1);
SimpleKalmanFilter M_KalmanFilter(1, 1,0.1);

// Calculated value
float R1 = 9820;              // fix resistor value is 10k
float logR2, R2, T, Tf, R2_C;
float estimated_value;
float estimated_value_C;

// Steinhart-Hart approximation (NTC)
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

/*-------------------------------------------
 Set the resolution for analog reading
--------------------------------------------*/ 

void Temp_init(){
     
   analogReadResolution(10);
   
}


float tempSen_read_float() {
  /*
   *  Read temperature sensor with adc and return the resistance value of temperature sensor.
   */
  
  int Vo = analogRead(THERMISTOR_PIN);
  estimated_value = simpleKalmanFilter.updateEstimate(Vo);
  R2 = R1 * (1023.0 / estimated_value - 1.0); // change to 1023 if resolution change to 10 bit
  //Serial.println(R2);

  return R2;
}

float Manual_Control() {
  /*
   *  Read adc and return the resistance value of potential meter.
   */
  
  int V_1 = analogRead(Manual_Control_PIN);
  estimated_value_C = M_KalmanFilter.updateEstimate(V_1);
  R2_C = R1 * ((1023.0 / estimated_value_C) - 1.0); // change to 1023 if resolution change to 10 bit
  Serial.print("Manual input value: ");
  Serial.println(R2_C);
  Serial.println("___________________");
  
  return R2_C;
}

#endif //__ESP32_ADC_CPP
