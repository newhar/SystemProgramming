#include <linux/kernel.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>	// typedef
#include <sys/socket.h>	// connect ... functions for socket
#include <sys/stat.h> 
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>	// inet_addr()
#include <pthread.h>    

#define BUFFER_LEN 1024
#define NUM_PORTS 5

#define SERVER_IP "192.168.56.102"


int ports_number[NUM_PORTS] = {1111,2222,3333,4444,5555}; // 사용될 포트 넘버 5개

void *f_ReadFromServer(void*);
void connectToServer(int* fd);

int main() {
  int i;
  int socket_fd[NUM_PORTS];
  pthread_t pthreads[NUM_PORTS];

  //connect to server
  connectToServer(socket_fd);

  // pthread_create by pre_defiend 'ports_number[NUM_PORTS]'
  for (int i=0; i < 5; i++) {
    printf("pthread_create : %d is created.\n", ports_number[i]);
    if( (pthread_create(&pthreads[i], NULL, &f_ReadFromServer, (void *)ports_number[i])) != 0 ) {
      printf("pthread_create error : %d cannot create!\n",ports_number[i]);
    }
  }

  
  
}

void connectToServer(int* fd) {
  struct sockaddr_in server_addr;
	int i;

  for (i = 0; i < NUM_PORTS; i++) {
    // socket creation
    printf("socket : %d socket has been created./\n", ports_number[i]);
    if ( (fd[i] = socket(AF_INET, SOCK_STREAM, 0)) != 0 ) {
      printf("socket : %d socket cannot open!/\n", ports_number[i]);
      exit(0);
    }

    memset(&server_addr, 0x00, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(ports_number[i]);

    // socket connection 
    printf("socket : %d socket has been connected.\n", ports_number[i]);
    if ( connect(fd[i], (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) {
      printf("connect : %d socket cannot to server!\n", ports_number[i]);
      exit(0);
    }
    printf("Port %d: Connected", fd[i]);
  }

  return;
}