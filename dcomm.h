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

#define MaxFramesWindow 4
typedef unsigned char Byte;

typedef struct {
  char byte[9];
} Frame;

typedef struct{
	Frame Frames[MaxFramesWindow];
	int statusack[MaxFramesWindow];
	int ukuranWindow;
	int jumlahFrame;
} Window;

int generateChecksum(char* string);
int getIntLength(int i);
Frame compileToFrame(char data, int seqNum);
void decompileFrame(Frame f);

Window inisialisasiWindow();
Window compileToWindow(Frame* frames, int* statusack, int ukuranWindow, int jumlahFrame);


#endif
