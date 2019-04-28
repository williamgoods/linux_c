#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <err.h>
#include <sys/un.h>

int main(){
	char *sockname = "/tmp/socket";	

	int socketfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(socketfd == -1)
		errx(1,"socket failure!");	

	struct sockaddr_un my_addr;

	memset(&my_addr,0,sizeof(struct sockaddr_un));
	my_addr.sun_family = AF_UNIX;
	strncpy(my_addr.sun_path,sockname,sizeof(struct sockaddr_un) - 1);

	if((bind(socketfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr_un))))
			errx(1,"bind failure");
}
