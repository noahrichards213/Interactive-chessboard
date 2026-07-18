#include <CD74HC4067.h>
#include "MCP23S17.h"
#include <SPI.h>

#define CS_pin1 47
#define CS_pin2 48
#define CLK_pin 12
#define MISO_pin 13
#define MOSI_pin 11

#define ON 0
#define OFF 1
#define THRESHOLD 500

//recall that all the #'s are are referring to GPIO
#define S0 38
#define S1 39
#define S2 40
#define S3 41

//creating SPI object
SPIClass *fspi_1 = nullptr;

//creating MCP objects
MCP23S17 *MCP_1 = nullptr;
MCP23S17 *MCP_2 = nullptr;

//creating mux object
CD74HC4067 mux_A(255, 255, 255, 255);

const int g_common_pin = 3;
int channel_num = 1;

//chessboard specific variables
int piece_state[2];

//nitty gritty of the IC implementation
//as well as implementation of piece_state
void setup()
{
  Serial.begin(115200);

  fspi_1 = new SPIClass(FSPI);
  fspi_1->begin(CLK_pin, MISO_pin, MOSI_pin);

  MCP_1 = new MCP23S17(CS_pin1, fspi_1); 
  MCP_2 = new MCP23S17(CS_pin2, fspi_1);

  if (MCP_1->begin() == false) {
    Serial.println("Library begin1() failed.");
  } else {
      MCP_1->pinMode8(0, 0x00); 
      MCP_1->pinMode8(1, 0xFF); 
    Serial.println("Configuration 1 complete.");

  }

  if (MCP_2->begin() == false) {
    Serial.println("Library begin2() failed.");
  } else {
      MCP_2->pinMode8(0, 0x00); 
      MCP_2->pinMode8(1, 0xFF); 
    Serial.println("Configuration 2 complete.");

  }
  Serial.begin(115200);
  Serial.println("The ESP can print");
  // pinMode(S0, OUTPUT);
  pinMode(g_common_pin, INPUT);

  //initializing piece states 
  piece_state[0] = OFF;
  piece_state[1] = OFF;
}

void loop() {
  int sensorValue;
  int sensorDigital;



  MCP_1->write1(7, HIGH);
  MCP_1->write1(6, HIGH);
  MCP_2->write1(6, HIGH);
  MCP_2->write1(5, HIGH);

  // looping through both channels
  // if (channel_num == 1){
  //   channel_num = 0;
  //   // digitalWrite(S0, 0);
  // } else {
  //   channel_num = 1;
  //   // digitalWrite(S0, 1);
  // }
  //mux_A.channel(channel_num);

  if (channel_num == 0){
    MCP_1->write1(0, LOW);
  } else if (channel_num == 1){
    // MCP_1->write1(5, HIGH);
    MCP_1->write1(0, HIGH);

    MCP_1->write1(2, HIGH);
    // MCP_1->write1(7, HIGH);
  }
  
  sensorValue = analogRead(g_common_pin);

  if (sensorValue > THRESHOLD){
    sensorDigital = OFF;
  } else {
    sensorDigital = ON;
  }

  //seeing if it matches original
  //make this into a function, for detecting placing and removing
  if (piece_state[channel_num] != sensorDigital){
    if (sensorDigital == ON){
      Serial.println("We are placing a piece");
    } else if (sensorDigital == OFF){
      
      Serial.println("We are removing a piece");

      // waiting for piece to be placed down (not considering capture)
      // also not considering which moves are legal
        // int i = 0; 
        // // This executes the block once before checking the condition
        // do {
        //   for (i = 0; i < 2; i++) {
        //     if (i != channel_num) {
        //       if (i == 0){
        //           MCP_1->write1(0, LOW);
        //         } else if (i == 1){
        //           MCP_1->write1(0, HIGH);
        //         }
        //       sensorValue = analogRead(g_common_pin);

        //       // Check if we placed something on another piece
        //       if (piece_state[i] == OFF && sensorValue < THRESHOLD) {
        //         Serial.print("We are placing something after removing");
        //         piece_state[i] = ON;

        // //         delay(10000);
        // //         // Perform correction
        // //         // mux_A.channel(channel_num);
        // //         // sensorValue = analogRead(g_common_pin);

        // //         //finish this later, check the original square, for now not testing (july 3)
                
        //       }
        //     }
        //   }

        // } while (!(piece_state[i] == OFF && sensorValue < THRESHOLD));

      }

    piece_state[channel_num] = sensorDigital;
    }
  }


  // //printing to console
  // Serial.print("Piece ");
  // Serial.print(channel_num);
  // Serial.print(" ");

  // if (sensorValue > THRESHOLD){
  //   Serial.println("The piece is off");
  // } else {
  //   Serial.println("The piece is on");
  // }

  // Serial.print("The sensor value is: ");
  // Serial.println(sensorValue);


  // MCP_1->write1(7, HIGH);
  // MCP_1->write1(6, HIGH);

  // MCP_2->write1(6, HIGH);
  // MCP_2->write1(5, HIGH);

  // delay(1000);

  // MCP_1->write1(7, LOW);
  // MCP_1->write1(6, LOW);

  // MCP_2->write1(6, LOW);
  // MCP_2->write1(5, LOW);

  // delay(1000);
  // Serial.print("This is the pin value from channel: ");
  // Serial.print(channel_num);
  // Serial.println(sensorValue);
