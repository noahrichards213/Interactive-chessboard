#include "castling.h"

#include <stdio.h>

#include "constantsandstructs.h"
#include "legalmoveandcapture.h"

extern Piece board[8][8];

void executeCastling(int castlingMove) {
  int rank;
  int colour;
  char oldfileKing = 'e';
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
    newfileKing = 'g';
    newfileRook = 'f';
    oldfileRook = 'h';

  } else if (castlingMove == WHITEQUEENCASTLE ||
             castlingMove == BLACKQUEENCASTLE) {
    newfileKing = 'c';
    newfileRook = 'd';
    oldfileRook = 'a';
  }

  // still need to change availableMoves
  Piece king = {true, colour, rank, newfileKing, typeKing, NULL};
  Piece rook = {true, colour, rank, newfileRook, typeRook, NULL};

  // original king square
  board[8 - rank][oldfileKing - 97] = empty;

  // original rook square
  board[8 - rank][oldfileRook - 97] = empty;

  // new king square
  board[8 - rank][newfileKing - 97] = king;

  // new rook square
  board[8 - rank][newfileRook - 97] = rook;
}