#include <stdio.h>
#include <string.h>
#include <err.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdlib.h>
#include "get_num.h"

int main(int argc,char* argv[]){
	long len,offset;
	char *buffer;
	ssize_t numRead,numWriter;

	if(argc < 3 || strcmp(argv[1],"--help") == 0)
		warnx("%s file {r<length>|R<length>|w<string>|s<offset>...}\n",argv[0]);

	int flags = O_CREAT|O_RDWR;
	mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;

	int fd = open(argv[1],flags,mode);

	if(fd == -1)
		errx(1,"the file descriptor is wrong!");

	for(int ap = 2;ap < argc; ap++){
		switch(argv[ap][0]){
			case 'r':
			case 'R':
				len = getLong(&argv[ap][1],GN_ANY_BASE,argv[ap]);		
				buffer = malloc(len);
				if(buffer == NULL)
					errx(1,"malloc error!");

				numRead = read(fd,buffer,len);

				if(numRead == -1)	
					errx(1,"read error!");

				if(numRead == 0){
					warnx("%s has read the end of the file!",argv[0]);
				}else{
				     printf("%s:",argv[ap]);
				     for(int j = 0; j < numRead; j++){
				  	if(argv[ap][0] == 'r')
					   printf("%c",(unsigned char)buffer[j]?buffer[j]:'?');	
					else
					   printf("%02x",(unsigned int)buffer[j]);
					}
					printf("\n");
                                }	
				free(buffer);
				break;
			

			case 's':
				offset = getLong(&argv[ap][1],GN_ANY_BASE,argv[ap]);
				if(lseek(fd,offset,SEEK_SET) == -1)
					errx(1,"lseek error!");

				printf("%s,lseek succeeed!\n",argv[0]);
				break;
				
			case 'w':
				numWriter = write(fd,&argv[ap][1],strlen(&argv[ap][1]));
				if(numWriter == -1)		
					errx(1,"writer error!");
				printf("%s write %ld bytes \n",argv[ap],(long)numWriter);
				break;

			default:
				printf("Argument must start with [rRws]:%s\n",argv[ap]);		
		}	
	}
}
