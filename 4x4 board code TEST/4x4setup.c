#include "4x4setup.h"

#include <stdbool.h>

#include "4x4availablemoves.h"
#include "4x4constantsandstructs.h"
#include "4x4legalmoveandcapture.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

void initialBoard() {
  for (int i = 0; i < BOARDSIZE; i++){
    for (int j = 0; j < BOARDSIZE; j++){
      board[i][j] = empty;
    }
  }
  return;
}


void testPawn() {
  Piece P1 = {false, WHITE, 1, 'b', 'P', NULL};
  Piece P2 = {false, WHITE, 1, 'c', 'P', NULL};
  Piece p1 = {false, BLACK, 4, 'b', 'p', NULL};
  Piece p2 = {false, BLACK, 4, 'c', 'p', NULL};

  board[3][1] = P1;
  board[3][2] = P2;
  board[0][1] = p1;
  board[0][2] = p2;
  return;
}

void testKnight() {
  Piece K1 = {false, WHITE, 1, 'b', 'N', NULL};
  Piece K2 = {false, WHITE, 1, 'c', 'N', NULL};
  Piece k1 = {false, BLACK, 4, 'b', 'n', NULL};
  Piece k2 = {false, BLACK, 4, 'c', 'n', NULL};

  board[3][1] = K1;
  board[3][2] = K2;
  board[0][1] = k1;
  board[0][2] = k2;
  return;
}

void testBishop() {
  Piece B1 = {false, WHITE, 1, 'b', 'B', NULL};
  Piece B2 = {false, WHITE, 1, 'c', 'B', NULL};
  Piece b1 = {false, WHITE, 4, 'b', 'b', NULL};
  Piece b2 = {false, WHITE, 4, 'c', 'b', NULL};

  board[3][1] = B1;
  board[3][2] = B2;
  board[0][1] = b1;
  board[0][2] = b2;
  return;
}

void testQueen() {
  Piece Q1 = {false, WHITE, 1, 'b', 'Q', NULL};
  Piece q1 = {false, WHITE, 1, 'b', 'q', NULL};

  board[3][1] = Q1;
  board[0][1] = q1;
  return;
}

void testKingSide() {
  Piece K1 = {false, WHITE, 1, 'a', 'K', NULL};
  Piece R1 = {false, WHITE, 1, 'd', 'R', NULL};
  Piece k1 = {false, BLACK, 4, 'a', 'k', NULL};
  Piece r1 = {false, BLACK, 4, 'd', 'r', NULL};

  board[3][0] = K1;
  board[3][3] = R1;
  board[0][0] = k1;
  board[0][3] = r1;
  return;
}

void testQueenSide() {
  Piece K2 = {false, WHITE, 1, 'd', 'K', NULL};
  Piece R2 = {false, WHITE, 1, 'a', 'R', NULL};
  Piece k2 = {false, BLACK, 4, 'd', 'k', NULL};
  Piece r2 = {false, BLACK, 4, 'a', 'r', NULL};

  board[3][3] = K2;
  board[3][0] = R2;
  board[0][3] = k2;
  board[0][0] = r2; 
  return;
}

