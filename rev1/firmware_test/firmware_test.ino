#include "MCP23S17.h"
#include <SPI.h>

#define CS_pin1 47
#define CLK_pin 12
#define MISO_pin 13
#define MOSI_pin 11

// Define SPI and Device pointers
SPIClass *fspi = nullptr;
MCP23S17 *MCP_1 = nullptr;

void setup() {
  Serial.begin(115200);

  // 1. Initialize custom SPI bus
  fspi = new SPIClass(FSPI);
  fspi->begin(CLK_pin, MISO_pin, MOSI_pin, CS_pin1);


  // 2. Initialize the MCP object
  // Note: Ensure your library constructor allows passing an SPIClass reference.
  // If your library only takes (CS, MOSI, MISO, CLK), it likely uses the default 'SPI' 
  // object internally and cannot use your custom fspi bus.
  MCP_1 = new MCP23S17(CS_pin1, fspi); 

  // 4. Library Initialization
  if (MCP_1->begin(true) == false) {
    Serial.println("Library begin() failed.");
  } else {
    // 5. Setup configuration
    MCP_1->pinMode8(0, 0x00); // Port A as Output
    // MCP_1->pinMode8(1, 0xFF); // Port B as Input
    Serial.println("Configuration complete.");

  // MCP_1->pinMode1(11, OUTPUT);
  // MCP_1->write1(11, HIGH);
  // MCP_1->pinMode1(8, INPUT);

  // MCP_1->pinMode1(8, INPUT_PULLUP);



  }
}

void loop()
{
  MCP_1->write1(7, HIGH);
  delay(1300);
  MCP_1->write1(7, LOW);
  delay(1300);

  int value1 = MCP_1->read1(8);
  Serial.print("The value1 is ");
  Serial.println(value1);

  int value2 = MCP_1->read1(9);
  Serial.print("The value2 is ");
  Serial.println(value2);


  // MCP_1->write1(7, LOW);
  // delay(5000);
  // Serial.println("its low");
  //testing all the LED's
  // for (int square = 0; square < 16; square++){
  //   Serial.print("We are on square: ");
  //   Serial.print(square);
  //   write_MCP(square, HIGH, MCP_1, MCP_2);
  //   delay(100);
  //   write_MCP(square, LOW, MCP_1, MCP_2);
  // }

  //testing hall effects
  // while (1){
  //   for (int square = 0; square < 16; square++){
  //     if (read_MCP(square) == HIGH){
  //       write_MCP(square, HIGH);
  //     } else if (read_MCP(square) == LOW){
  //       write_MCP(square, LOW);
  //     }

  //   }
  // }


}

//index from 0 to 7 is portA
//index from 8 - 15 is portB

//based off where the hall effects are
// int read_MCP(uint8_t index, MCP23S17* MCP_1, MCP23S17* MCP_2){
// if (index < 8){
//  return MCP_1->read1(index + 8);
// } else if (index >= 8){
//  return MCP_2->read1(index);
// }
// }

// //based off where the leds are
// void write_MCP(uint8_t index, uint8_t value, MCP23S17* MCP_1, MCP23S17* MCP_2){
// if (index < 8){
//   MCP_1->write1(index, value);
// } else if (index >= 8){
//   MCP_2->write1(index - 8, value);
// }
// }

//  -- END OF FILE --