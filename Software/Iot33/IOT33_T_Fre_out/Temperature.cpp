/*-------------------------------------------
  ADC Converter - function file
  IOT33
  Trung Nguyen 2021
  version 1.00 - 07/05/2021
  
--------------------------------------------*/ 


#include <SimpleKalmanFilter.h>
#include <movingAvg.h>          // https://github.com/JChristensen/movingAvg
#ifndef Temperature_CPP
#define Temperature_CPP
#include "Temperature.h"
#include "Arduino.h"

SimpleKalmanFilter simpleKalmanFilter(1, 1,0.1);
SimpleKalmanFilter M_KalmanFilter(1, 1,0.1);
movingAvg mySensor(8);    // use 10 data points for the moving average

// Calculated value

float duration_h, duration_l, period, fre, DC, Ta;
float estimated_value;
float estimated_value_C;

// Steinhart-Hart approximation (NTC)
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

/*-------------------------------------------
 Set the resolution for analog reading
--------------------------------------------*/ 

void Temp_init(){

  analogReadResolution(10);
  mySensor.begin();
  pinMode(Pulse_read, INPUT);
   
}


float TempSen_read_float() {
  /*
   *  Read temperature smt172 and return the Temperature.
   */
  
  duration_h = pulseIn(Pulse_read, HIGH);
  duration_l = pulseIn(Pulse_read, LOW);
  period = duration_h + duration_l;
  //Serial.println(period);
  fre = 1000000/period;
  Serial.println("frequency:");
  Serial.println(fre);
  DC = (duration_h/period)*100;  //duty cycle
  //Serial.println(DC);

  int sensorMovingAvg = mySensor.reading(DC);

  Ta = sensorMovingAvg*212.77/100 - 68.085;  // Temperature calculation formular from datasheet
  Serial.println("Temperature:");
  Serial.println(Ta);
  
  return Ta;
  
}

float Manual_Control() {
  /*
   *  Read adc and return the resistance value of potential meter.
   */
  
  int V_1 = analogRead(Manual_Control_PIN);
  estimated_value_C = M_KalmanFilter.updateEstimate(V_1);
  //R2_C = R1 * ((1023.0 / estimated_value_C) - 1.0); // change to 1023 if resolution change to 10 bit
  Serial.print("Manual input value: ");
  Serial.println(estimated_value_C);
  Serial.println("___________________");
  
  return V_1;
}

#endif //__ESP32_ADC_CPP
