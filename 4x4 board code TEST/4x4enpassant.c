#include "4x4enpassant.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "4x4arraysize.h"
#include "4x4printing.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

void removeEnPassant() {
  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      int size = arraySize(board[i][j].availableMoves);
      for (int k = 0; k < size; k++) {
        // has en passant move
        if (board[i][j].availableMoves[k] >= 10000) {
          board[i][j].availableMoves[k] = EN_PASSANT;
        }
      }
    }
  }
}

// colour is the pawn that moved two squares
// rank and file is the square that it moved to (use actual inputted values)
void addEnPassant(Piece piece, int rank, char file) {
  int colour = piece.colour;
  char enemyPawn;

  // old rank - new rank
  int difference = abs(piece.rank - rank);

  // if pawn did not move two squares then we don't need this function
  if (difference != 2) {
    printf("We did return\n");
    return;
  }

  // white is 1, black is 0
  enemyPawn = colour ? 'p' : 'P';
  // we know that if the pawn successfully moved two
  // squares nothing can be blocking it

  // STILL NEED TO DO MAKE THIS . IT'S CURRENTLY NONSENSE
  //  left side en passant

  int boardRank = BOARDSIZE - rank;
  int boardFile = file - 97;

  // only one en passant move is possible per turn per piece
  int capacityMoves = 1;
  int* enPassantMove = malloc(sizeof(int) * capacityMoves);

  int boardDir;
  if (colour == WHITE) {
    // rank decreases as pawns move up
    boardDir = 1;
  } else if (colour == BLACK) {
    // rank increases as pawns move down
    boardDir = -1;
  }
  // this is immediately recognized and deal with in change avialable moves
  if (board[boardRank][boardFile - 1].type == enemyPawn) {
    // creating en passant array move
    //  multiples the enpassant square by 1000 to differentiate from reg move
    enPassantMove[0] = ((boardRank + boardDir) * 10 + (boardFile)) * 1000;
    board[boardRank][boardFile - 1].availableMoves = enPassantMove;
  }

  if (board[boardRank][boardFile + 1].type == enemyPawn) {
    // creating en passant array move
    //  multiples the enpassant square by 1000 to differentiate from reg move
    enPassantMove[0] = ((boardRank + boardDir) * 10 + (boardFile)) * 1000;
    board[boardRank][boardFile + 1].availableMoves = enPassantMove;
  }

  printf("We are unlocking en passant\n");

  return;
}
