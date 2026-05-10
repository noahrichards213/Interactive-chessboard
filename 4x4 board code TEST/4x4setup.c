#include "4x4setup.h"

#include <stdbool.h>

#include "4x4availablemoves.h"
#include "4x4constantsandstructs.h"
#include "4x4legalmoveandcapture.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

void blankSquares() {
  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
    }
  }
}
void testPawn() {
  Piece P1 = {false, WHITE, 1, 'b', 'P', NULL};
  Piece P2 = {false, WHITE, 1, 'c', 'P', NULL};
  Piece p2 = {false, BLACK, 4, 'b', 'p', NULL};
  Piece p2 = {false, BLACK, 4, 'c', 'p', NULL};

  board[1][3] = P1;
  board[2][3] = P2;
  board[0][1] = p1;
  board[0][2] = p2;
}

void testKnight() {
  Piece K1 = {false, WHITE, 1, 'b', 'N', NULL};
  Piece K2 = {false, WHITE, 1, 'c', 'N', NULL};
  Piece k1 = {false, BLACK, 4, 'b', 'n', NULL};
  Piece k2 = {false, BLACK, 4, 'c', 'n', NULL};
}

void testBishop() {
  Piece B1 = {false, WHITE, 1, 'b', 'B', NULL};
  Piece B2 = {false, WHITE, 1, 'c', 'B', NULL};
  Piece b1 = {false, WHITE, 4, 'b', 'b', NULL};
  Piece b2 = {false, WHITE, 4, 'c', 'b', NULL};
}

void testQueen() {
  Piece Q1 = {false, WHITE, 1, 'b', 'Q', NULL};
  Piece q1 = {false, WHITE, 1, 'b', 'q', NULL};
}

void testKingSide() {
  Piece K1 = {false, WHITE, 1, 'a', 'K', NULL};
  Piece R1 = {false, WHITE, 1, 'd', 'R', NULL};
  Piece k1 = {false, BLACK, 4, 'a', 'k', NULL};
  Piece r1 = {false, BLACK, 4, 'd', 'R', NULL};
}

void testQueenside() {
  Piece K1 = {false, WHITE, 1, 'd', 'K', NULL};
  Piece R1 = {false, WHITE, 1, 'a', 'R', NULL};
  Piece k1 = {false, BLACK, 4, 'd', 'k', NULL};
  Piece r1 = {false, BLACK, 4, 'a', 'R', NULL};
}

// back row for black
board[0][0] = r1;
board[0][1] = n1;
board[0][2] = b1;
board[0][3] = q1;
board[0][4] = k1;
board[0][5] = b2;
board[0][6] = n2;
board[0][7] = r2;

// pawns for black
board[1][0] = p1;
board[1][1] = p2;
board[1][2] = p3;
board[1][3] = p4;
board[1][4] = p5;
board[1][5] = p6;
board[1][6] = p7;
board[1][7] = p8;

// back row for white
board[7][0] = R1;
board[7][1] = N1;
board[7][2] = B1;
board[7][3] = Q1;
board[7][4] = K1;
board[7][5] = B2;
board[7][6] = N2;
board[7][7] = R2;

// pawn row for white
board[6][0] = P1;
board[6][1] = P2;
board[6][2] = P3;
board[6][3] = P4;
board[6][4] = P5;
board[6][5] = P6;
board[6][6] = P7;
board[6][7] = P8;

// empty spaces in the middle
for (int i = 2; i < 6; i++) {
  for (int j = 0; j < 8; j++) {
    board[i][j] = empty;
  }
}

// initial legal moves
for (int i = 0; i < 8; i++) {
  for (int j = 0; j < 8; j++) {
    if (board[i][j].type != '_') {
      changeAvailableMoves(&board[i][j], BLACK);
    }
  }
}
}
