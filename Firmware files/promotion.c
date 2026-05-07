#include "promotion.h"

#include <stdio.h>

#include "constantsandstructs.h"

#include "legalmoveandcapture.h"

Piece executePromotion(int colour, int rankDest, char fileDest) {
  char bufferPromotion[2];

  char chosenPiece;

  //need getchar() to remove newline
  int c;

  printf("Rook (r), knight (n), bishop (b), or queen (q): ");
  if (fgets(bufferPromotion, sizeof(bufferPromotion), stdin) != NULL) {
    chosenPiece = bufferPromotion[0];

    c = getchar();

    //if we have a white piece, the uppercase letters are 32 lower in the ASCII table
    if (colour == WHITE){
      chosenPiece -= 32;
    }
    printf("This is the chosen piece: %c", chosenPiece);
    Piece promotedPawn = {true, colour, rankDest, fileDest, chosenPiece, NULL};
    return promotedPawn;
  } else {
    printf("Error with input.");
    return empty;
  }
}