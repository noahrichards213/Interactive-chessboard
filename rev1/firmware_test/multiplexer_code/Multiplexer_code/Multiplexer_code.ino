#include <CD74HC4067.h>

//recall that all the #'s are are referring to GPIO
#define S0 38
#define S1 39
#define S2 40
#define S3 41

const int g_common_pin = 3;
int channel_num = 0;

CD74HC4067 mux_A(S0, -1, -1, -1);


void setup()
{
  Serial.begin(115200);
  Serial.println("The ESP can print");
  pinMode(S0, OUTPUT);
}

void loop() {
  pinMode(g_common_pin, INPUT);
  int sensorValue;

  if (channel_num == 1){
    channel_num = 0;
    digitalWrite(S0, 0);
  } else {
    channel_num = 1;
    digitalWrite(S0, 1);
  }


  mux_A.channel(channel_num);
  sensorValue = analogRead(g_common_pin);

  Serial.print("Piece ");
  Serial.print(channel_num);
  Serial.print(" ");
  if (sensorValue > 500){
    Serial.println("The piece is off");
  } else {
    Serial.println("The piece is on");
  }

  Serial.print("The sensor value is: ");
  Serial.println(sensorValue);


  // Serial.print("This is the pin value from channel: ");
  // Serial.print(channel_num);
  // Serial.println(sensorValue);
}