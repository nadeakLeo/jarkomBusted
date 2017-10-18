#ifndef DCOMM_H
#define DCOMM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <string.h>
#include <cstring>
#include <bitset>
#include <time.h>

using namespace std;
/* Flow Control */
#define XON (0x11)    //resume transmition
#define XOFF (0x13)   //pause transmition

/* Const, sesuai format segmen yang diberikan */
#define SOH 1
#define STX 2
#define ETX 3
#define ACK 6
#define NAK 21
#define Endfile 26

#define MAXBYTESIZE 256

typedef unsigned char Byte;

typedef struct {
  int count;
  int front;
  int rear;
  int maxsize;
  Byte *data;
} QTYPE;

typedef struct {
  Byte soh;
  unsigned int seqNum;
  Byte stx;
  Byte data;
  Byte etx;
  Byte checksum;
} Message;

typedef struct {
  Byte ack;
  unsigned int seqNum;
  Byte checksum;
} Response;

#endif
