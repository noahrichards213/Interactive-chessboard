#include "4x4checkonedir.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "4x4constantsandstructs.h"

extern Piece board[BOARDSIZE][BOARDSIZE];

// i can check Left, right up, down, top right, top, left, bottom right, bottom
// left, with one fxn hort & vert tells us which dir we're checking. e.g hort 1
// & vert 1 is top right -1 to 0 depending on direction
int* checkOneDir(Piece piece, int hort, int vert) {
  // - vert & + hort are the direction we are checking
  int rank = piece.rank;
  char file = piece.file;
  int colour = piece.colour;

  // max # of moves to be made in one direction + 1 for null terminator
  int capacityOneDir = 9;

  // storing numbers that represent spots on board
  int* possibleMoves = malloc(capacityOneDir * sizeof(int));
  if (possibleMoves == NULL) {
    printf("NULL");
  }

  int i = 0;
  // ensures we don't go off edge of board
  while ((rank + vert) >= 1 && (rank + vert) <= BOARDSIZE && (file + hort) >= 'a' &&
         (file + hort) <= 'a' + BOARDSIZE - 1) {
    // we come across empty square
    if (board[BOARDSIZE - (rank + vert)][file + hort - 97].type == '_') {
      possibleMoves[i] = (BOARDSIZE - (rank + vert)) * 10 + (file - 97) + hort;
      i++;

      if (hort != 0) {
        hort = hort + hort / abs(hort);
      }

      if (vert != 0) {
        vert = vert + vert / abs(vert);
      }

    //come across enemy piece
    } else if (board[BOARDSIZE - (rank + vert)][file + hort - 97].colour != colour) {
      possibleMoves[i] = (BOARDSIZE - (rank + vert)) * 10 + (file - 97) + hort;
      possibleMoves[i + 1] = -1;
      return possibleMoves;

      // we come across ally piece
    } else if (board[BOARDSIZE - (rank + vert)][file + hort - 97].colour == colour) {
      possibleMoves[i] = -1;
      return possibleMoves;
    }
  }
  possibleMoves[i] = -1;
  return possibleMoves;
}