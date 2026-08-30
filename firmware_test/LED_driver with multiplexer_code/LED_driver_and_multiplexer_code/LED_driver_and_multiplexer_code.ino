//LED driver
#define GATE_DELAY 20

#define SER_IN 4
#define SRCK   48
#define RCK    47

//for the hall effects
#define ON 0
#define OFF 1
#define THRESHOLD 250

#define BOARDSIZE 8


//Multiplexer
#include <CD74HC4067.h>


//VScode main function

// Recall that all the #'s are referring to GPIO
#define S0 7
#define S1 8
#define S2 9
#define S3 10

#define A 0
#define B 1
#define C 2
#define D 3

const int a_common_pin = 5;
const int b_common_pin = 6;
const int c_common_pin = 11;
const int d_common_pin = 12;

// Initialize the multiplexer with control pins S0, S1, S2, S3
CD74HC4067 mux_A(S0, S1, S2, S3);
CD74HC4067 mux_B(S0, S1, S2, S3);
CD74HC4067 mux_C(S0, S1, S2, S3);
CD74HC4067 mux_D(S0, S1, S2, S3);

// Updated to accept an unsigned int (16 bits) so it handles both ICs
void b595_write(uint64_t data)
{

    // 1. Shift out the highest byte (Bits 39-32) -> Goes to IC5 (the furthest chip)
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 56) & 0xFF);
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 48) & 0xFF);
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 40) & 0xFF);

    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 32) & 0xFF);

    // 2. Shift out Bits 31-24 -> Goes to IC4
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 24) & 0xFF);

    // 3. Shift out Bits 23-16 -> Goes to IC3
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 16) & 0xFF);

    // 4. Shift out Bits 15-8  -> Goes to IC2
    shiftOut(SER_IN, SRCK, MSBFIRST, (data >> 8) & 0xFF);
    
    // 5. Shift out the lowest byte (Bits 7-0) -> Stays in IC1 (the closest chip)
    shiftOut(SER_IN, SRCK, MSBFIRST, data & 0xFF);
    
    // 6. Pulse the latch (RCK) AFTER all 40 bits are sent
    // This updates all 5 chips simultaneously
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, HIGH);
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, LOW);
}

//let's start with just one piece moving around
int boardStates[BOARDSIZE][BOARDSIZE];

//helper functions to map boardState array to loopThroughSensors()
int findBoardRow(int mux_number, int loop_number){
  if (mux_number == A || mux_number == C){
    if (loop_number == 15 || loop_number == 11 || loop_number == 0 || loop_number == 4){
      return 0;
    } else if (loop_number == 14 || loop_number == 10 || loop_number == 1 || loop_number == 5){
      return 1;
    } else if (loop_number == 13 || loop_number == 9 || loop_number == 2 || loop_number == 6){
      return 2;
    } else if (loop_number == 12 || loop_number == 8 || loop_number == 3 || loop_number == 7){
      return 3;
    }
  } else if (mux_number == B){
    if (loop_number == 15 || loop_number == 11 || loop_number == 0 || loop_number == 4){
      return 4;
    } else if (loop_number == 14 || loop_number == 10 || loop_number == 1 || loop_number == 5){
      return 5;
    } else if (loop_number == 13 || loop_number == 9 || loop_number == 2 || loop_number == 6){
      return 6;
    } else if (loop_number == 12 || loop_number == 8 || loop_number == 3 || loop_number == 7){
      return 7;
    }
  } else if (mux_number == D){
    if (loop_number == 15 || loop_number == 11 || loop_number == 0 || loop_number == 4){
      return 7;
    } else if (loop_number == 14 || loop_number == 10 || loop_number == 1 || loop_number == 5){
      return 6;
    } else if (loop_number == 13 || loop_number == 9 || loop_number == 2 || loop_number == 6){
      return 5;
    } else if (loop_number == 12 || loop_number == 8 || loop_number == 3 || loop_number == 7){
      return 4;
    }
  }

  return -1;

}

int findBoardCol(int mux_number, int loop_number){  
  if (mux_number == A || mux_number == B){
    if (loop_number >= 12 && loop_number <= 15){
      return 0;
    } else if (loop_number >= 8 && loop_number <= 11){
      return 1;
    } else if (loop_number >= 0 && loop_number <= 3){
      return 2;
    } else if (loop_number >= 4 && loop_number <= 7){
      return 3;
    }
  } else if (mux_number == C){
    if (loop_number >= 12 && loop_number <= 15){
      return 7;
    } else if (loop_number >= 8 && loop_number <= 11){
      return 6;
    } else if (loop_number >= 0 && loop_number <= 3){
      return 5;
    } else if (loop_number >= 4 && loop_number <= 7){
      return 4;
    }
  } else if (mux_number == D){
    if (loop_number >= 12 && loop_number <= 15){
      return 4;
    } else if (loop_number >= 8 && loop_number <= 11){
      return 5;
    } else if (loop_number >= 0 && loop_number <= 3){
      return 6;
    } else if (loop_number >= 4 && loop_number <= 7){
      return 7;
    }
  }

  return -1;
}

