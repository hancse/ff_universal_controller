/*-------------------------------------------
  Wifi connection
  IOT33
  Trung Nguyen 2021
  version 1.00 - 18/11/2021

  --------------------------------------------*/

#define SECRET_SSID "Tenda2"
#define SECRET_PASS "1234567890"
#define SECRET_MQTT_USER "public"
#define SECRET_MQTT_PASS "public"


bool Mqtt_setup();

float mqtt_read_raw_set_temperature(int i);

void send_raw_temperature();
