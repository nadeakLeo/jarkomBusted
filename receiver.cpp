#include "bit.h"

#define MAXRECVBUFF 10

using namespace std;

typedef struct {
  int front;
  int rear;
  int maxsize;
  Byte *data;
  bool *received;
} RecvWindow;

void sendACK(Byte ack, int sockfd, struct sockaddr_in sender_addr, int slen, int seqNum, int checksum) {
  Response resp;
  resp.ack = ack;
  resp.seqNum = seqNum;
  resp.checksum = checksum;

  if (sendto(sockfd, &resp, sizeof(Response), 0, (struct sockaddr*) &sender_addr, slen) == -1) {
    cout << "Error sending response" << endl;
    exit(EXIT_FAILURE);
  }
}

bool isCheckSumCorrect(Message input) {
  return (bitset<7>(makeCRC(getBitString(input)))) == 0;
}

bool isFrameValid(Message input) {
  // return (input.soh == SOH &&
  //         input.seqNum < MAXRECVBUFF &&
  //         input.stx == STX &&
  //         input.etx == ETX &&
          return(isCheckSumCorrect(input));
}

void shrinkWindow(RecvWindow* window) {
  window->front = (window->front + 1) % (window->maxsize);
}

void extendWindow(RecvWindow* window) {
  window->received[window->rear] = false;
  window->rear = (window->rear + 1) % window->maxsize;
}

void insertIntoProcess(Byte data, QTYPE *queue, int sockfd, struct sockaddr_in sender_addr, int slen) {
  queue->data[queue->rear++] = data;
  queue->rear %= queue->maxsize;
  queue->count++;
}

#define RECEIVEBUFFSIZE 8

Byte rcvBuff[RECEIVEBUFFSIZE];
QTYPE rcvq = {0,0,0,RECEIVEBUFFSIZE,rcvBuff};
QTYPE *rxq = &rcvq;
bool send_xoff = false;

struct sockaddr_in receiver_addr, sender_addr;
int sockfd, byteCounter = 0, byteConsumed = 0, slen = sizeof(sender_addr);
int port;

Byte buffer[MAXRECVBUFF];
bool received[MAXRECVBUFF];
RecvWindow window = {0, MAXRECVBUFF/2 - 1, MAXRECVBUFF, buffer, received};

Byte q_get(QTYPE *queue) {
  Byte *current;

  if (!queue->count) return '\0';

  Byte c = queue -> data[queue->front++];
  queue->front %= queue->maxsize;
  queue->count--;

  return c;
}
#define DELAY 5
void* getBufferData(void*) {
  while(true) {
    q_get(rxq);
    usleep(DELAY *1000);
  }
}

int main(int argc, char **argv) {
  if(argc < 2){
		cout<<"Wrong number of arguments, should receive 1 argument"<<endl;
		cout<<"./receiver [port]"<<endl;
		exit(EXIT_FAILURE);
	}

	Byte c;
	port = atoi(argv[1]);

	sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	receiver_addr.sin_family = AF_INET;
	receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	receiver_addr.sin_port = htons(port);

	if(bind(sockfd, (struct sockaddr *) &receiver_addr, sizeof(receiver_addr)) < 0){
		cout<<"Error binding"<<endl;
		exit(EXIT_FAILURE);
	}
	cout<<"Binding at port:"<<port<<endl;

	pthread_t child_thread;
	if(pthread_create(&child_thread, NULL, &getBufferData, NULL) < 0){
		cout<<"Error creating thread"<<endl;
		exit(EXIT_FAILURE);
	}

	// sliding window protocol
	Message message;
    while(true){
	    if(recvfrom(sockfd, &message, sizeof(Message), 0,
				(struct sockaddr *) &sender_addr, (socklen_t *) &slen) == -1)
		{
	        cout<<"Error receiving byte"<<endl;
	        exit(EXIT_FAILURE);
	    }

	    if(isFrameValid(message) && rcvq.count < RECEIVEBUFFSIZE){
	    	int seqNum = message.seqNum;
	    	Byte data = message.data;
	    	window.data[seqNum] = data;

	    	sendACK(ACK, sockfd, sender_addr, slen, seqNum, message.checksum);
	    	if(data != Endfile){
          cout << "Frame with sequence number : " << seqNum << " received (Data received : )" << data << ")" << endl;
		    	window.received[seqNum] = true;
	    	}
		    else
		    	break;
	    }
	    else if(rcvq.count >= RECEIVEBUFFSIZE){
	    	cout<<"Fail: Buffer Full"<<endl;
	    }
	    else{
	    	cout<<"Fail: Wrong checksum. Frame has error"<<endl;
	    	sendACK(NAK, sockfd, sender_addr, slen, message.seqNum, message.checksum);
	    }

	    for(int i = window.front; i != window.rear; i = (i+1)%window.maxsize){
	    	if(i == window.front && window.received[i]){
	    		insertIntoProcess(window.data[i], rxq, sockfd, sender_addr, slen);
	    		shrinkWindow(&window);
	    		extendWindow(&window);
	    	}
	    }
	}
	while(rxq->count!=0){
		//Do Nothing
	}

  return 0;
}
