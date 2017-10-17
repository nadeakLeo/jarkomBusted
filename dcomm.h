#ifndef DCOMM_H
#define DCOMM_H

#include <string.h>

/* Flow Control */
#define XON (0x11)    //resume transmition
#define XOFF (0x13)   //pause transmition

/* Const, sesuai format segmen yang diberikan */
#define SOH 1
#define STX 2
#define ETX 3
#define ACK 6

typedef unsigned char Byte;

typedef struct {
  char byte[9];
} Frame;

int generateChecksum(char* string);
int getIntLength(int i);
Frame compileToFrame(char data, int seqNum);

#endif
