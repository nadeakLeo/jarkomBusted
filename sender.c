#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dcomm.h"

void createFramesFromMessage();

int main(int argc, char **argv) {
  Frame F[100];
  if (argc != 2) {
    printf("Usage : %s <port> \n", argv[0]);
    exit(0);
  }

  int port = atoi(argv[1]);
  int sockfd;
  struct sockaddr_in serverAddr;
  char buffer[256];
  socklen_t addr_size;

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof(serverAddr));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // strcpy(buffer, "Hello Server\n");
  // sendto(sockfd, buffer, 256, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));  

  Frame Frames[2];
  int statusack[2];
  Frames[0] = compileToFrame('b', 105);
  statusack[0] = 1;

  Window W;
  W = compileToWindow(Frames, statusack, 1);
  strcpy(buffer, W.Frames[0].byte[4]);
  sendto(sockfd, buffer, 256, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

  printf("W.Frames[0].byte[4] = %c\n", W.Frames[0].byte[4]);
  printf("[+] Data send: %s\n", buffer);
  createFramesFromMessage();
  return 0;
}

void createFramesFromMessage(Frame (*F)[100]) {
  FILE *f;
  int c, counter = 0, idx = 0;
  char temp[100]; //Message length : 5
  f = fopen("Message.txt","r");

  /* Membaca file */
  while ((c = fgetc(f)) != EOF) {
    temp[counter] = c;
    counter++;
    (*F)[counter] = compileToFrame(temp[counter], counter);
  }
}
