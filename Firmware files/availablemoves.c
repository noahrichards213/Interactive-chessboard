#include "availablemoves.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "arraysize.h"
#include "checkonedir.h"
#include "constantsandstructs.h"

extern Piece board[8][8];

int* rookMoves(Piece piece) {
  int* left = checkOneDir(piece, -1, 0);
  int* up = checkOneDir(piece, 0, 1);
  int* right = checkOneDir(piece, 1, 0);
  int* down = checkOneDir(piece, 0, -1);

  int leftSize = arraySize(left);
  int rightSize = arraySize(right);
  int upSize = arraySize(up);
  int downSize = arraySize(down);

  int totalSize = leftSize + rightSize + upSize + downSize;

  int* allMoves = malloc((totalSize + 1) * (sizeof(int)));
  if (allMoves == NULL) {
    printf("NULL");
  }
  int index = 0;

  for (int i = 0; i < leftSize; i++, index++) {
    allMoves[index] = left[i];
  }

  for (int i = 0; i < rightSize; i++, index++) {
    allMoves[index] = right[i];
  }

  for (int i = 0; i < upSize; i++, index++) {
    allMoves[index] = up[i];
  }

  for (int i = 0; i < downSize; i++, index++) {
    allMoves[index] = down[i];
  }
  allMoves[index] = -1;

  return allMoves;
}

int* bishopMoves(Piece piece) {
  int* topRight = checkOneDir(piece, 1, 1);
  int* topLeft = checkOneDir(piece, -1, 1);
  int* bottomRight = checkOneDir(piece, 1, -1);
  int* bottomLeft = checkOneDir(piece, -1, -1);

  int topRightSize = arraySize(topRight);
  int topLeftSize = arraySize(topLeft);
  int bottomRightSize = arraySize(bottomRight);
  int bottomLeftSize = arraySize(bottomLeft);

  int totalSize = topRightSize + topLeftSize + bottomRightSize + bottomLeftSize;

  int* allMoves = malloc((totalSize + 1) * (sizeof(int)));
  if (allMoves == NULL) {
    printf("NULL");
  }
  int index = 0;

  for (int i = 0; i < topRightSize; i++, index++) {
    allMoves[index] = topRight[i];
  }

  for (int i = 0; i < topLeftSize; i++, index++) {
    allMoves[index] = topLeft[i];
  }

  for (int i = 0; i < bottomRightSize; i++, index++) {
    allMoves[index] = bottomRight[i];
  }

  for (int i = 0; i < bottomLeftSize; i++, index++) {
    allMoves[index] = bottomLeft[i];
  }
  allMoves[index] = -1;

  return allMoves;
}

int* queenMoves(Piece piece) {
  int* movesFromRook = rookMoves(piece);
  int* movesFromBishop = bishopMoves(piece);

  int rookMovesSize = arraySize(movesFromRook);
  int bishopMovesSize = arraySize(movesFromBishop);

  int totalSize = rookMovesSize + bishopMovesSize;

  int* allMoves = malloc((totalSize + 1) * (sizeof(int)));
  if (allMoves == NULL) {
    printf("NULL");
  }
  int index = 0;

  for (int i = 0; i < rookMovesSize; i++, index++) {
    allMoves[index] = movesFromRook[i];
  }

  for (int i = 0; i < bishopMovesSize; i++, index++) {
    allMoves[index] = movesFromBishop[i];
  }
  allMoves[index] = -1;

  return allMoves;
}

int* knightMoves(Piece piece) {
  int rank = piece.rank;
  char file = piece.file;
  int colour = piece.colour;

  // max # of moves knight can make = 8 + 1 for -1 terminator
  int capacityMoves = 9;
  int* allMoves = malloc(capacityMoves * sizeof(int));

  int index = 0;

  // BEST TO DRAW THIS TO UNDERSTAND THE GEOMETRY

  // double for loop allows us to look in all four corners
  int addedRank, addedFile;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i != 0 && j != 0) {
        addedRank = 8 - (rank + 2 * j);
        //-97 to manipulate ascii value
        addedFile = (file + 2 * i) - 97;

        // first square per corner
        if (addedRank >= 0 && addedRank <= 7 && (addedFile - i) >= 0 &&
            (addedFile - i) <= 7) {
          if (board[addedRank][addedFile - i].type == '_' ||
              board[addedRank][addedFile - i].colour != colour) {
            allMoves[index] = addedRank * 10 + (addedFile - i);
            index++;
          }
        }
        // second square per corner
        if ((addedRank + j) >= 0 && (addedRank + j) <= 7 && addedFile >= 0 &&
            addedFile <= 7) {
          if (board[addedRank + j][addedFile].type == '_' ||
              board[addedRank + j][addedFile].colour != colour) {
            allMoves[index] = (addedRank + j) * 10 + addedFile;
            index++;
          }
        }
      }
    }
  }

  allMoves[index] = -1;
  return allMoves;
}

