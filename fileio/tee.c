#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>

#define bool int

int main(int argc,char* argv[]){
	char* buffer;
	size_t size;
	size_t number = 0;
	buffer = malloc(1);

	if(buffer == NULL)
		errx(1,"buffer alloc error!");

	while((size = read(STDIN_FILENO,(buffer + (number++)),1)) > 0){
		buffer = (char*)realloc(buffer,number);
		if(buffer == NULL)
			errx(1,"buffer realloc error!");
	}

	printf("%s",buffer);

	int flag = O_CREAT | O_RDWR;
	mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

	int location = 1;
	bool choose = 0;

	for(int ap = 1; ap < argc ; ap++)
		if(strcmp(argv[ap],"-a") == 0){
			choose = 1;
			location = ap + 1;
		}

	if((location != 1) && choose){
		flag = O_CREAT | O_RDWR | O_APPEND;
	}

	int fd;
	
	printf("%ld",(long)sizeof(buffer));

	for(;location < argc;location++)
		fd = open(argv[location],flag,mode);
		size_t w_status = write(fd,buffer,number-1);
		if(w_status == -1)
			errx(1,"write step is error!");
		if(close(fd) == -1)
			errx(1,"close the fd is error!");

	free(buffer);
}
