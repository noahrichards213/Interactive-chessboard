#include "4x4makemove.h"

#include <math.h>
#include <stdio.h>

#include "4x4arraysize.h"
#include "4x4availablemoves.h"
#include "4x4castling.h"
#include "4x4enpassant.h"
#include "4x4legalinput.h"
#include "4x4legalmoveandcapture.h"
#include "4x4printing.h"
#include "4x4promotion.h"
extern Piece board[BOARDSIZE][BOARDSIZE];

// keep in mind we are designing for actual circuit
//  so we will not actually use fgets

void makeMove(int colour) {
  char fileSource;
  int rankSource;
  Piece sourcePiece = empty;

  // determining if source move is legal
  while (sourcePiece.type == '_') {
    int c;
    char bufferSource[3];
    printf("This is the colour: %d\n", colour);
    printf("Which piece?");
    if (fgets(bufferSource, sizeof(bufferSource), stdin) != NULL) {
      fileSource = bufferSource[0];

      // manipulating ascii value of '1', '2', '3', etc.
      rankSource = (int)bufferSource[1] - 48;

      c = getchar();

      sourcePiece = legalSource(fileSource, rankSource, colour);
      if (sourcePiece.type == '_') {
        printf("You do not have a piece there. Try again: ");
        printf("Your colour is %d\n", colour);
        printf("The type that you chose was %c\n",
               board[BOARDSIZE - rankSource][fileSource - 97].type);
      }
    } else {
      printf("Error with source input.");
    }
  }

  char fileDest;
  int rankDest;
  int yesDest = -1;

  // determing if dest move is legal
  while (yesDest == -1) {
    int c;
    char bufferDest[3];
    printf("To where?");

    if (fgets(bufferDest, sizeof(bufferDest), stdin) != NULL) {
      fileDest = bufferDest[0];

      // manipulating ascii value of '1', '2', '3', etc.
      rankDest = (int)bufferDest[1] - 48;

      // to clear nextline character left in buffer

      c = getchar();

      yesDest = legalDest(fileDest, rankDest, fileSource, rankSource, colour);
      if (yesDest == -1) {
        printf("That is not a legal move. Please try again.");
      }
    } else {
      printf("Error with dest input.");
    }
  }

  // remove en passant moves if they weren't aleady made
  removeEnPassant();

  // handles promotion (but still need to add available moves for promoted
  // piece in)
  if ((sourcePiece.type == 'P' && rankDest == BOARDSIZE) ||
      (sourcePiece.type == 'p' && rankDest == 1)) {
    board[BOARDSIZE - rankDest][fileDest - 97] =
        executePromotion(colour, rankDest, fileDest);
    board[BOARDSIZE - rankDest][fileDest - 97].rank = rankDest;
    board[BOARDSIZE - rankDest][fileDest - 97].file = fileDest;
    board[BOARDSIZE - rankDest][fileDest - 97].hasMoved = true;

    printf("this is the promoted type: %c\n",
           board[BOARDSIZE - rankDest][fileDest - 97].type);
    board[BOARDSIZE - rankSource][fileSource - 97] = empty;
    return;
  }

  // handles castling

  // this range is just castling moves, no enpassant
  if ((yesDest >= 100 && yesDest < 10000)) {
    executeCastling(yesDest);
    return;
  }

  // regular piece movement
  board[BOARDSIZE - rankDest][fileDest - 97] = sourcePiece;
  // but we change rank and file, set avail moves to NULL (will be chnaged later
  // but now fails loudly)
  board[BOARDSIZE - rankDest][fileDest - 97].rank = rankDest;
  board[BOARDSIZE - rankDest][fileDest - 97].file = fileDest;
  board[BOARDSIZE - rankDest][fileDest - 97].hasMoved = true;

  // actually we need to set opposite colour to NULL when it's not their turn

  board[BOARDSIZE - rankSource][fileSource - 97] = empty;

  // here we remove the enemy pawn if the move is en passant (won't remove
  // regularly)

  if (yesDest >= 10000) {
    int boardDir;
    if (colour == WHITE) {
      // rank decreases as pawns move up
      boardDir = 1;
    } else if (colour == BLACK) {
      // rank increases as pawns move down
      boardDir = -1;
    }
    board[BOARDSIZE - rankDest + boardDir][fileDest - 97] = empty;
    printf("WE ARE ARRIVING HERE\n");
  }
  // here we check if we need too add en passant (kind of weird with one
  // condition in each function but whatever, can fix later)
  if (sourcePiece.type == 'P' || sourcePiece.type == 'p') {
    addEnPassant(sourcePiece, rankDest, fileDest);
  }
  return;
}
