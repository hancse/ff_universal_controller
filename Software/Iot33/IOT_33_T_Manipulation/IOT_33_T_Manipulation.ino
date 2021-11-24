// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include "Arduino.h"
#include "arduino_secrets.h"
#include "MCP_42050.h"
#include "Temperature.h"

//int saadcBuffer[SAMPLES];  // variable to store the value coming from the sensor

// Define Analog Pin for thermistor input


void setup()
{
 Serial.begin(9600);
 Mqtt_setup();
 MCP42050_init();
 temp_init();
 Serial.begin(9600);
 
}

void loop()
{
//float x = read_raw_temperature(0);
  //tempSen_read_float();
  //float x = tempSen_read_float();
  //Serial.println("Raw value: ");
  //Serial.print(x);
  MCP42050_Resistor_out(read_raw_temperature(0));
  send_raw_temperature(0);
  // MQTT send from pi one mess every 4 second
  delay(2000);
}
