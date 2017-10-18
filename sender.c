#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bitset>
#include <sys/wait.h>
#include <string.h>
#include "dcomm.h"

/* Tipe Window Pada Sender */
typedef struct {
  int count;
  int front;
  int rear;
  int maxsize;
  Byte *data;
  bool *ackStatus;
  clock_t *startTime;
} SendWindow;

/* FUNCTION AND PROCEDURE */
// void sendMessage(Byte seqNum, Byte data, int sockfd, struct sockaddr)

string getBitString(Message input) {
  string result = "";
  result += bitset<8>(input.soh).to_string();
  result += bitset<8>(input.seqNum).to_string();
  result += bitset<8>(input.stx).to_string();
  result += bitset<8>(input.data).to_string();
  result += bitset<8>(input.etx).to_string();

  return result;
}

string makeCRC(string BitString) {

}

/* MAIN PROGRAM */
int main(int argc, char **argv) {
  Message A;
  A.soh = SOH;
  A.seqNum = 100;
  A.stx = STX;
  A.data = "a";
  A.etx = ETX;
  printf("%s\n", getBitString(A));
  // Frame F[100];
  // if (argc != 2) {
  //   printf("Usage : %s <port> \n", argv[0]);
  //   exit(0);
  // }
  //
  // int port = atoi(argv[1]);
  // int sockfd;
  // struct sockaddr_in serverAddr;
  // char buffer[256];
  // socklen_t addr_size;
  //
  // sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  // memset(&serverAddr, '\0', sizeof(serverAddr));
  //
  // serverAddr.sin_family = AF_INET;
  // serverAddr.sin_port = htons(port);
  // serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //
  // // strcpy(buffer, "Hello Server\n");
  // // sendto(sockfd, buffer, 256, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  //
  // Frame Frames[2];
  // int statusack[2];
  // Frames[0] = compileToFrame('b', 105);
  // statusack[0] = 1;
  //
  // Window W;
  // W = compileToWindow(Frames, statusack, 1);
  // buffer[0] =  W.Frames[0].byte[4];
  // sendto(sockfd, buffer, 256, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  //
  // printf("W.Frames[0].byte[4] = %c\n", W.Frames[0].byte[4]);
  // printf("[+] Data send: %s\n", buffer);
  // createFramesFromMessage();
  return 0;
}
