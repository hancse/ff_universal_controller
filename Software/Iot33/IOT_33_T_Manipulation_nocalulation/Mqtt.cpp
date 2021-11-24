/*
  ArduinoMqttClient - WiFi Simple Receive

  This example connects to a MQTT broker and subscribes to a single topic.
  When a message is received it prints the message to the serial monitor.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev.2 board

  This example code is in the public domain.
*/

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif

#include "Temperature.h"
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.hivemq.com";
int        port     = 1883;
const char p_topic[]  = "FZ/IOT33/Temperature";  
const char s_topic[]  = "FZ/Raw_Sensor_values";  // subcribe topic


void Mqtt_setup() {
 
 //Initialize serial and wait for port to open:
/*
 *   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 */

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(s_topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(s_topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(s_topic);
  Serial.println();
}

// i: counter value which help an array to received all character from mqtt messages 

float read_raw_temperature(int i){

  // input parameters
  float adc_raw;
  char adc_char;
  int messageSize = mqttClient.parseMessage();
  char message[messageSize];
  
  if (messageSize) {
    // we received a message, print out the topic and contents
    //Serial.print("Received a message with topic '");
    //Serial.print(mqttClient.messageTopic());
    //Serial.print("', length ");
    //Serial.print(messageSize);
    //Serial.println(" bytes:");
    
    // use the Stream interface to print the contents
    
     while (mqttClient.available()) {
      
        adc_char = mqttClient.read();
        message[i] = adc_char;
        i = i+1;
        
        }
    adc_raw = atof(message);
  }

  return adc_raw;
}

/*
 Publish messages
*/

const long interval = 1000;
unsigned long previousMillis = 0;

void send_raw_temperature(){

  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //Serial.print("Sending message to topic: ");
    //Serial.println(p_topic);
    Serial.print("Raw_value: ");
    Serial.println(tempSen_read_float());

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(p_topic);
    //mqttClient.print("hello ");
    mqttClient.print(tempSen_read_float());
    mqttClient.endMessage();

    Serial.println();

    //count++;
  }
  
}
