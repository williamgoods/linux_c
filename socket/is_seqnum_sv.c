#include "is_seqnum.h"
#include "read_line.h"
#include "lib/get_num.h"

#define handle_error(msg) \
	    do {perror(msg); exit(EXIT_FAILURE); } while(0) 

//#define __USE_MISC
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)

int main(int argc,char* argv[]){
	struct addrinfo hint;
	struct addrinfo *result,*rp;
	int lfd,cfd,optval,reqlen;
	struct sockaddr_storage claaddr;
	socklen_t addrlen;

	char addrStr[ADDRSTRLEN];
	char node[NI_MAXHOST];
	char service[NI_MAXSERV];

	char reqStr[NI_LEN];	

	char seqStr[NI_LEN];
	uint32_t seqNum;

	printf("%d\n",ADDRSTRLEN);

	if(argc > 1 && strcmp(argv[1],"--help") == 0)
		handle_error("Please use the [init-seq-num]\n");

	seqNum = (argc > 1) ? getInt(argv[1],0,"init-seq-num") : 0;

	//ignore the signal step and go to the next step,put it for the late time

	memset(&hint,0,sizeof(struct addrinfo));
	hint.ai_addr = NULL;
	hint.ai_canonname = NULL;
	hint.ai_next = NULL;
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	//hint.ai_protocol = 0;
	hint.ai_flags = AI_NUMERICSERV | AI_PASSIVE;

	if(getaddrinfo(NULL,PORT_NUM,&hint,&result) != 0)
		handle_error("getaddrinfo failure!");

	for(rp = result;rp != NULL;rp = rp->ai_next){
		lfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		
		if(lfd == -1)
			continue;

		if(setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&optval,(socklen_t)sizeof(optval)) == -1)
			handle_error("setsockopt failure!");
		//look for one socket and then close it
		if(bind(lfd,rp->ai_addr,rp->ai_addrlen) == 0)
			break;

		close(lfd);
	}

	if(rp == NULL)
		handle_error("there is no address to bind!");

	if(listen(lfd,BACKLOG) == -1)
		handle_error("listen failure!");

	freeaddrinfo(result);

	for(;;){
		addrlen = sizeof(struct sockaddr_storage);
		cfd = accept(lfd,(struct sockaddr *)&claaddr,&addrlen);

		if(cfd == -1){
			handle_error("accept failure!");
			continue;
		}

		printf("--------------------------------------------------------------------\n");

		if(getnameinfo((struct sockaddr *)&claaddr,addrlen,node,NI_MAXHOST,service,
					NI_MAXSERV,0) == 0){
			snprintf(addrStr,ADDRSTRLEN,"(%s %s)",node,service);
		}else{
			snprintf(addrStr,ADDRSTRLEN,"(?UNKNOWED?)");	
		}

		printf("Connect from %s\n",addrStr);

		if(readLine(cfd,reqStr,NI_LEN) <= 0){
			close(cfd);
			continue;
		}
		
		reqlen = atoi(reqStr);
		if(reqlen < 0){
			close(cfd);
			continue;
		}
		
		snprintf(seqStr,NI_LEN,"%d\n",seqNum);

		if(write(cfd,&seqStr,strlen(seqStr)) != (ssize_t)strlen(seqStr))
			handle_error("write failure!");

		seqNum += reqlen;

		if(close(cfd) == -1)
			handle_error("close failure!");
	}
}
