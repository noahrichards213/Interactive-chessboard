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
#define CAPTURING 3
#define CASTLING 4
#define ENPASSANT 5
#define NORMAL 6
#define NOTLEGAL 7
#define PROMOTION 8

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
bool madeMove = false;


void write_leds(uint64_t data)
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

void findMuxAndLoop(int boardRow, int boardCol, int *mux_number, int *loop_number) {
    if (boardRow < 0 || boardRow > 7 || boardCol < 0 || boardCol > 7) {
        *mux_number = -1;
        *loop_number = -1;
        return;
    }

    int local_row = boardRow % 4;
    int local_col = boardCol % 4;

    if (boardRow < 4 && boardCol < 4) {
        *mux_number = A;
        if (local_col == 0)      *loop_number = 15 - local_row;
        else if (local_col == 1) *loop_number = 11 - local_row;
        else if (local_col == 2) *loop_number = local_row;
        else if (local_col == 3) *loop_number = 4 + local_row;
    } else if (boardRow >= 4 && boardCol < 4) {
        *mux_number = B;
        if (local_col == 0)      *loop_number = 15 - local_row;
        else if (local_col == 1) *loop_number = 11 - local_row;
        else if (local_col == 2) *loop_number = local_row;
        else if (local_col == 3) *loop_number = 4 + local_row;
    } else if (boardRow < 4 && boardCol >= 4) {
        *mux_number = C;
        if (local_col == 0)      *loop_number = 4 + local_row;
        else if (local_col == 1) *loop_number = local_row;
        else if (local_col == 2) *loop_number = 11 - local_row;
        else if (local_col == 3) *loop_number = 15 - local_row;
    } else {
        *mux_number = D;
        if (local_col == 0)      *loop_number = 12 + local_row;
        else if (local_col == 1) *loop_number = 8 + local_row;
        else if (local_col == 2) *loop_number = 3 - local_row;
        else if (local_col == 3) *loop_number = 7 - local_row;
    }
}

//so far doesn't include special moves (en passant, castling, promotion)

