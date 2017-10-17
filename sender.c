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

  strcpy(buffer, "Hello Server\n");
  sendto(sockfd, buffer, 256, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  printf("[+] Data send: %s", buffer);
  createFramesFromMessage();
  return 0;
}

void createFramesFromMessage() {
  FILE *f;
  int c, counter = 0, idx = 0;
  char temp[6]; //Message length : 5
  Frame F[10];
  f = fopen("Message.txt","r");

  /* Membaca file */
  while ((c = fgetc(f)) != EOF) {
    temp[counter] = c;
    counter++;

    if (counter == 5) {
      temp[counter] = '\0';

      for (int i = 0; i < 6; i++) {
        F[idx].message[i] = temp[i];
      }
      F[idx].number = idx;

      counter = 0;
      idx++;
    }
  }

  /* End of Frame */
  temp[0] = 26;

  for (int i = 1; i < 6; i++) {
    F[idx].message[i] = ' ';
  }
  F[idx].number = idx;
  idx++;

  printf("%s\n", F[idx-1].message);
}
