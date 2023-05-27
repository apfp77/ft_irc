#include <iostream>
// #include "Server.hpp"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

/*
	./ircserv 4242 4242
*/

#include <poll.h>

#define MAX_CLIENTS 10

int main(int argc, char const **argv) 
{
	if (argc != 3)
	{
		std::cout << "port password error" << '\n';
		return (1);
	}
	int serverSocket;//, clientSockets[MAX_CLIENTS];
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength;
	struct pollfd fds[MAX_CLIENTS + 1]; // +1 for the server socket
	int nfds, i, ret;

	// Create server socket
	struct addrinfo serverAddress;
	struct addrinfo *_servinfo;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.ai_family = AF_INET;
	serverAddress.ai_socktype = SOCK_STREAM;
	serverAddress.ai_flags = AI_PASSIVE;	 
	getaddrinfo(NULL, argv[1], &serverAddress, &_servinfo);

	serverSocket = socket(_servinfo->ai_family, _servinfo->ai_socktype, _servinfo->ai_protocol);
	if (serverSocket == -1) {
		perror("Failed to create server socket");
		return 1;
	}
	int optvalue = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));
	bind(serverSocket, _servinfo->ai_addr, _servinfo->ai_addrlen);

	// Listen for incoming connections
	if (listen(serverSocket, MAX_CLIENTS) == -1) {
		perror("Failed to listen for connections");
		return 1;
	}

	// Initialize the fds array
	fds[0].fd = serverSocket;
	fds[0].events = POLLIN;
	for (i = 1; i < MAX_CLIENTS + 1; ++i)
		fds[i].fd = -1;

	nfds = 1; // Initial number of file descriptors to monitor

	while (1) 
	{
		// Wait for events on file descriptors
		ret = poll(fds, nfds, -1);
		if (ret == -1) 
		{
			perror("Error in poll");
			break;
		}

		// Check for events on server socket
		if (fds[0].revents & POLLIN) 
		{
			// Accept incoming connection
			clientAddressLength = sizeof(clientAddress);
			int newClientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
			if (newClientSocket == -1)
			{
				perror("Failed to accept client connection");
				continue;
			}

			// Add new client socket to the fds array
			for (i = 1; i < MAX_CLIENTS + 1; ++i) 
			{
				if (fds[i].fd == -1) 
				{
					fds[i].fd = newClientSocket;
					fds[i].events = POLLIN;
					break;
				}
			}
			if (i == MAX_CLIENTS + 1) 
			{
				fprintf(stderr, "Too many clients, connection rejected\n");
				close(newClientSocket);
			}
			else 
			{
				if (i + 1 > nfds) 
						nfds = i + 1;
				printf("New client connected\n");
			}
		}

		// Check for events on client sockets
		for (i = 1; i < nfds; ++i) 
		{
			if (fds[i].revents & POLLIN) 
			{
				char buffer[1024];
				memset(&buffer, 0, sizeof(buffer));
				ssize_t bytesRead = recv(fds[i].fd, buffer, 1024, 0);
				if (bytesRead <= 0) 
				{
					if (bytesRead == 0)
						printf("Client disconnected\n");
					else 
						perror("Error in recv");
					close(fds[i].fd);
					fds[i].fd = -1;
				}
				else 
					printf("%s\n", buffer);
			}
		}
	}

	// Close all client sockets
	for (i = 1; i < MAX_CLIENTS + 1; ++i) {
		if (fds[i].fd != -1) {
			close(fds[i].fd);
		}
	}

	// Close server socket
	close(serverSocket);

	return 0;
}