//also doesn't include possibility for making illegal moves
void changedSquare(int mux_number, int loop_number, int state){

  // uint64_t long_number = 0ULL + loop_number;
  // write_leds(long_number << 8 * mux_number);;

  int boardRow = findBoardRow(mux_number, loop_number);
  int boardCol = findBoardCol(mux_number, loop_number);

  // if we took a piece off, means we are prepping to make a make a move
  if (state == OFF){
  
    uint64_t data = 0ull;
    //accounting for captures
    if (board[boardRow][boardCol].colour != colour){
      boardStates[boardRow][boardCol] = CAPTURING;
    } else {
      for (int i = 0; i < arraySize(board[boardRow][boardCol].availableMoves); i++){

        int realAvailableMove = board[boardRow][boardCol].availableMoves[i];

        if (realAvailableMove >= 100 && realAvailableMove <= 10000) {
            realAvailableMove /= 100;
        } else if (realAvailableMove > 10000) {
            realAvailableMove /= 1000;
        }

        if (realAvailableMove >= 0){
          data += calculateoneLED(realAvailableMove);
        }
      }
      write_leds(data);
      boardStates[boardRow][boardCol] = MOVING;

    }
  

  // if we put a piece on, means we are completing a move
   } else if (state == ON){

    int moveMade = boardRow * 10 + boardCol;
    Serial.print("Move made: ");
    Serial.println(moveMade);
    Piece *pieceMoved = nullptr;

    for (int row = 0; row < BOARDSIZE; row++){
      for (int col = 0; col < BOARDSIZE; col++){
        if (boardStates[row][col] == MOVING){
          pieceMoved = &board[row][col];
        }
      }
    }

    int moveType = isLegalMove(moveMade, pieceMoved);
    if (moveType != NOTLEGAL){
      Serial.println("We are doing a legal move");
      write_leds(0);
      if (moveType == CASTLING){
        Serial.println("We are doing a castling move");
        int mux, loop;
        int originalRookCol;
        int originalRookRow;
        int finalRookCol;
        int finalRookRow;

        //castling queenside
        if (boardCol == 2){
          if (colour == WHITE){
            originalRookCol = 0;
            originalRookRow = 7;
            finalRookCol = 3;
            finalRookRow = 7;
          } else if (colour == BLACK){
              originalRookCol = 0;
              originalRookRow = 0;
              finalRookCol = 3;
              finalRookRow = 0;
          }
        //castling kingside
        } else if (boardCol == 6){
          if (colour == WHITE){
              Serial.println("We are getting here");
              originalRookCol = 7;
              originalRookRow = 7;
              finalRookCol = 5;
              finalRookRow = 7;
          } else if (colour == BLACK){
              originalRookCol = 7;
              originalRookRow = 0;
              finalRookCol = 5;
              finalRookRow = 0;
          }
        }

        findMuxAndLoop(originalRookRow, originalRookCol, &mux, &loop);

        int raw_reading = 0; // Initialize low so it enters the loop

        while (raw_reading <= THRESHOLD) {
            // Select the correct mux and read the pin dynamically
            if (mux == A) {
                mux_A.channel(loop);
                analogRead(a_common_pin); // settling read
                raw_reading = analogRead(a_common_pin);
            } 
            else if (mux == B) {
                mux_B.channel(loop);
                analogRead(b_common_pin);
                raw_reading = analogRead(b_common_pin);
            } 
            else if (mux == C) {
                mux_C.channel(loop);
                analogRead(c_common_pin);
                raw_reading = analogRead(c_common_pin);
            } 
            else if (mux == D) {
                Serial.println("We are in mux number D");
                mux_D.channel(loop);
                analogRead(d_common_pin);
                raw_reading = analogRead(d_common_pin);
            }
        }
        Serial.println("We are out of it");
        write_leds(calculateoneLED(finalRookRow * 10 + finalRookCol));
        int raw_reading_2 = 1000;

        while (raw_reading_2 > THRESHOLD) {
            if (mux == A) {
                findMuxAndLoop(0, 3, &mux, &loop);
                mux_A.channel(loop);
                analogRead(a_common_pin); // settling read
                raw_reading_2 = analogRead(a_common_pin);
            } 
            else if (mux == B) {
                findMuxAndLoop(7, 3, &mux, &loop);
                mux_B.channel(loop);
                analogRead(b_common_pin); // settling read
                raw_reading_2 = analogRead(b_common_pin);
            } 
            else if (mux == C) {
                findMuxAndLoop(0, 5, &mux, &loop);
                mux_C.channel(loop);
                analogRead(c_common_pin); // settling read
                raw_reading_2 = analogRead(c_common_pin);
            } 
            else if (mux == D) {
                Serial.println("We are here in D again");
                findMuxAndLoop(7, 5, &mux, &loop);
                mux_D.channel(loop);
                analogRead(d_common_pin); // settling read
                raw_reading_2 = analogRead(d_common_pin);
            }
        }

        board[finalRookRow][finalRookCol] = board[originalRookRow][originalRookCol];
        board[originalRookRow][originalRookCol] = empty;
        boardStates[finalRookRow][finalRookCol] = ON;
        boardStates[originalRookRow][originalRookCol] = OFF;
        write_leds(0);

      } else if (moveType == ENPASSANT){
        Serial.println("The move type is definitely enpassant");
          int mux, loop, capturedRow;
          if (boardRow == 2){
            capturedRow = 3;
          
          } else if (boardRow == 5){
            capturedRow = 4;
          }
          findMuxAndLoop(capturedRow, boardCol, &mux, &loop);
          int en_passant_reading = 0;
          while (en_passant_reading <= THRESHOLD) {
              if (mux == A) {
                  Serial.println("We are at muxA for en passant");
                  mux_A.channel(loop);
                  analogRead(a_common_pin); // settling read
                  en_passant_reading = analogRead(a_common_pin);
              } 
              else if (mux == B) {
                  mux_B.channel(loop);
                  analogRead(b_common_pin); // settling read
                  en_passant_reading = analogRead(b_common_pin);
              } 
              else if (mux == C) {
                  mux_C.channel(loop);
                  analogRead(c_common_pin); // settling read
                  en_passant_reading = analogRead(c_common_pin);
              } 
              else if (mux == D) {
                  mux_D.channel(loop);
                  analogRead(d_common_pin); // settling read
                  en_passant_reading = analogRead(d_common_pin);
              }
          }
          Serial.println("We removed the enpassant piece");
          boardStates[capturedRow][boardCol] = OFF;
          board[capturedRow][boardCol] = empty;

          }
        }
      
    boardStates[boardRow][boardCol] = ON;


    //updating board w/ move
    for (int row = 0; row < BOARDSIZE; row++){
      for (int col = 0; col < BOARDSIZE; col++){
        if (boardStates[row][col] == MOVING){

          Piece sourcePiece = board[row][col];
          board[boardRow][boardCol] = board[row][col];
          
          board[row][col] = empty;
          boardStates[row][col] = OFF;

          board[boardRow][boardCol].rank = 8 - boardRow;

          char letter = 'a' + boardCol;
          board[boardRow][boardCol].file = letter;
          board[boardRow][boardCol].hasMoved = true;
          
          printBoard(board);
          Serial.print("boardRow: ");
          Serial.println(boardRow);
          Serial.print("boardCol: ");
          Serial.println(boardCol);

          removeEnPassant();

          if (board[boardRow][boardCol].type == 'P' || board[boardRow][boardCol].type == 'p') {
            addEnPassant(sourcePiece, board[boardRow][boardCol].rank, letter);
          }
    
          if (colour == WHITE) {
            colour = BLACK;
          } else if (colour == BLACK) {
            colour = WHITE;
          }

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
  
          
        }
      
      }
    }
    //removes all the LEDS
    write_leds(0);


    } else {
      //if this is not a legal move
      Serial.println("You are making an illegal move");
    }
  }
  




