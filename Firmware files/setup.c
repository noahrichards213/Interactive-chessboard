#include "setup.h"

#include <stdbool.h>

#include "availablemoves.h"
#include "constantsandstructs.h"
#include "legalmoveandcapture.h"

extern Piece board[8][8];

void setupfunction() {
  // first need to place pieces, then calculate all moves
  Piece P1 = {false, 1, 2, 'a', 'P', NULL};
  Piece P2 = {false, 1, 2, 'b', 'P', NULL};
  Piece P3 = {false, 1, 2, 'c', 'P', NULL};
  Piece P4 = {false, 1, 2, 'd', 'P', NULL};
  Piece P5 = {false, 1, 2, 'e', 'P', NULL};
  Piece P6 = {false, 1, 2, 'f', 'P', NULL};
  Piece P7 = {false, 1, 2, 'g', 'P', NULL};
  Piece P8 = {false, 1, 2, 'h', 'P', NULL};

  Piece R1 = {false, 1, 1, 'a', 'R', NULL};
  Piece R2 = {false, 1, 1, 'h', 'R', NULL};
  Piece N1 = {false, 1, 1, 'b', 'N', NULL};
  Piece N2 = {false, 1, 1, 'g', 'N', NULL};
  Piece B1 = {false, 1, 1, 'c', 'B', NULL};
  Piece B2 = {false, 1, 1, 'f', 'B', NULL};
  Piece Q1 = {false, 1, 1, 'd', 'Q', NULL};
  Piece K1 = {false, 1, 1, 'e', 'K', NULL};

  Piece p1 = {false, 0, 7, 'a', 'p', NULL};
  Piece p2 = {false, 0, 7, 'b', 'p', NULL};
  Piece p3 = {false, 0, 7, 'c', 'p', NULL};
  Piece p4 = {false, 0, 7, 'd', 'p', NULL};
  Piece p5 = {false, 0, 7, 'e', 'p', NULL};
  Piece p6 = {false, 0, 7, 'f', 'p', NULL};
  Piece p7 = {false, 0, 7, 'g', 'p', NULL};
  Piece p8 = {false, 0, 7, 'h', 'p', NULL};

  Piece r1 = {false, 0, 8, 'a', 'r', NULL};
  Piece r2 = {false, 0, 8, 'h', 'r', NULL};
  Piece n1 = {false, 0, 8, 'b', 'n', NULL};
  Piece n2 = {false, 0, 8, 'g', 'n', NULL};
  Piece b1 = {false, 0, 8, 'c', 'b', NULL};
  Piece b2 = {false, 0, 8, 'f', 'b', NULL};
  Piece q1 = {false, 0, 8, 'd', 'q', NULL};
  Piece k1 = {false, 0, 8, 'e', 'k', NULL};

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
