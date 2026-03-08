#include "promotion.h"

#include <stdio.h>

#include "constantsandstructs.h"

Piece executePromotion(int colour, int rankDest, char fileDest) {
  char bufferPromotion[2];

  char chosenPiece;
  printf("Rook (r), knight (n), bishop (b), or queen (q): ");
  if (fgets(bufferPromotion, sizeof(bufferPromotion), stdin) != NULL) {
    chosenPiece = bufferPromotion[0];
    // Note: must still change availablemoves, probably in main
    Piece promotedPawn = {true, colour, rankDest, fileDest, chosenPiece, NULL};
    return promotedPawn;
  } else {
    printf("Error with input.");
    return empty;
  }
}