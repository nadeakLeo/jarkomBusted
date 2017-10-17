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
  char* byte;
} Frame;

int generateChecksum(Frame input);
int getIntLength(int i);
Frame compileToFrame(char data, int seqNum);
void decompileFrame(Frame f, char *data, int *seqNum, int *errCheck);

typedef struct{
	Frame Frames[MaxFramesWindow];
	int statusack[MaxFramesWindow];
	int ukuranWindow;
	int jumlahFrame;
} Window;


Window compileToWindow(Frame* frames, int* statusack, int ukuranWindow, int jumlahFrame);
void gantiStatusAck(Window* W, int posisi);

#endif
