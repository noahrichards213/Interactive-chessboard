#ifndef LEGALMOVEANDCAPTURE
#define LEGALMOVEANDCAPTURE

#include "constantsandstructs.h"
// inserts/updates piece info
void changeAvailableMoves(Piece* piece, int colour);

// checks is there is an available move
bool checkLegalMoves(int turn);
#endif