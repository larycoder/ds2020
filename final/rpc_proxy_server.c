/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc_proxy.h"
#include <sys/socket.h> // socket lib
#include <stdio.h> // io lib
#include <string.h> // string lib
#include <netdb.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> // read/write fd
#include <fcntl.h>

#define P_DEBUG 1

#define SA struct sockaddr

static struct sockaddr_in p_addr; // address holder
static struct sockaddr_in6 p_addr6;

int lookup_host (char *host, char* addr_string, int addr_string_len); // DNS resolve

int *
connect_proxy_1_svc(dest_host *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */
	bzero(&p_addr, sizeof(p_addr)); // clean address holder
	bzero(&p_addr6, sizeof(p_addr6));

	// catch Ip from DNS
	char host_ip[100];
	int mode = lookup_host(argp->host, host_ip, 100);
	if(strlen(host_ip) == 0){
		printf("Resolve DNS fail...\n");
		result = -1;
		return &result;
	}
	printf("IP of DNS %s: %s\n", argp->host, host_ip);
	printf("Port %d\n", argp->port); // test

	// assign IP, PORT
	if(mode == 0){
		result = socket(AF_INET, SOCK_STREAM, 0); // open socket fd
		if(result == -1){
			printf("socket creation failed...\n");
			return &result;
		}
		printf("Socket created...\n");

		p_addr.sin_family = AF_INET;
		p_addr.sin_addr.s_addr = inet_addr(host_ip);
		p_addr.sin_port = htons(argp->port);

		// connect the client socket to server socket
		if(connect(result, (SA*)&p_addr, sizeof(p_addr)) != 0){
			result = -1;
			printf("Server connection fail...\n");
			return &result;
		}
	}
	else{
		result = socket(AF_INET6, SOCK_STREAM, 0);
		if(result == -1){
			printf("socket creation failed...\n");
			return &result;
		}
		printf("Socket created...\n");

		p_addr6.sin6_family = AF_INET6;
		p_addr6.sin6_port = htons(argp->port);
		inet_pton(AF_INET6, host_ip, &p_addr6.sin6_addr);

		if(connect(result, (SA *)&p_addr6, sizeof(p_addr6)) != 0){
			result = -1;
			printf("Server connection fail...\n");
			return &result;
		}
	}

	printf("Connected to server...\n");

	return &result;
}

int *
send_proxy_1_svc(p_message *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */
	if(P_DEBUG){
		printf("send length: %d\n", argp->length);
	}
	result = write(argp->fd, argp->ct.content_val, argp->length); // write content to socket

	return &result;
}

p_message *
recv_proxy_1_svc(int *argp, struct svc_req *rqstp)
{
	static p_message result;

	/*
	 * insert server code here
	 */
	static char ct[100];
	content c_ct;
	c_ct.content_val = ct;
	c_ct.content_len = 100;
	result.ct = c_ct;
	result.fd = *argp;

	// read data from server to client
	
	// setup non-blocking r/w
	int flags = fcntl(*argp, F_GETFL, 0);
	fcntl(*argp, F_SETFL, flags | O_NONBLOCK);

	result.length = read(result.fd, result.ct.content_val, sizeof(ct)); // read from socket

	if(P_DEBUG && result.length > 0){
		printf("recv length: %d\n", result.length);
		printf("recv value: ");
		for(int i = 0; i < result.length; i++) printf("%c", result.ct.content_val[i]);
	}

	return &result;
}

int *
close_proxy_1_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */
	printf("CLose socket\n");
	result = close(*argp); // close fd
	return &result;
}

// Resolve DNS request
int
lookup_host (char *host, char *addr_string, int addr_string_len)
{
	// clean holder of DNS resolved addr
	bzero(addr_string, addr_string_len);
	addr_string[0] = '\0';

	// DNS resolve code
  struct addrinfo hints, *res;
  int errcode;
  char addrstr[100];
  void *ptr;
	int mode;

  memset (&hints, 0, sizeof (hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;

  errcode = getaddrinfo (host, NULL, &hints, &res);
  if (errcode != 0)
    {
      perror ("getaddrinfo");
      return -1;
    }

  printf ("Host: %s\n", host);
  while (res)
	{
		inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

		switch(res->ai_family)
		{
			case AF_INET:
				ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
				mode = 0;
				break;
			case AF_INET6:
				ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
				mode = 1;
				break;
		}
		
		// test - get Ipv4 only
		if(mode == 0){
			inet_ntop (res->ai_family, ptr, addrstr, 100);
			memcpy(addr_string, addrstr, strlen(addrstr));
			return mode;
		}
		res = res->ai_next;
	}

	return 0;
}