#include "dcomm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int generateChecksum (Frame input){
  unsigned long long result = 0;
  char* temp = input.byte;

  unsigned long long gen = 0x131;
  int i = 0;
  do {
    result = result << 8;
    result += (unsigned long long) temp[i];
    i++;
  } while (temp[i] != ETX);

  result = result << 8;

  for (int i = sizeof(long long)*8; i > 8; i--) {
    if ((result >> (i - 1)) & 1) {
      result = result ^ (gen << (i - 9));
    }
  }

  return (int) result;
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
  int numLength = getIntLength(seqNum);
  int crcLength, idx = 0;
  int size = 1 + numLength + 1 + /*Message Length*/1 + 1 + 1 + 1;
  fResult.byte = (char*) malloc (size);
  char num[numLength];
  char* crc;

  fResult.byte[idx] = SOH;
  idx++;

  sprintf(num, "%d", seqNum);
  for (int j = idx; j <= numLength; j++) {
    fResult.byte[j] = num[j-1];
  }
  idx = numLength+1;

  fResult.byte[idx] = STX; idx++;
  fResult.byte[idx] = data; idx++;
  fResult.byte[idx] = ETX; idx++;

  //sisanya checksum
  int checksum = generateChecksum(fResult);
  crcLength = getIntLength(checksum);
  crc = (char*) malloc (crcLength);
  sprintf(crc, "%d", checksum);

  for (int i = 0; i < crcLength; i++) {
    fResult.byte[idx] = crc[i];
    idx++;
  }

  fResult.byte[idx] = '\0';

  free(crc);
  return fResult;
}

void decompileFrame(Frame f, char *data, int *seqNum, int *errCheck) {
  if (f.byte[0] == SOH) {
    int i = 1, numLength=0;
    char* num, crc[33];
    while(f.byte[i] != STX) {
      numLength+=1;
      i++;
    }
    i = 1;
    num = (char *) malloc (numLength);
    while(f.byte[i] != STX) {
      num[i-1] = f.byte[i];
      i++;
    }
    *seqNum = atoi(num);
    i++;
    while(f.byte[i] != ETX) {
      *data = f.byte[i];
      i++;
    }
    i++;
    int n = 0;
    while (f.byte[i] != '\0') {
      crc[n] = f.byte[i];
      i++; n++;
    }

    *errCheck = atoi(crc);

    free(num);
    // free(crc);
  } else {
    printf("[+] Error: Frame Corrupted\n");
  }
}

// int main() {
//   Frame x;
//   x = compileToFrame('z', 2105);
//   decompileFrame(x);
//
//   // printf("%d\n", generateChecksum(x));
// }
//   x = compileToFrame('b', 105);
//   decompileFrame(x);

//   printf("%c\n", (char) 1050);
// }

Window compileToWindow(Frame* Frames, int* statusack, int jumlahFrame){
  Window W;
  int i, sisaukuran;
  i = 0;
  sisaukuran = jumlahFrame*9;
  W.jumlahFrame = 0;
  while((sisaukuran >= 0) && (i < jumlahFrame)){
    W.Frames[i] = Frames[i];
    W.statusack[i] = statusack[i];
    W.jumlahFrame = W.jumlahFrame + 1;
    i++;
    sisaukuran = sisaukuran - 9;
  }
  return W;

  // Frame x;
  // x = compileToFrame('b', 105);
  // W.Frames[1] = x;
}

int semuaACK(Window W){
  int i, semua_ack;
  semua_ack = 0;
  for(i=0; i<W.jumlahFrame; i++){
    if (W.statusack[i] != 1){
      return 0;
    }
  }
  return 1;
}

void gantiStatusAck(Window *W, int posisi){
  if (W->statusack[posisi] == 0){
    W->statusack[posisi] = 1;
  }
  else if (W->statusack[posisi] == 1){
    W->statusack[posisi] = 0;
  }
  else{
    printf("ACK AWAL SALAH\n");
  }
}

int main(){
  Frame Frames[2];
  int statusack[2];
  Frames[0] = compileToFrame('b', 105);
  statusack[0] = 1;

  Window W;
  W = compileToWindow(Frames, statusack, 1);
  printf("%d\n", W.statusack[0]);
  printf("%d\n", W.Frames[0].byte[0]);
  printf("%d\n", semuaACK(W));
  // decompileFrame(W.Frames[0]);
  // printf("%d\n", W.Frames[0].byte[2]);
}
