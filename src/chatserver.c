/******************************************************************************
 ** Filename: chatserver.c
 ** Author: Tim Robinson
 ** Date created: 7/23/2015
 ** Last modification date: 7/26/2015 8:58 PM PST
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

#define HANDLE "Server"

// This variable allows us to send a message to a client upon a SIGINT signal
// being raised
int fdHolder;

/******************************************************************************
 ** Function: signal_handler
 ** Description: This function defines the behavior of the program once a 
 ** 	SIGINT signal is raised. If a client is connected, a "\quit" message is
 ** 	sent to the client and the program ends.
 ** Parameters: signal number
 ** Preconditions: None
 ** Postconditions: This function finishes with the program ending, a "\quit"
 **	message sent to the connected client.
******************************************************************************/
void signal_handler(int signal) {

	// If the fdHolder has been set to a client file descriptor, send a "\quit"
	// message to the client
	if(fdHolder != -1) {
		printf("\nClosing connection...");
		int closemessage = send(fdHolder, "\\quit", 500, 0);
	}
	printf("\nClosing server...\nGood-bye!\n");

	exit(0);
}


int main(int argc, char *argv[]) {

	fdHolder = -1;

	// These will be used to process a SIGINT signal once raised
	void signal_handler(int signal);
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	// If the SIGINT signal is raised, process the signal using the information
	// provided above
	if(sigaction(SIGINT, &sa, NULL) == -1) {
		perror("server sigaction failure");

		exit(1);
	}

	// This is used to get the user-defined port number from the command line
	char *PORT;

	// This is used to track the success of the getaddrinfo() function
	int status;

	// This will store a file descriptor
	int s;

	// This will store a file descriptor when the connection is established
	int new_fd;

	// This is used to send the 1 as an address while setting the socket options
	int yes=1;

	// This struct will store the client's socket address information
	struct sockaddr_storage their_addr;

	// This will store the address size of the client's socket, used in
	// accepting the connection with the client
	socklen_t addr_size;

	// These structures will hold the address information for our sockets
	struct addrinfo servSet, *res, *p;

	// This will store the client's IP address
	char inaddr[INET6_ADDRSTRLEN];

	// If the format of the program call is not appropriate, an error is shown
	if(argc != 2) {
		fprintf(stderr, "Port must be provided. Use \"./clientserve <port number>\"\n");
		return 1;
	} else {
		PORT = argv[1];
	}

	// This clears all of the data at the servSet structure
	memset(&servSet, 0, sizeof servSet);

	// Defines as using either IPv4 or IPv6
	servSet.ai_family = AF_UNSPEC;

	// Defines as TCP socket
	servSet.ai_socktype = SOCK_STREAM;

	// This allows the IP address to fill in for us, which we will do with the
	// localhost for testing
	servSet.ai_flags = AI_PASSIVE;

	// The "res" variable points to a linked list of addrinfo structs created 
	// using the information in the other variables
	// NULL is used here to use the localhost
	if ((status = getaddrinfo(NULL, PORT, &servSet, &res)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	// Loop through the returned linked list of addrinfo structs
	// Create a socket using the first successful link in the list
	for(p = res; p != NULL; p = p->ai_next) {

		// Try to create a socket using the addrinfo struct, if it is not
		// successful try the next struct in the linked list. 's' is the file
		// descriptor we'll use for the socket
		if((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server socket failure");
			continue;
		}

		// Assign the socket address p->ai_addr to the socket referred to by 's'
		if(bind(s, p->ai_addr, p->ai_addrlen) == -1) {
			close(s);
			perror("server bind failure");
			continue;
		}

		break;
	}

	// Since we have created a socket and have a file descriptor, we do not need
	// the addrinfo struct linked list anymore
	freeaddrinfo(res);

	// Open the port for connections, returns a -1 if listening fails
	if(listen(s, 10) == -1) {
		perror("server listen failure");
		exit(1);
	}

	printf("Server up and waiting on Port %s...\n", PORT);

	// Waiting to accept a new connection
	while(1) {

		// Accept a connection if there is one in the queue, send in address
		// variables that will be populated with the client information
		new_fd = accept(s, (struct sockaddr *)&their_addr, &addr_size);

		fdHolder = new_fd;
		addr_size = sizeof their_addr;

		// If accepting the connection fails
		if(new_fd == -1) {
			perror("server accept failure");
			continue;
		}

		// This puts the IP address as a string in the "inaddr" variable
		// Here, I am assuming that we are using IPv4 to simplify the printing 
		// of the client's IP information
		inet_ntop(AF_INET, &(((struct sockaddr_in*)(struct sockadrr *)&their_addr)->sin_addr), inaddr, sizeof inaddr);
		printf("Server connected to client at %s\n", inaddr);
		printf("Wait for client message and then respond.\n");
		printf("The server and client must alternate in their communication.\n");

		// This will store the incoming message from the client
		char * message = (char *)malloc(sizeof(char) * 1000);
		int received;

		// While the server receives messages successfully 
		while((received = recv(new_fd, message, 1000, 0))) {

			// If the message "\quit" is given from the client
			if(strncmp(message, "\\quit", 3) == 0) {
				printf("Client closing connection...\n");
				printf("Server closing connection...\n");

				// Close the connection
				if(close(new_fd) == 0) {
					printf("Server waiting on Port %s...\n", PORT);
				} else {
					perror("server close failure");
				}

				// Stop receiving messages
				break;
			}

			// Print the client's message. The client sends their message prepended
			// with their selected username
			printf("%s\n", message);

			// Clear the message contents after printing it to prepare for more
			// messages
			message = (char *)malloc(sizeof(char) * 1000);

			// These will be used for the server's message to the client. The
			// server's handle, which is defined above, will prepend their message
			// to the client as well
			char *inputString = (char *)malloc(sizeof(char *) * 1000);
			char *tmpString = (char *)malloc(sizeof(char *) * 1000);
			printf("%s> ", HANDLE);

			// If the server enters text
			if(fgets(inputString, 1000, stdin)) {

				// If the server's message is "\quit"
				if(strncmp(inputString, "\\quit", 5) == 0) {

					// Send a closing message to the client
					int close_message = send(new_fd, inputString, 1000, 0);	

					printf("Server closing connection...\n");
					printf("Client closing connection...\n");

					// Close the connection
					if(close(new_fd) == 0) {
						printf("Server waiting on Port %s...\n", PORT);
					} else {
						perror("server close failure");	
					}
					break;
				}

				// Prepend "<handle>> " to the message using the temporary string
				strcpy(tmpString, HANDLE);
				strcat(tmpString, "> ");
				strcat(tmpString, inputString);

				// Send the message to the client	
				int newmessage = send(new_fd, tmpString, 500, 0); 
			}
			
		}
	}

	return 0;
}