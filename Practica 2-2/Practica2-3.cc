#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <math.h>

int main(int argc, char** argv) {
	bool exit = false;
	struct addrinfo *result;
	struct addrinfo hints;
	memset((void *) &hints, '\0', sizeof(struct addrinfo));
	struct sockaddr cliente;
	socklen_t cliente_len = sizeof(cliente);
	char host[NI_MAXHOST];
	char serv[NI_MAXSERV];
	char buffer[256];
	
	hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
	hints.ai_family   = AF_INET; // IPv4
	hints.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo(argv[1], argv[2], &hints, &result);
	if(rc!=0){
		std::cout << "Error getaddrinfo():" << gai_strerror(rc) << std::endl;
		return -1;
	}

	int sd = socket(result->ai_family, result->ai_socktype, 0);

    	memset((void*) buffer,'\0', 256);

	buffer[0] = argv[3][0];

	size_t bytesSent = sendto(sd, buffer, 255, 0,result->ai_addr, result->ai_addrlen);
	
	size_t bytes = recvfrom(sd, buffer, bytesSent, 0, &cliente,
	      &cliente_len);
	

	
	getnameinfo(&cliente, cliente_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);

	std::cout<< buffer << "\n";

	freeaddrinfo(result);


    return 0;
}


