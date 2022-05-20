/*-------------------------------------------
  Heat Pump Temperature Manipulation
  IOT33 
  Trung Nguyen 2021
  version 1.00 - 18/11/2021
  
--------------------------------------------*/ 
#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif



#include "Arduino.h"
#include "arduino_secrets.h"
#include "MCP_42050.h"
#include "Temperature.h"
#include "Datalog.h"
#include "Arduino.h"
#include "PWM_out.h"

//int saadcBuffer[SAMPLES];  // variable to store the value coming from the sensor

// Define Analog Pin for thermistor input
long  mqtt_reconnect_time = 0;
float mqtt_reci_raw_t;
bool  Remote_Control;
float manual_control_set;

void setup()
{
   Serial.begin(9600);
   SD_init();
   MCP42050_init();
   PWM_init();
   Temp_init();
   Remote_Control = Mqtt_setup();
 
}

void loop()
{
  //Remote_Control = remote_enable();
  //Serial.print("Remote_Enable: 1 mean True - Enable, 0 mean false- Disable ");
  //Serial.println(Remote_Enable);
   /*
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
  */
    //if ((millis() - mqtt_reconnect_time) > 10000) {
    
    if ((WiFi.status()!= 3) && (millis() - mqtt_reconnect_time) > 20000){
      
      Remote_Control = Mqtt_setup();
      Serial.println("Remote_Enable: 1 mean True-Enable, 0 mean false-Disable");
      mqtt_reconnect_time = millis();
    }
    
    else {
      
      if (!Remote_Control){
        manual_control_set = Manual_Control();
        MCP42050_Resistor_out(manual_control_set);
        PWM_out(manual_control_set);
        
        Serial.print(Remote_Control);
        Serial.println(" - : Remote Control is Disable");
        }
      
      else{
        mqtt_reci_raw_t = mqtt_read_raw_set_temperature(0);
        MCP42050_Resistor_out(mqtt_reci_raw_t);
        Serial.print(Remote_Control);
        Serial.println(" - : Remote Control is Enable");
      }

      //
      //MCP42050_Resistor_out(mqtt_reci_raw_t);
      send_raw_temperature();
      SD_log(TempSen_read_float(),mqtt_reci_raw_t,manual_control_set,Remote_Control);
  
      // MQTT send from pi one mess every 4 second
      delay(2000);
      }
      //mqtt_reconnect_time = millis();

  

}
