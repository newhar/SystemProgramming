#include <linux/kernel.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>	
#include <sys/socket.h>	
#include <sys/stat.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>	
#include <pthread.h>    

#define BUFF_SIZE 1024
#define NUM_PORTS 5

#define SERVER_IP "192.168.56.102"

int ports_number[] = {1111,2222,3333,4444,5555}; // 사용될 포트 넘버 5개

void *f_CreateAndRead(void*);

int main() {
  int i;
  pthread_t pthreads[NUM_PORTS];

  // pthread_create by pre_defiend 'ports_number[NUM_PORTS]'
  for (int i=0; i<NUM_PORTS; i++) {
    printf("pthread_create : %d has been created.\n", ports_number[i]);
    if( (pthread_create(&pthreads[i], NULL, f_CreateAndRead, (void *)&ports_number[i])) != 0 ) {
      printf("pthread_create error : %d cannot create!\n",ports_number[i]);
    }
  }

  for (int i=0; i<NUM_PORTS; i++) {
    printf("pthread_join : %d has been joined.\n", ports_number[i]);
    if( (pthread_join(pthreads[i], NULL)) != 0) {
      printf("pthread_join : %d cannot join!",ports_number[i]);
    }
  }

  return;
}

void *f_CreateAndRead(void* arg) {
  int PORT = *((int*) arg);
  int fd;

  struct sockaddr_in server_addr;

  // socket creation
  printf("socket : %d socket has been created./\n", PORT);
  if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
    printf("socket : %d socket cannot open!/\n", PORT);
    exit(0);
  }

  memset(&server_addr, 0x00, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  server_addr.sin_port = htons(PORT);

  // socket connection 
  printf("socket : %d socket has been connected.\n", PORT);
  if ( connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) {
    printf("connect : %d socket cannot to server!\n", PORT);
    exit(0);
  }
  printf("Port %d: Connected", fd);

  // create text file
  char outfile[256];
  char pnum[6];

  strcpy(outfile, "./");
  sprintf(pnum, "%d", PORT);
  strcat(outfile, pnum);
  strcat(outfile, "./txt");

  // file open
  FILE* fp;
  fp = fopen(outfile, "wt+");

  struct timeval tv;
  struct timezone tz;
  struct tm *tm; 

  // read from client
  while(1) {
    char buffer[BUFF_SIZE];
    int read_size = read(fd, buffer, BUFF_SIZE);
  	buffer[read_size] = '\0';

	  gettimeofday(&tv, &tz);
	  tm = localtime(&tv.tv_sec);

	  fprintf(fp, "%d:%02d:%02d.%ld  %d  %s\n", 
		tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec,
		read_size, buffer);
      
	  sleep(1);

    }

    fclose(fp);
    close(fd);

    return NULL;

}