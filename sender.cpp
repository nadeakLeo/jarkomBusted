
#include "bit.h"

using namespace std;

/* CONSTANT */
#define TIMEOUT 1000 //ms

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
string getBitString(Message input) {
  string result = "";
  result += bitset<8>(input.soh).to_string();
  result += bitset<8>(input.seqNum).to_string();
  result += bitset<8>(input.stx).to_string();
  result += bitset<8>(input.data).to_string();
  result += bitset<8>(input.etx).to_string();

  return result;
}

Byte getCheckSum(Message input) {
  string chkSumBit = makeCRC(getBitString(input));

  return ((Byte) bitset<8>(chkSumBit).to_ulong());
}

void addQueueWindow(Byte data, SendWindow *window) {
  int rear = window->rear;
  window->data[rear] = data;
  window->ackStatus[rear] = false;
  window->startTime[rear] = -1;
  window->rear = (rear + 1) % (window->maxsize);
  window->count++;
}

void sendMessage(Byte msgno, Byte data, int sockfd, struct sockaddr_in receiver_addr, int slen){
    Message message;
    message.soh = SOH;
    message.seqNum = msgno;
    message.stx = STX;
    message.data = data;
    message.etx = ETX;

    message.checksum = getCheckSum(message);

    sendto(sockfd, &message, sizeof(Message), 0,
                    (struct sockaddr*)&receiver_addr, slen);
}

void slideWindow(SendWindow *window) {
  window->front = (window->front + 1) % window->maxsize;
  window->count--;
}

#define MAXTRANSBUFF 10
struct sockaddr_in receiver_addr;
int sockfd, slen = sizeof(receiver_addr);
Byte *buffer, lastReceivedChar;
bool *ack;
clock_t startTime[MAXTRANSBUFF];
SendWindow window = {0,0,0, MAXTRANSBUFF, buffer, ack, startTime};

void* receiveResponse(void*){
  //implement checksum later

  Response resp;

  while(true) {
    if (recvfrom(sockfd, &resp, sizeof(Response), 0, (struct sockaddr*)&receiver_addr, (socklen_t*) &slen) == -1)  {
          cout << "Error in receiving data" << endl;
          exit(EXIT_FAILURE);
        }

    if (resp.ack == ACK) {
      cout << "ACK received for sending message with sequence Number :" <<
              resp.seqNum << endl;
      window.ackStatus[resp.seqNum] = true;
    } else {
      //resend
      int seqNum = resp.seqNum;
      int data = window.data[seqNum];

      window.startTime[seqNum] = clock();
      sendMessage(seqNum, data, sockfd, receiver_addr, slen);
      cout << "Sent message with sequence Number - " << seqNum << " : " <<
              data << endl;
    }
  }
};

void* receiveMessage (void*) {
  Byte b;
  while (true) {
    if (recvfrom(sockfd, &b, sizeof(Byte), 0,
        (struct sockaddr*)&receiver_addr, (socklen_t*) &slen) == -1) {
            cout << "Error in receiving data" << endl;
            exit (EXIT_FAILURE);
        }

    if (b == XON) {
      cout << "XON received" << endl;
    } else {
      cout << "XOFF received" << endl;
    }

    lastReceivedChar = b;
  }
};

/* MAIN PROGRAM */
int main(int argc, char **argv) {
  if(argc < 6){
		cout<<"Wrong number of arguments, should receive 4 arguments\n"<<endl;
		cout<<"./transmitter <filename> <windowsize> <buffersize> <destination_ip> <destination_port>"<<endl;
		exit(EXIT_FAILURE);
	}

  char *ip = argv[4], *file = argv[1];
  int port = atoi(argv[5]);
  int windowSize = atoi(argv[2]), bufferSize = atoi(argv[3]);
  buffer = new Byte[bufferSize];
  ack = new bool[bufferSize];
  window = {0,0,0, windowSize, buffer, ack, startTime};

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  receiver_addr.sin_addr.s_addr = inet_addr(ip);
  receiver_addr.sin_family = AF_INET;
  receiver_addr.sin_port = htons(port);

  pthread_t recvResp_thread;
  if (pthread_create(&recvResp_thread, NULL, &receiveResponse, NULL) < 0) {
    cout << "Error Threading" << endl;
    exit(EXIT_FAILURE);
  }

  FILE *f;
  f = fopen(file, "r");

  if (f == NULL) {
    cout << "Error: File not found"<< endl;
    exit(EXIT_FAILURE);
  }

  Byte b;
  int seqNum = 0;
  bool end = false;
  while (true) {
    while (window.count <= window.maxsize/2 && !end) {
      if (lastReceivedChar != XOFF) {
        if (fscanf(f, "%c", &b) == EOF) {
          b = Endfile;
          end = true;
        }
        addQueueWindow(b, &window);
      }
    }

    int i = window.front;
    while (i != window.rear) {
      if (!window.ackStatus[i]) {
        double timeDiff = (double)(clock() - window.startTime[i])/CLOCKS_PER_SEC * 1000;

        if (window.startTime[i] == -1 || timeDiff > TIMEOUT) {
          if (window.startTime[i] != -1) {
            cout << "Time out in processing frame no " << i << endl;
          }

          window.startTime[i] = clock();
          sendMessage(i, window.data[i], sockfd, receiver_addr, slen);
          cout << "Sent message with sequence number-" << i << " : " << window.data[i] << endl;
        }
      } else if (i == window.front && window.ackStatus[i]) {
        slideWindow(&window);
      }
      i = (i+1) % window.maxsize;
    }
    if (end && window.front == window.rear) {
      break;
    }
  }
  fclose(f);

  return 0;
};