int* pawnMoves(Piece piece) {
  int rank = piece.rank;
  char file = piece.file;
  int colour = piece.colour;

  // handles the fact that black and white pawns move in diff directions
  int dir;
  if (colour == WHITE) {
    // rank decreases as pawns move up
    dir = -1;
  } else if (colour == BLACK) {
    // rank increases as pawns move down
    dir = 1;
  }

  // handles the fact that pawns can move two squares only on move 1;
  if (piece.hasMoved == false) {
    dir *= 2;
  }

  int normDir = dir / abs(dir);

  // max number of moves a pawn can make is 4 + 1 for terminator is 5;
  int capacityMoves = 5;
  int* allMoves = malloc(sizeof(int) * capacityMoves);

  int index = 0;

  // handles regular movement
  int i = 0;

  int nextRank = (8 - rank) + normDir;
  int maxMoves = abs(dir);
  while (i < maxMoves && board[nextRank][file - 97].type == '_') {
    // movement forward
    if (board[nextRank][file - 97].type == '_') {
      //-97 to manipulate ascii value
      allMoves[i] = nextRank * 10 + (file - 97);
      nextRank = nextRank + normDir;
      index++;
      i++;
    }
  }

  // handling captures
  // for left capture
  if ((file - 1) >= 'a') {
    if (board[(8 - rank) + normDir][(file - 97) - 1].colour != colour &&
        board[(8 - rank) + normDir][(file - 97) - 1].type != '_') {
      //-97 to manipulaet ascii value
      allMoves[index] = ((8 - rank) + normDir) * 10 + ((file - 97) - 1);
      index++;
    }
  }

  // for right capture
  if ((file + 1) <= 'h') {
    if (board[(8 - rank) + normDir][(file - 97) + 1].colour != colour &&
        board[(8 - rank) + normDir][(file - 97) + 1].type != '_') {
      //-97 to manipulaet ascii value

      allMoves[index] = ((8 - rank) + normDir) * 10 + ((file - 97) + 1);
      index++;
    }
  }

  allMoves[index] = -1;
  return allMoves;
}

int* kingMoves(Piece piece) {
  int rank = piece.rank;
  char file = piece.file;
  int colour = piece.colour;

  // max king moves is 8 + 1 for -1 terminator
  int capacityMoves = 9;
  int* noCheckMoves = malloc(sizeof(int) * capacityMoves);

  // regular king moves
  int index = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (!(i == 0 && j == 0)) {
        // -97 to manipulate ascii value
        int addedFile = (file - 97) + i;
        int addedRank = (8 - rank) + j;

        if (addedFile >= 0 && addedFile <= 7 && addedRank >= 0 &&
            addedRank <= 7) {
          if (board[addedRank][addedFile].type == '_' ||
              board[addedRank][addedFile].colour != colour) {
            noCheckMoves[index] = addedRank * 10 + addedFile;
            index++;
          }
        }
      }
    }
  }

  // castling
  if (piece.hasMoved == true) {
    noCheckMoves[index] = -1;
    return noCheckMoves;
  }

  // check if rook's haven't moved
  bool hRookisGood;
  bool aRookisGood;
  if (colour == WHITE) {
    hRookisGood = (board[7][7].type == 'R' && board[7][7].hasMoved == false);
    aRookisGood = (board[7][0].type == 'R' && board[0][7].hasMoved == false);
  } else if (colour == BLACK) {
    hRookisGood = (board[0][7].type == 'r' && board[7][7].hasMoved == false);
    aRookisGood = (board[0][0].type == 'r' && board[0][7].hasMoved == false);
  }

  if (!aRookisGood || !hRookisGood) {
    noCheckMoves[index] = -1;
    return noCheckMoves;
  }

  // now ensure square that square king and rook to are empty. first assume they
  // are true
  bool kingEmptySquares = true;
  bool queenEmptySquares = true;

  int i = 1;
  int j = 1;

  // kingside
  while (i < 3 && kingEmptySquares == true) {
    if (board[8 - rank][(file - 97) + i].type != '_') {
      kingEmptySquares = false;
    }
    i++;
  }

  // queenside
  while (j <= 3 && queenEmptySquares == true) {
    if (board[8 - rank][(file - 97) - j].type != '_') {
      queenEmptySquares = false;
    }
    j++;
  }

  // kingside
  if (kingEmptySquares == true) {
    if (piece.colour == WHITE) {
      noCheckMoves[index] = WHITEKINGCASTLE;
    } else if (piece.colour == BLACK) {
      noCheckMoves[index] = BLACKKINGCASTLE;
    }
    index++;
  }

  // queenside
  if (queenEmptySquares == true) {
    if (piece.colour == WHITE) {
      noCheckMoves[index] = BLACKKINGCASTLE;
    } else if (piece.colour == BLACK) {
      noCheckMoves[index] = BLACKQUEENCASTLE;
    }
    index++;
  }

  /*common conditions:
  can't castle out of, through, into check (handle in gameloop) */

  noCheckMoves[index] = -1;

  return noCheckMoves;
}
