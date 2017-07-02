#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

// Structures for storing addresses
#include <netinet/in.h>


int main() {


	char server_response[] = "You have reached the server!";


	int server_socket;

	server_socket = socket(
	/* type of protocol = */ AF_INET, 
      /* protocol of socket = */ SOCK_STREAM,
/* protocol (0 for default) = */  0);

	
	// define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to the address
	bind(
		/* socket = */ server_socket,
	/* server address = */ (struct sockaddr *) &server_address,
	/* size of addrss = */ sizeof(server_address));

	// listen for inputs
	listen(server_socket, /* backlog */ 10);

	int client_socket;


	client_socket = accept(server_socket, NULL, NULL);

	send(client_socket, &server_response, sizeof(server_response), 0);


	close(server_socket);

	return 0;
}


