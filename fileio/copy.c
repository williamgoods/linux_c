#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc,char *argv[]){
	char* buffer = (char*)malloc(sizeof(char)*100);
	ssize_t size,status;

 	int fd = open(argv[1],O_RDONLY);

	if(fd == -1)
		errx(1,"the file is error!\n");

	int flag = O_CREAT|O_WRONLY|O_TRUNC;
	mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;

	int w_fd = open(argv[2],flag,mode);	

	if(w_fd == -1)
                errx(1,"the file is error!-------------\n");

	while((size = read(fd,buffer,100)) > 0)
		if(write(w_fd,buffer,size) != size)
			errx(1,"the error is in the write!");
	
	if(size == -1)
		errx(1,"the read and write has some error!");

	off_t offset_start = lseek(w_fd,0,SEEK_SET);
	off_t offset_end = lseek(w_fd,0,SEEK_END);	
	off_t offset = offset_end - offset_start;
	
	printf("the file have %d bytes\n",(int)offset);
	

	if(close(fd) == -1)
		errx(1,"close the fd is wrong!");
	if(close(w_fd) == -1)
		errx(1,"close the w_fd wrong!");	
}
