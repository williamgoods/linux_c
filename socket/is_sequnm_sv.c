#include "is_seqnum.h"

#define handle_error(msg) \
	    do {perror(msg); exit(EXIT_FAILURE); } while(0) 

int main(int argc,char* argv[]){
	struct addrinfo hint;
	struct addrinfo *result,*rp;

	int lfd;

	if(argc > 1 && strcmp(argv[1],"--help") == 0)
		handle_error("Please use the [init-seq-num]\n");

	//ignore the signal step and go to the next step,put it for the late time

	memset(&hint,0,sizeof(struct addrinfo));
	hint.ai_addr = NULL;
	hint.ai_canonname = NULL;
	hint.ai_next = NULL;
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_flags = AI_NUMERICSERV | AI_PASSIVE;

	if(getaddrinfo(NULL,PORT_NUM,&hint,&result) != 0)
		handle_error("getaddrinfo failure!");

	for(rp = result;rp != NULL;rp = rp->ai_next){
		lfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);		
		if(lfd == -1)
			handle_error("socket failure!");

			
	}		
}
