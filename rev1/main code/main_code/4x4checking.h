#ifndef CHECKING
#define CHECKING

#include <stdbool.h>

#include "4x4constantsandstructs.h"

// checks if in check
bool inCheck(int colour);

// checks if castling thtrough check
bool throughCheck(int castle, int colour);

#endif