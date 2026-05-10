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
  setupfunction();

  printf("We are setting up before crashing\n");

  int colour = WHITE;
  bool haveMove = true;

  // print initial board and legal moves
  printBoard(board);
  printLegalMoves();

  // if there is a legal move to be made
  while (haveMove) {
    makeMove(colour);

    // change legal moves
    for (int i = 0; i < BOARDSIZE; i++) {
      for (int j = 0; j < BOARDSIZE; j++) {
        if (board[i][j].type != '_') {
          changeAvailableMoves(&board[i][j], colour);
        }
      }
    }

    for (int i = 0; i < BOARDSIZE; i++) {
      for (int j = 0; j < BOARDSIZE; j++) {
        // first, we need to check every single move and see if it leads to
        // check (it then would be unallowed)
        int size = arraySize(board[i][j].availableMoves);
        for (int k = 0; k < size; k++) {
          if (board[i][j].type != '_') {
            if (removeCheck(board[i][j], k, board[i][j].availableMoves[k]) ==
                true) {
              if (board[i][j].type == 'n') {
                printf("The piece was: %c\n", board[i][j].type);
                printf("These are its legal moves:\n");
                for (int p = 0; p < arraySize(board[i][j].availableMoves);
                     p++) {
                  printf("%d", board[i][j].availableMoves[p]);
                }
              }
              board[i][j].availableMoves[k] = ALLOWSCHECK;
            }
          }
        }
      }
    }
    printLegalMoves();
    printBoard(board);

    if (colour == WHITE) {
      colour = BLACK;
    } else if (colour == BLACK) {
      colour = WHITE;
    }
    haveMove = checkLegalMoves(colour);
  }

  if (inCheck(colour)) {
    printf("It's checkmate\n");
  } else {
    printf("It's stalemate\n");
  }

  return 0;
}
