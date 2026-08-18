#define BOARDSIZE 8

//LED driver
#define GATE_DELAY 20

#define SER_IN 4
#define SRCK   48
#define RCK    47

//Hall effects
#define ON 0
#define OFF 1
#define MOVING 2
#define THRESHOLD 250

//Multiplexers
#include <CD74HC4067.h>

#define S0 7
#define S1 8
#define S2 9
#define S3 10

const int a_common_pin = 5;
const int b_common_pin = 6;
const int c_common_pin = 11;
const int d_common_pin = 12;

#define A 0
#define B 1
#define C 2
#define D 3

//Main function header inclusions
#include <math.h>
#include <stdbool.h>

// arraysize
#include "arraysize.h"

// check one dir
#include "checkonedir.h"

// availablemoves
#include "availablemoves.h"

// constants and piece data structure
#include "constantsandstructs.h"

// functions for all legal moves
#include "availablemoves.h"

// setup
#include "setup.h"

// make move
#include "makemove.h"

// printing things to console
#include "printing.h"

// checking functions
#include "checking.h"

// include legal move and capture functions
#include "legalmoveandcapture.h"

// include en passant mechanics
#include "enpassant.h"

// function to remove illegal moves because it would leave player in check
#include "removecheck.h"

//END OF INCLUDING HEADERS

// Initialize the multiplexer with control pins S0, S1, S2, S3
CD74HC4067 mux_A(S0, S1, S2, S3);
CD74HC4067 mux_B(S0, S1, S2, S3);
CD74HC4067 mux_C(S0, S1, S2, S3);
CD74HC4067 mux_D(S0, S1, S2, S3);

//setup for board
int colour = WHITE;
bool haveMove = true;


void write_leds(unsigned int data)
{
    //writing to all 64 registers
    for (int reg_num = 7; reg_num >= 0; reg_num--){
      shiftOut(SER_IN, SRCK, MSBFIRST, ((data >> (8 * reg_num)) & 0xFF));
    }

    //pulsing registers to LEDS
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, HIGH);
    delayMicroseconds(GATE_DELAY);
    digitalWrite(RCK, LOW);
}

//include Piece board
Piece board[BOARDSIZE][BOARDSIZE];

//let's start with just one piece moving around
int boardStates[BOARDSIZE][BOARDSIZE];

//NOTE FOR THESE FUNCTIONS, MUX D IS UPSIDE DOWN ON PCB, THAT IS REASON FOR ASYMMETRY

//these functions give the index (i.e from 0 to BOARDSIZE - 1 which is 7)
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
}


//so far doesn't include special moves (en passant, castling, promotion)

//also doesn't include possibility for making illegal moves
void changedSquare(int mux_number, int loop_number, int state){

  int boardRow = findBoardRow(mux_number, loop_number);
  int boardCol = findBoardCol(mux_number, loop_number);

  // if we took a piece off, means we are prepping to make a make a move
  if (state == OFF){

    //if piece has no legal moves
    boardStates[boardRow][boardCol] = MOVING;
    write_leds(calculateallLED(boardRow, boardCol));

  // if we put a piece on, means we are completing a move
  } else if (state == ON){
    boardStates[boardRow][boardCol] = ON;

    //updating board w/ move
    for (int row = 0; row < BOARDSIZE - 1; row++){
      for (int col = 0; col < BOARDSIZE - 1; col++){
        if (boardStates[row][col] == MOVING){
          board[boardRow][boardCol] = board[row][col];
          board[row][col] = empty;
          boardStates[row][col] = OFF;
        }
      }
    }
    //removes all the LEDS
    write_leds(0);

  }

}

uint64_t calculateallLED(int row, int col){
  uint64_t sum = 0ull;
  for (int i = 0; i < arraySize(board[row][col].availableMoves); i++){
    sum += calculateoneLED(board[row][col].availableMoves[i]);
  }

  return sum;
}

int calculateoneLED(int availableMove){
  int row = availableMove / 10;
  int col = availableMove % 10;
  int drain_num = -1;
  int reg_num = -1;

  //based on row and column from availableMove, how do we access that square based on register?

  // THIS IS TO FIND THE DRAIN # (drain #'s are 0 to 7)
  drain_num = BOARDSIZE - 1  - col;

  //THIS IS TO FIND THE REG # (reg's are 0 to 7)
  reg_num = BOARDSIZE - 1 - row;
  
  int addend = 1;
  //in these tpic registers w/ shiftout MSBFIRST, Drain0 fills first for original AND daisy-chained registers
  addend >> (reg_num * 8) + drain_num;

  return addend;

}

int checkMux(int mux_number){

}

void checkmateResult(){
  Serial.println("It's checkmate");
}

void stalemateResult(){
  Serial.println("It's stalemate");
}

void setup()
{
  //setup from vscode main
  setupfunction();

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
  pinMode(c_common_pin, INPUT);
  pinMode(d_common_pin, INPUT);

  Serial.println("Multiplexer code initialized");

}

void loop()
{    
  // print initial board and legal moves
  printBoard(board);
  printLegalMoves();

  // if there is a legal move to be made
  while (haveMove) {
    
  for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++){
    int muxA_state;
    int muxB_state;
    int muxC_state;
    int muxD_state;

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
    }


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
    } 

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
    } 

    mux_D.channel(i);
    muxD_state = analogRead(d_common_pin);

    if (muxD_state > THRESHOLD){
      muxD_state = OFF;
    } else {
      muxD_state = ON;
    }

    if (muxD_state != boardStates[findBoardRow(D, i)][findBoardCol(D, i)]){
      changedSquare(D, i, muxD_state);
    }

  }

  // first check that we are not 'moving'
  bool isMoving = false;
  for (int row = 0; row < BOARDSIZE; row++){
      for (int col = 0; col < BOARDSIZE; col++){
        if (boardStates[row][col] == MOVING){
          isMoving = true;
        }
      }
  }

  if (isMoving == false){
    // change legal moves
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (board[i][j].type != '_') {
          changeAvailableMoves(&board[i][j], colour);
        }
      }
    }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // first, we need to check every single move and see if it leads to
      // check (it then would be unallowed)
      int size = arraySize(board[i][j].availableMoves);
      for (int k = 0; k < size; k++) {
        if (board[i][j].type != '_') {
          if (removeCheck(board[i][j], k, board[i][j].availableMoves[k]) ==
              true) {
            if (board[i][j].type == 'n') {
              for (int p = 0; p < arraySize(board[i][j].availableMoves);
                    p++) {
              }
            }
            board[i][j].availableMoves[k] = ALLOWSCHECK;
          }
        }
      }
    }
  }
  printLegalMoves();
  printBoard(board);

  if (colour == WHITE) {
    colour = BLACK;
  } else if (colour == BLACK) {
    colour = WHITE;
  }
  haveMove = checkLegalMoves(colour);

  if (inCheck(colour)) {
    checkmateResult();
  } else {
    stalemateResult();
  }

  }


  }
  

}
