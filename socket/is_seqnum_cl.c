#include "is_seqnum.h"
#include "read_line.h"

#define handle_error(msg) \
            do {perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc,char *argv[]){
 	  struct addrinfo hint;
          struct addrinfo *result,*rp;
          int cfd;

          char *reqStr;

	  char seqStr[NI_LEN];

	  int numRead;
  
          if(argc < 2 || strcmp(argv[1],"--help") == 0)
                  handle_error("Please use the server-host [sequence]\n");
  
	  memset(&hint,0,sizeof(struct addrinfo));
          hint.ai_addr = NULL;
          hint.ai_canonname = NULL;
          hint.ai_next = NULL;
          hint.ai_family = AF_UNSPEC;
          hint.ai_socktype = SOCK_STREAM;
          //hint.ai_protocol = 0;
          hint.ai_flags = AI_NUMERICSERV;
  
          if(getaddrinfo(argv[1],PORT_NUM,&hint,&result) != 0)
                  handle_error("getaddrinfo failure!");
  
          for(rp = result;rp != NULL;rp = rp->ai_next){
                  cfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
                  
                  if(cfd == -1)
                          continue;
	 	 
                  if(connect(cfd,rp->ai_addr,rp->ai_addrlen) != -1)
	  	 	  break;
 	          		
	          close(cfd);
          }
  
          if(rp == NULL)
                  handle_error("could not connect the server");
	      
          freeaddrinfo(result);		

	  reqStr = (argc > 2) ? argv[2] : "1";
	  if(write(cfd,reqStr,strlen(reqStr)) != (ssize_t)strlen(reqStr))
		  handle_error("write reqStr failure!");

	  if(write(cfd,"\n",1) != 1)
		  handle_error("write newline failure!");

	  numRead = readLine(cfd,seqStr,NI_LEN);

	  if(numRead == -1)
		  handle_error("readLine failure!");

	  if(numRead == 0)
		  handle_error("Unexpected EOF from server!");

	  printf("squence number: %s",seqStr);
	  exit(EXIT_FAILURE);
}
