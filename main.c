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
#include "arraysize.h"

// check one dir
#include "checkonedir.h"

// availablemoves
#include "availablemoves.h"

// END OF USEFUL FUNCTIONS

// constants and piece data structure
#include "constantsandstructs.h"

// functions for all legal moves
#include "availablemoves.h"

// global board
Piece board[8][8];

// setup
#include "setup.h"

// make move
#include "makemove.h"

// printing things to console
#include "printing.h"

// checking functions
#include "checking.h"

// include legal move and capture functions
#include "legalmoveandcapture.h"

// include en passant mechanics
#include "enpassant.h"

// include function to remove illegal moves because it would leave player in
// check
#include "removecheck.h"

int main() {
  // setting up board & list of pieces
  setupfunction();

  printf("We are setting up before crashing\n");

  int colour = WHITE;
  bool haveMove = true;

  // print initial board and legal moves
  printBoard();
  printLegalMoves();

  // if there is a legal move to be made
  while (haveMove) {
    makeMove(colour);

    // change legal moves
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (board[i][j].type != '_') {
          changeAvailableMoves(&board[i][j], colour);
        }
      }
    }

    printBoard();
    printLegalMoves();

    // for (int i = 0; i < 8; i++) {
    //   for (int j = 0; j < 8; j++) {
    //     // first, we need to check every single move and see if it leads to
    //     // check (it then would be unallowed)
    //     int size = arraySize(board[i][j].availableMoves);
    //     for (int k = 0; k < size; k++) {
    //       // TESTING

    //       if (board[i][j].type != '_') {
    //         if (removeCheck(board[i][j], k, board[i][j].availableMoves[k]) ==
    //             true) {
    //           board[i][j].availableMoves[k] = ALLOWSCHECK;
    //         }
    //       }

    //       // now we check that we're through check
    //       if (board[i][j].type != '_') {
    //         if (board[i][j].availableMoves[k] == WHITEKINGCASTLE ||
    //             board[i][j].availableMoves[k] == BLACKKINGCASTLE ||
    //             board[i][j].availableMoves[k] == WHITEQUEENCASTLE ||
    //             board[i][j].availableMoves[k] == BLACKQUEENCASTLE) {
    //           if (throughCheck(board[i][j].availableMoves[k], colour) == true) {
    //             board[i][j].availableMoves[k] = CASTLINGTHROUGHCHECK;
    //           }

    //           // now we check that we're castling out of check (in check is
    //           // covered by removing moves that result in check)
    //           if (inCheck(colour)) {
    //             board[i][j].availableMoves[k] = CASTLINGOUTOFCHECK;
    //           }
    //         }
    //       }
    //     }
    //   }
    // }

    if (colour == WHITE) {
      colour = BLACK;
    } else if (colour == BLACK) {
      colour = WHITE;
    }
  }
  haveMove = checkLegalMoves(colour);

  if (haveMove == false) {
    if (inCheck(colour)) {
      printf("It's checkmate\n");
    } else {
      printf("It's stalemate\n");
    }
  }

  return 0;
}
