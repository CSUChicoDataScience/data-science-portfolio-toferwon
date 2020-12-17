// Programming Assignment 1 for EECE 446
// Author: Kris Selvidge
// Group 1, Partner Amintas Major
// Inspired by code class example http-client.c

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>

#define MAX_LINE 256

int lookup_and_connect( const char *host, const char *service );

int main(int argc, char *argv[]) {

	if(argc != 4) {
	printf("Usage: p-counter chunkSize host http:0|1|2|3\n");
		return(0);
	}
	int chunkSize = atoi(argv[1]);
	if(chunkSize < 4 || chunkSize > 1000){
		printf("chunkSize must be between 4 and 1000\n");
		return(0);
	}
	char *address;
	//address = "www.ecst.csuchico.edu"; converted to ip manually through nslookup
	address = argv[2];
	int protocol = atoi(argv[3]);
	int retransmits = 0;
	char request[] = "GET / HTTP/1.0\r\n\r\n";
	char httpv[] = "HTTP/1.0";
	if(protocol == 1) {
		strcpy(request,"GET / HTTP/1.1\r\n\r\n");
		strcpy(httpv,"HTTP/1.1");
	} 
	if(protocol == 2) {
		strcpy(request,"GET / HTTP/2\r\n\r\n");
		strcpy(httpv,"HTTP/2");
	} 
	if(protocol == 3) {
		strcpy(request,"GET / HTTP/3\r\n\r\n");
		strcpy(httpv,"HTTP/3");
	} 
	clock_t begin, end;
	double connect_time,send_time,parse_time;


	int client_socket;
//	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// connect to an address
//	struct sockaddr_in remote_address;
//	remote_address.sin_family = AF_INET;
//	remote_address.sin_port = htons(80);
//	inet_aton(address, (struct in_addr *) &remote_address.sin_addr.s_addr);

	begin = clock();

	/* Lookup IP and connect to server */
	if ( ( client_socket = lookup_and_connect( argv[2],"80" ) ) < 0 ) {
		exit( 1 );
	}

	//connect(client_socket, (struct sockaddr *) &remote_address, sizeof(remote_address));
	end = clock();
	connect_time = (double)(end - begin) / CLOCKS_PER_SEC;


	char response[chunkSize];


	begin = clock();
	send(client_socket, request, sizeof(request), 0);
	end = clock();
	send_time = (double)(end - begin) / CLOCKS_PER_SEC;
	int tSize = 0, btSize = 0,ctSize = 0, bSize = 0;
	int l = chunkSize;
	int bufSize = 0;
	int ob = 0;
	while(bufSize==0 || ob != bufSize) {
		sleep(0.3);
		ob = bufSize;
		ioctl(client_socket,FIONREAD,&bufSize);
	}

	begin = clock();
	while(l!=0){
		btSize = 0;
		ctSize = 0;
		l = recv(client_socket, &response, sizeof(response), 0);
		if(l!=chunkSize) {
			retransmits++;
		}
		for(int i=0;i<l;i++){
			bSize++;
//			if(bSize < 100) {
//				printf("%c",response[i]);
//			}
			if(response[i]=='<'){
				btSize++; 
			}
			if(response[i]=='>'){
				ctSize++;
			}
//			if(response[i]=='<' && i < (l-2)){
//				if(response[i+1]=='p' && response[i+2]=='>'){
//					pSize++;
//				}
//			}
		}
		if (btSize > ctSize) {
			btSize = ctSize;
		}
		tSize += btSize;
	}

	end = clock();
	parse_time = (double)(end - begin) / CLOCKS_PER_SEC;
	
	

	printf("192.168.0.1,%s,%s,%i,%i,%i,%i,%f,%f,%f\n",address,httpv,chunkSize,tSize,bSize,retransmits,connect_time,send_time,parse_time);
	close(client_socket);

	return 0;


}


int lookup_and_connect( const char *host, const char *service ) {
	struct addrinfo hints;
	struct addrinfo *rp, *result;
	int s;

	/* Translate host name into peer's IP address */
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if ( ( s = getaddrinfo( host, service, &hints, &result ) ) != 0 ) {
		fprintf( stderr, "file-client: getaddrinfo: %s\n", gai_strerror( s ) );
		return -1;
	}

	/* Iterate through the address list and try to connect */
	for ( rp = result; rp != NULL; rp = rp->ai_next ) {
		if ( ( s = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 ) {
			continue;
		}

		if ( connect( s, rp->ai_addr, rp->ai_addrlen ) != -1 ) {
			break;
		}

		close( s );
	}
	if ( rp == NULL ) {
		perror( "file-client: connect" );
		return -1;
	}
	freeaddrinfo( result );

	return s;
}
