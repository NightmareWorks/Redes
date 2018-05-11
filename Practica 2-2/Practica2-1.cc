#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

int main(int argc, char** argv) {
	struct addrinfo *res;
	struct addrinfo hints;
	memset((void *) &hints, '\0', sizeof(struct addrinfo));
	
	hints.ai_family = AF_UNSPEC; 


	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	
	if(rc!=0){
		std::cout << "Error getaddrinfo():" << gai_strerror(rc) << std::endl;
		return -1;
	}

	for( struct addrinfo *tmp = res; tmp->ai_next!=NULL; tmp=tmp->ai_next){
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		int error = getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, NI_MAXHOST,
			serv, NI_MAXSERV, NI_NUMERICHOST);
		
		if(error !=0){
			std::cout << "Error getaddrinfo():" << gai_strerror(rc) << std::endl;
			return -1;		
		}
		
		std::cout << host << " "<< tmp->ai_family << " " << tmp->ai_socktype << "\n";
	}

	delete res;
    return 0;
}