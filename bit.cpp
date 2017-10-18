#include <iostream>
#include "bit.h"

using namespace std;

string makeCRC(string bitString) {
  static char result[8];
  char crc[7], invert;

  for (int i = 0; i < 7; i++) {
    crc[i] = 0;
  }

  for (int i = 0; i < bitString.length(); i++) {
    invert = ('1' == bitString[i]) ^ crc[6];

    crc[6] = crc[5] ^ invert;
    crc[5] = crc[4];
    crc[4] = crc[3] ^ invert;
    crc[3] = crc[2];
    crc[2] = crc[1] ^ invert;
    crc[1] = crc[0];
    crc[0] = invert;
  }

  for (int i = 0; i < 7; i++) {
    result[6 - i] = crc[i] ? '1' : '0';
  }

  result[7] = 0;

  return result;
}
