/*-------------------------------------------
  Heat Pump Temperature Manipulation
  Datalog Funtion file 
  Trung Nguyen 2021
  version 1.00 - 18/11/2021
  
--------------------------------------------*/ 

#ifndef Datalog_CPP
#define Datalog_CPP
#include <SPI.h>
#include "Datalog.h"
#include <SD.h>

/*-------------------------------------------
--------------------------------------------*/ 

// A simple data logger for the Arduino analog pins

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card

uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

const int chipSelect = 9;

RTC_DS3231 rtc;

// the logging file
File logfile;

//const int chipSelect = 9;

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);

  while(1);
}

void SD_init(void)
{
  Serial.begin(9600);
  Serial.println();
  
  // use debugging LEDs
  //pinMode(redLEDpin, OUTPUT);
  //pinMode(greenLEDpin, OUTPUT);
  
  #if WAIT_TO_START
    Serial.println("Type any character to start");
    //while (!Serial.available());
  #endif //WAIT_TO_START

  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(9, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(9)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");
  
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 1000; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    error("couldnt create file");
  }
  
  Serial.print("Logging to: ");
  Serial.println(filename);

  // connect to RTC
  Wire.begin();  
  if (!rtc.begin()) {
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL
  }
  

  logfile.println("millis,stamp,datetime,Real_NTC_Sensor,Raw_MQTT_Set_T,Manual_Control,Remote_Enable");    
#if ECHO_TO_SERIAL
  Serial.println("millis,stamp,datetime,Real_NTC_Sensor,Raw_MQTT_Set_T,Manual_Control,Remote_Enable");
#endif //ECHO_TO_SERIAL
 
  // If you want to set the aref to something other than 5v
  //analogReference(EXTERNAL);
}


void SD_log(float Raw_NTC, float Raw_MQTT_Set_T,float Manual_Set_T, bool Remote)
{
  DateTime now;

  // delay for the amount of time we want between readings
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));
  
  //digitalWrite(greenLEDpin, HIGH);
  
  // log milliseconds since starting
  uint32_t m = millis();
  logfile.print(m);           // milliseconds since start
  logfile.print(", ");    

  // fetch the time
  now = rtc.now();
  // log time
  logfile.print(now.unixtime()); // seconds since 1/1/1970
  logfile.print(", ");
  logfile.print('"');
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print('"');

  // log the raw value of NTC sensor
  logfile.print(", ");    
  logfile.print(Raw_NTC);

  //log the MQTT set Temperature
  logfile.print(", ");    
  logfile.print(Raw_MQTT_Set_T);

  //log the Manual set Temperature
  logfile.print(", ");    
  logfile.print(Manual_Set_T);

  //log the Remote Control Enable
  logfile.print(", ");    
  logfile.print(Remote);
  
  

  
#if ECHO_TO_SERIAL
  //Serial.print(", ");    
  //Serial.print(Raw_NTC);
#endif //ECHO_TO_SERIAL

  logfile.println();
#if ECHO_TO_SERIAL
  Serial.println();
#endif // ECHO_TO_SERIAL

  //digitalWrite(greenLEDpin, LOW);

  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  
  // blink LED to show we are syncing data to the card & updating FAT!
  //digitalWrite(redLEDpin, HIGH);
  logfile.flush();
  //digitalWrite(redLEDpin, LOW);
}

#endif //__ESP32_ADC_CPP
