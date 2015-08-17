#
## Filename: chatclient.py
## Author: Tim Robinson
## Date created: 7/23/2015
## Last modification date: 7/26/2015 8:58 PM PST
# 

from socket import *
import sys
import signal

# This will allow the user to tell the server that to close the connection upon
# receiving a SIGINT
def signal_handler(signal, frame):
	print("Closing connection...")
	print("Closing client...")

	# The program will only run if a clientSocket has been established,
	# so we know we can safely try to send data to the server here
	# Sending the "\quit" message will let the server know that the client has
	# closed their connection
	clientSocket.send("\quit")
	sys.exit(0)

# This lets the program know use the signal handler function when a SIGINT is
# given by the user
signal.signal(signal.SIGINT, signal_handler)

# This checks to see if the command line syntax used to create this program is
# correct
if(len(sys.argv) != 3):
	print("Incorrect syntax! Use \"python chatclient.py <server host name> <port number>\"")
	sys.exit(0)
else:
	ip_address = sys.argv[1]
	serverPort = int(sys.argv[2])

# Create a socket that will use TCP 
clientSocket = socket(AF_INET, SOCK_STREAM)
server_address = (ip_address, serverPort)

# Try to connect with the server at the address above 
try:
	clientSocket.connect(server_address)
except:
	print "No go on the connection..."
	sys.exit()

print "Connection established with " + ip_address + " on port " + str(serverPort)

# This validates the user's chosen username. The username must be at most 10
# characters and must contain no whitespace
while True:
	handle = raw_input('What\'s your username? ')
	if (len(handle) < 11) and ((' ' in handle) == False):
		break;
	else:
		print "Username is at most 10 characters, no whitespace allowed"

# Send the initial message and then wait for a response
while True:
	sentence = raw_input(str(handle) + '> ')

	# If the client enters "\quit" as a message, send the message to the server
	# and close the connection. 
	if sentence == "\\quit":
		clientSocket.send(sentence)
		print "Client closing connection..."
		print "Server closing connection..."
		clientSocket.close()
		break

	# Prepend "<handle>: " to the client's message to be sent
	sentence = handle + "> " + sentence

	# Send the message to the server
	clientSocket.send(sentence)

	# Clear the data in the variable storing received messages
	newincoming = ''

	# Get receive messages at the socket from the server, store the message
	# in the "newincoming" variable
	newincoming = clientSocket.recv(1024)

	# If the client receives the message "\quit", close the connection
	if newincoming[:5] == "\\quit":
		print "Server closing connection..."
		print "Client closing connection..."
		clientSocket.close()
		break
	
	# Print the message
	else:
		sys.stdout.write(newincoming)