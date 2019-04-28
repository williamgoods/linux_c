#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <err.h>
#include <unistd.h>
#include <stdlib.h>

//#define EXIT_FAILURE -1
#define BUFFERSIZE 20
#define handle_error(msg) \
	do{perror(msg); _exit(EXIT_FAILURE); } while(0)

int main(){
	int socket[2];

	if(socketpair(AF_UNIX,SOCK_STREAM,0,socket) == -1)
		handle_error("socketpair failure!");

	pid_t pid;
	char *bufferp;
	bufferp = malloc(BUFFERSIZE);
	char *bufferc;
        bufferc = malloc(BUFFERSIZE);

	ssize_t pread,cread;

	char* wordtop = "child ====> parent\n";
	char* wordtoc = "parent ====> child\n";

	pid = fork();

	//printf("start!\n");

	if(pid == 0){ //this is the child process
		for(int i = 0; i < 5; i++){
			printf("the children start!\n");
			write(socket[0],wordtop,BUFFERSIZE);

			if((cread = read(socket[1],bufferc,BUFFERSIZE)) != -1)
                                if(write(STDOUT_FILENO,bufferc,cread) == -1)  
                                        handle_error("parent write failure!");

                        if(cread == -1)
                                handle_error("parent read failure!");
		}
	}else{ //this is the parent process
		for(int i = 0; i < 5; i++){
			write(socket[1],wordtoc,BUFFERSIZE);

			printf("the parent start!\n");
			if((pread = read(socket[0],bufferp,BUFFERSIZE)) != -1)
				if(write(STDOUT_FILENO,bufferp,pread) == -1)			
					handle_error("parent write failure!");

			if(pread == -1)
				handle_error("parent read failure!");	

			//write(socket[1],wordtoc,BUFFERSIZE);
		}
	}	

	for(int i = 0; i < 2; i++){
		printf("the socket%d is %d\n",i,socket[i]);
		if(close(socket[i]) == -1)
			handle_error("close failure");	
	}

	free(bufferc);
	free(bufferp);
}	
