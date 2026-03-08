#ifndef LEGALINPUT
#define LEGALINPUT

#include <stdbool.h>

#include "constantsandstructs.h"

Piece legalSource(char file, int rank, int colour);
int legalDest(char file, int rank, char fileSource, int rankSource, int colour);

#endif