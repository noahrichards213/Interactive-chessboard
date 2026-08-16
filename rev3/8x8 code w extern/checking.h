#ifndef CHECKING
#define CHECKING

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "constantsandstructs.h"

// checks if in check
bool inCheck(int colour);

// checks if castling thtrough check
bool throughCheck(int castle, int colour);

#ifdef __cplusplus
}
#endif

#endif