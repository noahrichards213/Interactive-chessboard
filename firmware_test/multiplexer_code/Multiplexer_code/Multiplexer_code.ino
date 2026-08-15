#include <CD74HC4067.h>

// Recall that all the #'s are referring to GPIO
#define S0 7
#define S1 8
#define S2 9
#define S3 10

const int a_common_pin = 5;
const int b_common_pin = 6;

// Initialize the multiplexer with control pins S0, S1, S2, S3
CD74HC4067 mux_A(S0, S1, S2, S3);
CD74HC4067 mux_B(S0, S1, S2, S3);


void setup() {
  Serial.begin(115200);
  Serial.println("Multiplexer code initialized");

  // Set control pins as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set the common signal pin as an input once
  pinMode(a_common_pin, INPUT);
  pinMode(b_common_pin, INPUT);
}

void loop() {
  int sensorValueA;
  int sensorValueB;

  // Loop through channels 0 to 15
  for (int i = 0; i <= 15; i++) {
    // --- Mux A Processing ---
    if (i <=3 || i >= 12) {
        mux_A.channel(i);   
    
    delayMicroseconds(200); // Short delay for signal stabilization

    sensorValueA = analogRead(a_common_pin);

    // Only print the line if sensorValueA < 50
    if (sensorValueA < 50) {
      Serial.print("Mux A - Piece ");
      Serial.print(i);
      Serial.println(": The piece is on");

      Serial.print("The sensor value is: ");
      Serial.println(sensorValueA);
    }
    }
    // --- Mux B Processing ---
    if (i <=3 || i >= 12) {
      mux_B.channel(i);   
    
    delayMicroseconds(200); // Short delay for signal stabilization

    sensorValueB = analogRead(b_common_pin);

    // Only print the line if sensorValueB < 50
    if (sensorValueB < 50) {
      Serial.print("Mux B - Piece ");
      Serial.print(i);
      Serial.println(": The piece is on");

      Serial.print("The sensor value is: ");
      Serial.println(sensorValueB);
    }
    }
  }
  
  delay(500); // Slow down loop repetition so the Serial Monitor isn't flooded
}