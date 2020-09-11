#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "pthread.h"

#define BUF_LEN 1024



void *threadRoutine(void *argumentPointer);




int main(int argc,char *argv[])
{

	pthread_t threadID;
	int i;
	printf("create thread!\n");

	for(i=4444;i<=8888;i+=1111)
	{
		pthread_create(&threadID,NULL,threadRoutine,(void*)&i);
	}
	
	printf("main thread is waiting!\n");
	
	pthread_join(threadID,NULL);
	
	printf("main thread finished waiting!\n");

}


void *threadRoutine(void *argumentPointer)
{
	int argument;
	argument = *((int *)argumentPointer);


	char buffer[BUF_LEN];
	struct sockaddr_in server_addr;
	int server_fd;
	int len;


	//socket creation

	if((server_fd = socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("server : cant't open stream socket\n");
  		exit(0);
    }
	
	//connect

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(argument);
	server_addr.sin_addr.s_addr = inet_addr("argv[1]");

	if(connect(server_fd, (struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		printf("connection failed");
		exit(0);
	}

	//read 

	while((len = read(server_fd,buffer,BUF_LEN))>0)
	{
		buffer[len]=NULL;
		printf("port number : %s\n",argument);
		printf("%s\n",buffer);
	}


	//close

	close(server_fd);
}
