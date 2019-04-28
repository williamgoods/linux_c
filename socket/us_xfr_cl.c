#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 10
//#define EXIT_FAILURE -1
#define handle_error(msg) \
           do { perror(msg); _exit(EXIT_FAILURE); } while (0)

int main(){
	char* path = "/tmp/socket";
        struct sockaddr_un send_addr;
	char* buffer;
	buffer = malloc(BUFFERSIZE);

        int socketfd = socket(AF_UNIX,SOCK_STREAM,0);

        if(socketfd == -1)
                handle_error("socketfd failure!");
	
	memset(&send_addr,0,sizeof(struct sockaddr_un));
       	send_addr.sun_family = AF_UNIX;
        strncpy((send_addr.sun_path),path,sizeof(struct sockaddr_un) - 1);

	int status = 0;

	if(connect(socketfd,(struct sockaddr*)&send_addr,sizeof(struct sockaddr_un)) == -1)
		handle_error("connect failure!");
	else if(status == 0)
		printf("connect successful!\n");
	
	size_t numbersize = 0;
	while((numbersize = read(STDIN_FILENO,buffer,BUFFERSIZE)) > 0){
		printf("buffer is %s\n",buffer);
		printf("read %ld bytes",(long)numbersize);
		if(write(socketfd,buffer,numbersize) != numbersize)
			handle_error("write failure!");
	}

	if(numbersize == -1)
		handle_error("read failure!");

	free(buffer);
	_exit(EXIT_FAILURE);
}
