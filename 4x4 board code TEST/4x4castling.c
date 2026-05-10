#include "4x4castling.h"

#include <stdio.h>

#include "4x4constantsandstructs.h"
#include "4x4legalmoveandcapture.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

void executeCastling(int castlingMove) {
  int rank;
  int colour;
  char oldfileKing;
  char newfileKing;
  char typeKing;
  char typeRook;
  char newfileRook;
  char oldfileRook;

  if (castlingMove == WHITEKINGCASTLE || castlingMove == WHITEQUEENCASTLE) {
    rank = 1;
    colour = WHITE;
    typeKing = 'K';
    typeRook = 'R';
  } else if (castlingMove == BLACKKINGCASTLE ||
             castlingMove == BLACKQUEENCASTLE) {
    rank = 8;
    colour = BLACK;
    typeKing = 'k';
    typeRook = 'r';
  }

  if (castlingMove == WHITEKINGCASTLE || castlingMove == BLACKKINGCASTLE) {
    oldfileKing = 'a';
    newfileKing = 'c';
    newfileRook = 'b';
    oldfileRook = 'd';

  } else if (castlingMove == WHITEQUEENCASTLE ||
             castlingMove == BLACKQUEENCASTLE) {
    oldfileKing = 'd';
    newfileKing = 'b';
    newfileRook = 'c';
    oldfileRook = 'a';
  }

  // still need to change availableMoves
  Piece king = {true, colour, rank, newfileKing, typeKing, NULL};
  Piece rook = {true, colour, rank, newfileRook, typeRook, NULL};

  // original king square
  board[BOARDSIZE - rank][oldfileKing - 97] = empty;

  // original rook square
  board[BOARDSIZE - rank][oldfileRook - 97] = empty;

  // new king square
  board[BOARDSIZE - rank][newfileKing - 97] = king;

  // new rook square
  board[BOARDSIZE - rank][newfileRook - 97] = rook;
}