#ifndef CHECKING
#define CHECKING

#include <stdbool.h>

#include "constantsandstructs.h"

// checks if in check
bool inCheck(int colour);

// checks if castling thtrough check
bool throughCheck(int castle, int colour);

#endif