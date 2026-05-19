// including necessary default libraries
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// including header files

// USEFUL FUNCTIONS

// arraysize
#include "4x4arraysize.h"

// check one dir
#include "4x4checkonedir.h"

// availablemoves
#include "4x4availablemoves.h"

// END OF USEFUL FUNCTIONS

// constants and piece data structure
#include "4x4constantsandstructs.h"

// functions for all legal moves
#include "4x4availablemoves.h"

// global board
Piece board[BOARDSIZE][BOARDSIZE];

// setup
#include "4x4setup.h"

// make move
#include "4x4makemove.h"

// printing things to console
#include "4x4printing.h"

// checking functions
#include "4x4checking.h"

// include legal move and capture functions
#include "4x4legalmoveandcapture.h"

// include en passant mechanics
#include "4x4enpassant.h"

// include function to remove illegal moves because it would leave player in
// check
#include "4x4removecheck.h"

int main() {
  // setting up board & list of pieces
  blankSquares();
  testPawn();

  int colour = WHITE;

  for (int i = 0; i < BOARDSIZE; i++) {
    for (int j = 0; j < BOARDSIZE; j++) {
      if (board[i][j].type != '_') {
        changeAvailableMoves(&board[i][j]);
      }
    }
  }

  bool haveMove = true;

  // print initial board and legal moves
  printBoard();
  printLegalMoves();

  // if there is a legal move to be made
  while (haveMove) {
    makeMove(colour);

    if (colour == WHITE) {
      colour = BLACK;
    } else if (colour == BLACK) {
      colour = WHITE;
    }

    // change legal moves
    for (int i = 0; i < BOARDSIZE; i++) {
      for (int j = 0; j < BOARDSIZE; j++) {
        if (board[i][j].type != '_') {
          changeAvailableMoves(&board[i][j]);
        }
      }
    }

    for (int i = 0; i < BOARDSIZE; i++) {
      for (int j = 0; j < BOARDSIZE; j++) {
        // first, we need to check every single move and see if it leads to
        // check (it then would be unallowed)
        if (board[i][j].colour == colour) {
          for (int k = 0; k < arraySize(board[i][j].availableMoves); k++) {
            if (removeCheck(board[i][j], k, board[i][j].availableMoves[k]) ==
                true) {
              board[i][j].availableMoves[k] = ALLOWSCHECK;
            }
          }
        }
      }
    }

    printLegalMoves();
    printBoard();

    haveMove = checkLegalMoves(colour);
  }

  if (inCheck(colour)) {
    printf("It's checkmate\n");
  } else {
    printf("It's stalemate\n");
  }

  return 0;
}
