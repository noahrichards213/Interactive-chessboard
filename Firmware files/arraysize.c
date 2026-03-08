#include "arraysize.h"

#include <stdio.h>

int arraySize(int* array) {
  if (array == NULL) {
    return 0;
  }
  int i = 0;

  while (array[i] != -1) {
    i++;
  }
  return i;
}
