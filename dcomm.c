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

// int main() {
//   Frame x;
//   x = compileToFrame('b', 105);
//   decompileFrame(x);

//   printf("%c\n", (char) 1050);
// }

Window inisialisasiWindow(Frame* Frames, int* statusack, int ukuranWindow, int jumlahFrame){
  Window W;
  int i, sisaukuran;
  i = 0;
  sisaukuran = jumlahFrame;
  W.jumlahFrame = 0;
  while((sisaukuran >= 0) && (i < jumlahFrame)){
    W.Frames[i] = Frames[i];
    W.statusack[i] = statusack[i];
    W.ukuranWindow = ukuranWindow;
    W.jumlahFrame = W.jumlahFrame + 1;
    i++;
    sisaukuran = sisaukuran - 9;
  }
  return W;

  // Frame x;
  // x = compileToFrame('b', 105);
  // W.Frames[1] = x;
}

int main(){
  Frame Frames[2];
  int statusack[2];
  Frames[0] = compileToFrame('b', 105);
  statusack[0] = 1;

  Window W;
  W = inisialisasiWindow(Frames, statusack, 2, 1);
  printf("%d\n", W.statusack[0]);
  printf("%d\n", W.Frames[0].byte[0]);
  // decompileFrame(W.Frames[0]);
  // printf("%d\n", W.Frames[0].byte[2]);
}