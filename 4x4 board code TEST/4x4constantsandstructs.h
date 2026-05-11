#ifndef CONSTANTSANDSTRUCTS
#define CONSTANTSANDSTRUCTS

#include <stdbool.h>
#include <stdio.h>

// struct for Piece
typedef struct Piece {
  bool hasMoved;
  // white is 1, black is 0
  int colour;
  int rank;
  char file;
  // if it's an empty square, we have type E (for empty)
  char type;
  /* each square on the board is represented by a number, we
  pass an array of these numbers to the struct for the piece */
  int* availableMoves;
} Piece;

// constants
static const int WHITEKINGCASTLE = 3200;
static const int WHITEQUEENCASTLE = 3100;
static const int BLACKKINGCASTLE = 200;
static const int BLACKQUEENCASTLE = 100;

static const int ALLOWSCHECK = -5;
static const int CASTLINGTHROUGHCHECK = -2;
static const int CASTLINGOUTOFCHECK = -3;
static const int EN_PASSANT = -4;

static const int WHITE = 1;
static const int BLACK = 0;

#define BOARDSIZE 4

static const Piece empty = {false, -1, -1, 'z', '_', NULL};

#endif
