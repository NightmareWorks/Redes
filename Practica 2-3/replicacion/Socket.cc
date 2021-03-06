#include "Socket.h"
#include "Serializable.h"

#include <string.h>

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación Socket
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

bool operator== (const Socket &s1, const Socket &s2)
{
	char host1[NI_MAXHOST];
	char server1[NI_MAXSERV];

	char host2[NI_MAXHOST];
	char server2[NI_MAXSERV];

	int error = getnameinfo(&s1.sa,s1.sa_len, host1, NI_MAXHOST, server1, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
			
	error = getnameinfo(&s2.sa,s2.sa_len,host2, NI_MAXHOST, server2, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);


	return s1.sa.sa_family == s2.sa.sa_family && host1 == host2 && server1 == server2;

}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;
}

// ----------------------------------------------------------------------------

Socket::Socket(const char * address, const char * port):sd(-1)
{
    struct addrinfo *result;
    struct addrinfo hints;
    memset((void *) &hints, '\0', sizeof(struct addrinfo));

    //UDP
    hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
    hints.ai_family   = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(address, port, &hints, &result);
    if(rc!=0) {
        std::cout << "Error getaddrinfo():" << gai_strerror(rc) << std::endl;
		throw new std::runtime_error("Error socket");
        //ERROR
    }
    sa = *result->ai_addr; sa_len = result->ai_addrlen;
    sd = socket(result->ai_family, result->ai_socktype, 0);
    if(sd == -1) //ERROR
		throw new std::runtime_error("Error socket");

}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
	obj->to_bin();
	size_t bytesSent = sendto(sock->sd, obj->data(), obj->size(), 0, &sock->sa, sock->sa_len);
	if(bytesSent == -1) return -1;
	return 0;
	
}

// ----------------------------------------------------------------------------

int Socket::recv(char * obj, Socket ** sock)
{
	struct sockaddr cliente;
	socklen_t cliente_len = sizeof(cliente);
	
	size_t bytes = recvfrom(sd, &obj, MAX_MESSAGE_SIZE, 0, &cliente, &cliente_len);
	
	if(bytes == -1) return -1;
	if(sock != 0 ){
		*sock = new Socket(&cliente, cliente_len);
	}
	return 0;

}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
