uint64_t calculateallLED(int row, int col){
  uint64_t sum = 0ull;
  for (int i = 0; i < arraySize(board[row][col].availableMoves); i++){
    sum += calculateoneLED(board[row][col].availableMoves[i]);
  }

  return sum;
}

uint64_t calculateoneLED(int availableMove){
  int row = availableMove / 10;
  int col = availableMove % 10;
  int drain_num = -1;
  int reg_num = -1;

  // THIS IS TO FIND THE DRAIN # (drain #'s are 0 to 7)
  drain_num = BOARDSIZE - 1 - col;

  //THIS IS TO FIND THE REG # (reg's are 0 to 7)
  reg_num = row;
  
  uint64_t addend = 1ULL;
  //in these tpic registers w/ shiftout MSBFIRST, Drain0 fills first for original AND daisy-chained registers

  // Serial.print("Reg_num: ");
  // Serial.println(reg_num);
  // Serial.print("drain_num: ");
  // Serial.println(drain_num);
  addend = addend << ((reg_num * 8) + drain_num);

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

int isLegalMove(int moveMade, Piece *pieceMoved){
  for (int i = 0; i < arraySize(pieceMoved -> availableMoves); i++){
    int availableMove = pieceMoved -> availableMoves[i];
    if (moveMade == availableMove){
      return NORMAL;
    } else if (moveMade != 0 && moveMade == availableMove / 100) {
      return CASTLING;
    } else if (moveMade != 0 && moveMade == availableMove / 1000){
      return ENPASSANT;
    }
  }
  return NOTLEGAL;
}
void setup()
{
  Serial.begin(115200);    
  Serial.println("REMEMBER THAT SETUP IS NOT YET COMPLETE");
  //setup from vscode main
  setupfunction();


  //boardStates setup
  for (int i = 0; i < BOARDSIZE; i++){
    for (int j = 0; j < BOARDSIZE; j++){
      boardStates[i][j] = OFF;
    }
  }

  boardStates[6][1] = ON;
  boardStates[1][2] = ON;
  boardStates[0][3] = ON;
  boardStates[7][4] = ON;
  boardStates[7][7] = ON;
  
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
  bool isMoving = false;
  bool isCapturing = false;
  // print initial board and legal moves
  // printBoard(board);
  // printLegalMoves();

  // if there is a legal move to be made
  while (haveMove) {
    
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
    }
      




  // printLegalMoves();
  // printBoard(board);



  // haveMove = checkLegalMoves(colour);
  
  



  }
  }

  // if (isMoving == false && isCapturing == false){
  //   if (inCheck(colour)) {
  //     checkmateResult();
  //   } else {
  //     stalemateResult();
  //   }
  // }

}




  
  





