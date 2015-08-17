# Chat Server and Client
## I. File list
chatserver.c&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Chat Server Implementation<br />
chatclient.py&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Chat Client Implementation<br />
README.md&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This file<br />


## II. Program Instructions

#### A. Running the program and establishing a connection

The program is built by running the chat server and then the chat client.
Given the compile syntax of "gcc -o chatserver chatserver.c", the chat server
is run by default with the host IP as the "localhost" or "127.0.0.1" with the
user provided command line argument as the port number, using the syntax:
	
	"./chatserver <port number>"

where the port number used for testing was 53142. This creates a socket on the
localhost at the provided port number. If the arguments provided are not given
in this format, an error is thrown and the user is notified of the correct
syntax.

Upon running successfully, the server remains open waiting for a connection
from a client, which is shown in the console. If a connection is established,
the server user is notified of the connecting IP address and told to wait for
the client's first message.

The chat client is run using the syntax:

 	"python chatclient.py <host name> <port number>"

where the host name used during testing is written as either "localhost" or
specified by the IP address "127.0.0.1". The port number must match the port
number used to establish the socket on the server. If the host name or port 
number do not match the server's established socket, the client user is
notified that the connection was not made and the program exits.

Upon a successful connection to the server, the client is notified that the
connection has occurred at the specified host name and port number.


#### B. Communication between the server and client

The client then must provide a username by which their messages sent to the
server will be identified. This username is prepended to all messages sent by
the client to the server. The message must be 500 characters or less in order
for the program to act as expected. The username must be less than 11 
characters and cannot contain any whitespace.

The client then must provide the first message to the client by typing their 
desired message and hitting <Enter>. Upon this message being sent, the client
must wait for a message from the server in order to send another message to the
server.

The server must wait for the first message from the client in order to respond
with a message of its own. The client and server messages must alternate with 
one another for the program to behave as expected. For non-alternating
messages, the program's function is undefined.


#### C. Closing a connection using messages

If the server sends or receives a message of "\quit", the server closes its 
connection and remains open to receive additional connections from other 
clients. The server user is notified of the order of connection closure (which
helps distinguish if the "\quit" message was sent by the server or the client)
and then is notified the port on which the server remains open.

If the client sends or receives a message of "\quit", the client closes its
connection and the client program exits.


#### D. Closing a connection using SIGINT

If a SIGINT is raised on the server (e.g. receives "Ctrl-C" command), the
server sends a "\quit" message to the client. As above, this "\quit" message
recieved by the client causes the client to close the connection and exit. The
server program then exits as well.

If a SIGINT is raised on the client (e.g. receives "Ctrl-C" command), the
client sends a "\quit" message to the server. As above, this "\quit" message
received by the server causes the server to close the connection and reamin
open for additional connections by other clients. The client program then
exits.

If the SIGINT signal is raised on an application while waiting for a response
from the other (i.e. out-of-order by the alternation scheme), the signaling
program will still send the "\quit" message and exit. This message will be 
processed by the other upon the other's trying to send another message to the
exited program, at which point that message will be lost and the connection
will be closed as specified above. 


## III. References

##### A. Beej's Guide to Network Programming | Using Internet Sockets
Brian "Beej Jorgensen" Hall<br />
http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html

	1. This resource was used for proper syntax and structure for a program
	implementing internet sockets to accomplish network programming tasks.

	2. This resource aided in the understanding, creation, and use of various
	structs involved in network programming in C. This includes the handling of
	addrinfo structs, sockaddr structs, and sockaddr_in structs.

	3. This resource aided in the understanding, creation, and use of various
	functions involved in network programming in C. This includes the handling
	of the getaddrinfo() function, the socket() function, the bind() function,
	the listen() function, the accept() function, the inet_ntop() function, the
	recv() function, the send() function, and the close() function.

	4. This resource aided in the understanding and use of the various structs
	and functions listed in III.A.2 and III.A.3 in conjunction with one another.
	Much of the structure of this program was aided by the information offered
	in this resource, though the program was ultimately built to accomplish the
	operation outlined in II.A-D. 


##### B. Beej's Guide to Unix IPC | Section 3 Signals
Brain "Beej Jorgensen" Hall<br />
http://beej.us/guide/bgipc/output/html/multipage/signals.html

	1. This resource was used for proper syntax and structure for a program
	implementing signal handling.

	2. This resource aided in the understanding, creation, and use of the 
	sigaction struct and the sigaction and sigemptyset functions.

	3. This resource ultimately aided in the understanding of the processing of
	the SIGINT signal raised on the chat server.


##### C.How do I capture SIGINT in Python?
Responding user: Matt J on July 10, 2009, edited by user Tim Tisdall March 11, 2014<br />
http://stackoverflow.com/questions/1112343/how-do-i-capture-sigint-in-python

	1. This resource was used to understand how to register a signal handler
	in Python.

	2. This resource aided in the understanding of the processing of the SIGINT
	signal raised on the chat client.


##### D. CS 372 Lecture #15 | Socket Programming Primer
Oregon State University, Summer 2015<br />
https://courses.ecampus.oregonstate.edu/index.php?video=cs372/15.mp4<br />
&nbsp;&nbsp;&nbsp;(login required)

	1. This resource was used for proper syntax and structure for a program
	implementing internet sockets to accomplish network programming tasks.

	2. This resource aided in the understanding, creation, and use of various
	functions involved in network programming in Python. This includes the 
	handling of the socket() function, the send() function, the recv() function,
	and the close() function.

	3. This resource aided in the understanding and use of the various functions
	listed in III.D.2 in conjunction with one another. Much of the structure of 
	this program was aided by the information offered in this resource, though 
	the program was ultimately built to accomplish the operation outlined in 
	II.A-D.


## IV. Testing Notes

While testing this program using the "localhost" as the server IP address, the 
server and the client must be located on the same server. This means that while
testing on the Oregon State University server, if the chat server is on flip3,
the chat client must also be on flip3. If they are not both on the same server,
the client server cannot use the "localhost" as the IP address.
