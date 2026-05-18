#ifndef LEGALMOVEANDCAPTURE
#define LEGALMOVEANDCAPTURE

#include "4x4constantsandstructs.h"
// inserts/updates piece info
void changeAvailableMoves(Piece* piece);

// checks is there is an available move
bool checkLegalMoves(int turn);
#endif