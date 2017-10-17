#include "dcomm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int generateChecksum (char* string){

}

int getIntLength(int x) {
  int n = 0;
  do {
    n++;
    x /= 10;
  } while (x);

  return n;
}

Frame compileToFrame(char data, int seqNum) {
  Frame fResult;
  fResult.byte[0] = SOH;
  // char num[getIntLength(seqNum)];
  // for (int i = 4; i > getIntLength(seqNum); i--) {
  //   fResult.byte[i] = 0;
  // }
  //
  // sprintf(num, "%d", seqNum);
  // for (int j = 1; j <= getIntLength(seqNum); j++) {
  //   fResult.byte[j] = num[j-1];
  // }

  fResult.byte[5] = STX;
  fResult.byte[6] = data;
  fResult.byte[7] = ETX;

  return fResult;
}

void decompileFrame(Frame f) {
  if (f.byte[0] == SOH) {
    int i = 1;
    char num[4];
    char msg;
    while(f.byte[i] != STX) {
      num[i-1] = f.byte[i];
      i++;
    }

    int seqNumber = atoi(num);
    i++;
    while(f.byte[i] != ETX) {
      msg = f.byte[i];
      i++;
    }
    printf("Sequence Number : %s\n", num);
    printf("Data : %c\n", msg);
  } else {
    printf("[+] Error: Frame Corrupted\n");
  }
}

int main() {
  Frame x;
  x = compileToFrame('b', 105);
  decompileFrame(x);

  printf("%c\n", (char) 1050);
}
