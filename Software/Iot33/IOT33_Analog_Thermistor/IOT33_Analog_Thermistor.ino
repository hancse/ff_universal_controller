#include <SimpleKalmanFilter.h>

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

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;
float R1 = 10000;
float logR2, R2, T, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;


void setup() {
Serial.begin(9600);
analogReadResolution(10);
}

void loop() {

  //Vo = analogRead(ThermistorPin);
  // change the resolution to 12 bits and read A0
  //analogReadResolution(10);
  //analogReadResolution(8);

  //analogReference(AR_EXTERNAL);


  //R2 = R1 * (4095.0 / (float)Vo - 1.0); // change value to 1023 or 4095
  //logR2 = log(R2);
  //T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  //float Tc = T - 273.15;
  //float Tf = (Tc * 9.0)/ 5.0 + 32.0; 

  //Serial.print("Temperature: "); 
  //Serial.print(Tf);
  //Serial.print(" F; ");
  //Serial.print(Tc);
  //Serial.println(" C");   

  //delay(500);

  // read a reference value from A0 and map it from 0 to 100
  float real_value = analogRead(A7); ///1024.0 * 100.0
  float estimated_value = simpleKalmanFilter.updateEstimate(real_value);
  
  R2 = R1 * (1023.0 / estimated_value - 1.0); // change value to 1023 or 4095
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  float Tc = T - 273.15;
  //Tf = (Tc * 9.0)/ 5.0 + 32.0; 
  
  // calculate the estimated value with Kalman Filter
  

  // send to Serial output every 100ms
  // use the Serial Ploter for a good visualization
  //if (millis() > refresh_time) {
    //Serial.print(real_value,4);
    //Serial.print(estimated_value,4);
    //Serial.println();
    
    //refresh_time = millis() + SERIAL_REFRESH_TIME;
  //}
    
  Serial.print(" estimated Temperature: "); 
  
  Serial.println(estimated_value);
  Serial.print(" Measure Temperature: ");  
  //Serial.print(Tf);
  //Serial.print(" F; ");
  Serial.print(R2);
  Serial.println(" C");


   delay(1000);

}