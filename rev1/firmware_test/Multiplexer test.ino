#include <CD74HC4067.h>

//recall that all the #'s are are referring to GPIO
#define S0 38
#define S1 39
#define S2 40
#define S3 41

//this will be connected to a hall effect
#define channel_num 0

const int g_common_pin = 3;

CD74HC4067 mux_A(S0, S1, S2, S3);


void setup()
{
  Serial.begin(115200);
  Serial.println("The ESP can print");
}

void loop() {
  pinMode(g_common_pin, INPUT);
  int sensorValue;
  mux_A.channel(channel_num);
  sensorValue = analogRead(g_common_pin);

  Serial.print("This is the pin value from channel ");
  Serial.println(channel_num);
  Serial.print(sensorValue);
  Serial.
}