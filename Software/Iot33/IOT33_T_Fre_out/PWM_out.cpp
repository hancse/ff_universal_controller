/*-------------------------------------------
  ADC Converter - function file
  IOT33
  Trung Nguyen 2021
  version 1.00 - 07/05/2021
  
--------------------------------------------*/ 



#ifndef PWM_out_CPP
#define PWM_out_CPP

#include "PWM_out.h"
#include <SAMD21turboPWM.h>

TurboPWM pwm;
float Ta_cal;
float Ta_rev;
int pwm_out;

/*-------------------------------------------
 Set the resolution for analog reading
--------------------------------------------*/ 

void PWM_init(){

  pwm.setClockDivider(9, false);
  pwm.timer(1, 1, 1000, true);
   
}


void PWM_out(int pwm_val){
  /*
   *  set PWM output
   */
   // pin out and % of PWM 200 min 20 %
  pwm_out = (-1*pwm_val) + 1023;
  
  Serial.println("pwm control value");
  //Serial.println(pwm_out);
  Serial.println(pwm_val);

  Ta_cal = (pwm_val*212.77/1023) - 68.085;
  
  Serial.println("Temperature output with PWM");
  Serial.println(Ta_cal);

  //Ta_rev = ((1023-pwm_val)*212.77/1023) - 68.085;
  //Serial.println("Temperature output with reverse PWM");
  //Serial.println(Ta_rev);

  // change back to 409
  if (pwm_val >=562){ //562 ,  48.8
    Serial.println("value over control range");
    
    pwm_val = 562;
    pwm_out = (-1*pwm_val) + 1023;
    pwm.analogWrite(PWM_pin, pwm_out);
    }
  else if (pwm_val <= 290){ //346, 4
    Serial.println("value under control range");
    pwm_val = 290;
    pwm_out = (-1*pwm_val) + 1023;
    pwm.analogWrite(PWM_pin, pwm_out);
    }
  else{
    pwm.analogWrite(PWM_pin, (pwm_out));
    }
  
}


#endif //__ESP32_ADC_CPP
