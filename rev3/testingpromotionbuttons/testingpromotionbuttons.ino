// Define the range of GPIO pins to poll
const int startPin = 15;
const int endPin = 18;

void setup() {
  // Start serial communication at 9600 baud
  Serial.begin(115200);
  
  // Configure pins 15 to 18 as inputs with internal pull-ups
  // (Change to INPUT if you are using external pull-down resistors)
  for (int pin = startPin; pin <= endPin; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }
}

void loop() {
  // Loop through each pin and check its state
  for (int pin = startPin; pin <= endPin; pin++) {
    int pinState = digitalRead(pin);
    
    // If the pin is pulled LOW, print an alert
    if (pinState == LOW) {
      Serial.print("Alert: Pin ");
      Serial.print(pin);
      Serial.println(" is LOW!");
    }
  }
  
  // Short delay to avoid spamming the Serial Monitor
  delay(100);
}