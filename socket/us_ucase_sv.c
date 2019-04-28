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
#include <ctype.h>

//#define EXIT_FAILURE -1
#define BUFFERSIZE 10
#define EXIT_SUCCESS 0
#define BACKLOG 10
#define handle_error(msg) \
           do { perror(msg); _exit(EXIT_FAILURE); } while (0)

int main(){
	char* path = "/tmp/ud_ucase";
	struct sockaddr_un my_addr,recv_addr;

	int socketfd = socket(AF_UNIX,SOCK_DGRAM,0);
	
	if(socketfd == -1)
		handle_error("socketfd failure!");	

	if((remove(path) == -1) && errno != ENOENT)
		errx(1,"remove the %s",path);			
	
	memset(&my_addr,0,sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy((my_addr.sun_path),path,sizeof(struct sockaddr_un) - 1);

	if((bind(socketfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr_un))) == -1)	
		handle_error("bind failure!");

	char *buffer;
	buffer = malloc(BUFFERSIZE);
	socklen_t addr_size;
	ssize_t recv_size,send_size;

	for(;;){
		addr_size = sizeof(struct sockaddr_un);
		recv_size = recvfrom(socketfd,buffer,BUFFERSIZE,0,(struct sockaddr*)&recv_addr,&addr_size);

		printf("\nthis for severs\n");		

		if(recv_size == -1)
			handle_error("server recvfrom failure!");

		printf("Server has receive %ld bytes from the %s",(long)recv_size,recv_addr.sun_path);

		for(int i = 0; i < recv_size; i++)
			*(buffer + i) = toupper((unsigned char)*(buffer + i));

		send_size = sendto(socketfd,buffer,BUFFERSIZE,0,(struct sockaddr*)&recv_addr,addr_size);

		if(send_size == -1)
			handle_error("server sendto error");
	}

	if(close(socketfd) == -1)
		handle_error("close the socket failure!");

	free(buffer);
}	
