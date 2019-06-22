#include "read_line.h"

#define handle_error(msg) \
	   do {perror(msg); exit(EXIT_FAILURE); } while(0)

ssize_t readLine(int fd,void* buffer,size_t n){
	
	char* buf = (char *)buffer;
	ssize_t Readnum;
	size_t toReadnum;
	char ch;

	if(buffer == NULL || n <= 0)
		handle_error("input failure!");		

	toReadnum = 0;
	for(;;){
		Readnum = read(fd,&ch,1);

		if(Readnum == -1){
			if(errno == EINTR){
				continue;
			}else{
				return -1;
			}
		}else if(Readnum == 0){
			if(toReadnum == 0)
				return 0;
			else
				break;	
		}else{
			if(toReadnum < n - 1){
				toReadnum++;
				*(buf++) = ch;
			}

			if(ch == '\n'){
				break;
			}
		}
	}

	*buf = '\0';
	return toReadnum;
}