void changedSquare(int mux_number, int loop_number, int state){

  // for (int i = 0; i < BOARDSIZE; i++){
  //   for (int j = 0; j < BOARDSIZE; j++){
  //     Serial.print(boardStates[i][j]);
  //   }
  //   Serial.println(" ");
  // }


  
  if (state == OFF){
    Serial.print("AWAY ");
    Serial.print(mux_number);
    Serial.println(loop_number);
  } else if (state == ON){
    Serial.print("ON ");
    Serial.print(mux_number);
    Serial.println(loop_number);
  }
  uint64_t long_number = 0ULL + loop_number;
  b595_write(long_number << 8 * mux_number);

}

// uint64_t calculateallLED(int row, int col){
//   uint64_t sum = 0ull;
//   for (int i = 0; i < arraySize(board[row][col].availableMoves)){
//     sum += calculateoneLED(board[row][col].availableMoves[i]);
//   }

//   return sum;
// }

int findDrainNum(int row, int col){
 //for columns 2 and 0
  if (col % 2 == 0){
    return(row + 2);

  //for columns 3 and 1
  } else if (col % 2 == 1){
    if (row == 0){
      return 1;  
    } else if (row == 1){
      return 0;
    } else if (row == 2){
      return 7;
    } else if (row == 3){
      return 6;
    }
  }  
}

int findRegNum (int row, int col) {
    //LEFT is 1, RIGHT is 0
  if (col == 0 || col == 1){
    return 1;
  } else if (col == 2 || col == 3){
    return 0;
  }
}

int calculateoneLED(int availableMove){
  int row = availableMove / 10;
  int col = availableMove % 10;
  int drain_num = -1;
  int reg_num = -1;

  //based on row and column from availableMove, how do we access that square based on register?
  int numRegisters = (BOARDSIZE * BOARDSIZE) / 8;

  // THIS IS TO FIND THE DRAIN # ON THE REG
  drain_num = findDrainNum(row, col);

  //THIS IS TO FIND THE REG # (so far only for 4x4)
  reg_num = findRegNum(row, col);
  
  int addend = 1;
  //in these tpic registers w/ shiftout MSBFIRST, Drain0 fills first for original AND daisy-chained registers
  addend >> (reg_num * 8) + drain_num;

  return addend;
  

}

void setup()
{
  //boardStates setup
  for (int i = 0; i < BOARDSIZE; i++){
    for (int j = 0; j < BOARDSIZE; j++){
      boardStates[i][j] = OFF;
    }
  }
  // boardStates[0][3] = ON;
  
  Serial.begin(115200);    
  pinMode(SER_IN, OUTPUT);
  pinMode(SRCK, OUTPUT);
  pinMode(RCK, OUTPUT);

  Serial.println("LED driver initialized");

  // Set control pins as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set the common signal pin as an input once
  pinMode(a_common_pin, INPUT);
  pinMode(b_common_pin, INPUT);

  Serial.println("Multiplexer code initialized");

}

void loop()
{    
  for (int i = 0; i < 16; i++){
    int muxA_state;

    mux_A.channel(i);

    analogRead(a_common_pin);
    
    muxA_state = analogRead(a_common_pin);
    if (muxA_state > THRESHOLD){
      muxA_state = OFF;
    } else if (muxA_state <= THRESHOLD){
      muxA_state = ON;
    }

    if (muxA_state != boardStates[findBoardRow(A, i)][findBoardCol(A, i)]){
      changedSquare(A, i, muxA_state);
      boardStates[findBoardRow(A, i)][findBoardCol(A, i)] = muxA_state;
    }

    int muxB_state;

    mux_B.channel(i);

    analogRead(b_common_pin);
    
    muxB_state = analogRead(b_common_pin);
    if (muxB_state > THRESHOLD){
      muxB_state = OFF;
    } else if (muxB_state <= THRESHOLD){
      muxB_state = ON;
    }

    if (muxB_state != boardStates[findBoardRow(B, i)][findBoardCol(B, i)]){
      changedSquare(B, i, muxB_state);
      boardStates[findBoardRow(B, i)][findBoardCol(B, i)] = muxB_state;
    }


    int muxC_state;

    mux_C.channel(i);

    analogRead(c_common_pin);
    
    muxC_state = analogRead(c_common_pin);
    if (muxC_state > THRESHOLD){
      muxC_state = OFF;
    } else if (muxC_state <= THRESHOLD){
      muxC_state = ON;
    }

    if (muxC_state != boardStates[findBoardRow(C, i)][findBoardCol(C, i)]){
      changedSquare(C, i, muxC_state);
      boardStates[findBoardRow(C, i)][findBoardCol(C, i)] = muxC_state;
    }


    int muxD_state;

    mux_D.channel(i);

    analogRead(d_common_pin);
    
    muxD_state = analogRead(d_common_pin);
    if (muxD_state > THRESHOLD){
      muxD_state = OFF;
    } else if (muxD_state <= THRESHOLD){
      muxD_state = ON;
    }

    if (muxD_state != boardStates[findBoardRow(D, i)][findBoardCol(D, i)]){
      changedSquare(D, i, muxD_state);
      boardStates[findBoardRow(D, i)][findBoardCol(D, i)] = muxD_state;
    }
      
      

    
  }
}