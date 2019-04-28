#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>

#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

//#define EXIT_FAILURE -1
#define BUFFERSIZE 10
#define EXIT_SUCCESS 0
#define BACKLOG 10
#define handle_error(msg) \
           do { perror(msg); _exit(EXIT_FAILURE); } while (0)

int main(){
	char* path = "/tmp/socket";
	struct sockaddr_un my_addr;

	int socketfd = socket(AF_UNIX,SOCK_STREAM,0);
	
	if(socketfd == -1)
		handle_error("socketfd failure!");	

	if((remove(path) == -1) && errno != ENOENT)
		errx(1,"remove the %s",path);			
	
	memset(&my_addr,0,sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy((my_addr.sun_path),path,sizeof(struct sockaddr_un) - 1);

	if((bind(socketfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr_un))) == -1)	
		handle_error("bind failure!");

	if(listen(socketfd,BACKLOG) == -1)
		handle_error("listen failure!");	
	
	int request;

	char* buffer;
	buffer = malloc(BUFFERSIZE);
	
	for(;;){
		request = accept(socketfd,NULL,NULL);
		if(request == -1)
			handle_error("accept failure!");

		printf("this is my name!");

		size_t numbersize = 0;
		while((numbersize = read(request,buffer,BUFFERSIZE)) > 0){
			//printf("the buffer is %s\n",buffer);
			if(write(STDOUT_FILENO,buffer,numbersize) != numbersize)
				handle_error("write failure!");
		}
		
		if(numbersize == -1)
			handle_error("read failure!");
		if(close(request) == -1)
			handle_error("close failure!");
  	}

	free(buffer);
}	
