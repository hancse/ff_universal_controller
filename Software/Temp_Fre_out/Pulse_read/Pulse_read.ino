#include <movingAvg.h>          // https://github.com/JChristensen/movingAvg

int pin = 2;
float duration_h;
float duration_l;
float period;
float fre;
float DC;
float Ta;

movingAvg mySensor(8);    // use 10 data points for the moving average
void setup() {
  Serial.begin(9600);
  mySensor.begin();
  pinMode(pin, INPUT);
}

void loop() {
  //Serial.println("begin");
  duration_h = pulseIn(pin, HIGH);
  //Serial.println(duration_h);
  duration_l = pulseIn(pin, LOW);
  //Serial.println(duration_l);
  period = duration_h + duration_l;
  //Serial.println(period);
  //fre = 1000000/period;
  //Serial.println("frequency:");
  //Serial.print(fre);
  DC = duration_h/period;
  //Serial.println(DC);
  delay(1000);

  int sensorMovingAvg = mySensor.reading(DC*100);
  //sensorMovingAvg = mySensor.getAvg();
  Serial.println(sensorMovingAvg);
  Ta = sensorMovingAvg*212.77/100 - 68.085;
  Serial.println("Temperature:");
  Serial.println(Ta);
}
