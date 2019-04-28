#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE 10
//#define EXIT_FAILURE -1
#define handle_error(msg) \
           do { perror(msg); _exit(EXIT_FAILURE); } while (0)

int main(int argc,char* argv[]){
	char* path = "/tmp/ud_ucase";
        struct sockaddr_un send_addr,my_addr;
	char* buffer;

	if(argc < 2 || strcmp("--help",argv[1]) == 0)
		handle_error("we should enter more than 2 arguments with the string!");

	buffer = malloc(BUFFERSIZE);

        int socketfd = socket(AF_UNIX,SOCK_DGRAM,0);

        if(socketfd == -1)
                handle_error("socketfd failure!");
	
	memset(&send_addr,0,sizeof(struct sockaddr_un));
       	send_addr.sun_family = AF_UNIX;
        strncpy(send_addr.sun_path,path,sizeof(struct sockaddr_un) - 1);

	memset(&my_addr,0,sizeof(struct sockaddr_un));
        my_addr.sun_family = AF_UNIX;
	snprintf(my_addr.sun_path,(size_t)sizeof(my_addr.sun_path),"/tmp/ud_ucase_cl.%ld",(long)getpid());

	if((bind(socketfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr_un))) == -1)
                handle_error("bind failure!");

	size_t msglen,numberBytes;

	for(int i = 1; i < argc ; i++){
	  	msglen = strlen(argv[i]);

		if(sendto(socketfd,argv[i],msglen,0,(struct sockaddr *)&send_addr,
 					(socklen_t)sizeof(struct sockaddr_un)) != msglen){
				handle_error("client sendto failure!");
		}

		numberBytes = recvfrom(socketfd,buffer,BUFFERSIZE,0,NULL,NULL);

		if(numberBytes == -1)
			handle_error("client recvfrom failure!");
		printf("Responds %d: %s\n",i,buffer);
	}	

	free(buffer);
	remove(my_addr.sun_path);
	_exit(EXIT_FAILURE);	
}
