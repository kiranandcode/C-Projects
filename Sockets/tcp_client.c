#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

// Structures for storing addresses
#include <netinet/in.h>


int main(){ 

	// Make the initial socket
	int network_socket;
	network_socket = socket(/* domain = */AF_INET,
		/* type of socket (tcp..) = */SOCK_STREAM,
	  /* protocol used (0 for default)= */ 0);

	// Specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	// use htons to convert a port to network byte order
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;
	
	int connection_status =	
		connect(/* socket = */ network_socket,
	       /* address = */ (struct sockaddr *) &server_address,		  /* sizeof  = */ sizeof(server_address));

	// Check connected okay	
	if(connection_status == -1) {
		fprintf(stderr, "FATAL: Could not Connect to socket - TERMINATING");
		exit(1);
	}


	// recieve data
	char server_response[256];
	recv(network_socket,&server_response, sizeof(server_response), 0);


	// print out the server's reponse
	printf("The server sent %s\n", server_response);


	// close the socket
	close(network_socket);
	return 0;
}
