#include <iostream>
#include "parse.hpp"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//isdigit
#include <cctype>

#define ERROR 1

bool is_num(char *s)
{
	int i = 0;
	if (s[i] == '-' || !std::isdigit(s[i++]))
		return (false);
	while (s[i] && std::isdigit(s[i]))
		i++;
	if (s[i] == '\0')
		return (true);
	return (false);
}

#define MAX_CLIENTS 10
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "input ./ircserv <port> <password>" << '\n';
		return (ERROR);
	}
	if (!is_num(argv[1]))
	{
		std::cout << "input positive integer" << '\n';
		return (ERROR);
	}
	Server server(argv[1], argv[2]);
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	server.set_socket(server_fd);
	if (server_fd < 0)
	{
		std::cout << "socket error" << '\n';
		return (1);
	}
	int optvalue = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));
	
	sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(std::atoi(argv[1]));

	// 소켓 바인딩
	int bindResult = bind(server_fd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
	if (bindResult == -1)
	{
		perror("Failed to bind");
		return (1);
	}
	if (listen(server_fd, MAX_CLIENTS) == -1) {
		perror("Failed to listen for connections");
		return 1;
	}

	struct pollfd fds[MAX_CLIENTS + 1]; // +1 for the server socket
	fds[0].fd = server_fd;
	fds[0].events = POLLIN;
	for (int i = 1; i < MAX_CLIENTS + 1; ++i)
		fds[i].fd = -1;
	
	int monitoring = 1;
	int i;
	while (1)
	{
		// Wait for events on file descriptors
		int ret = poll(fds, monitoring, -1);
		if (ret == -1) 
		{
			perror("Error in poll");
			break;
		}

		// Check for events on server socket
		if (fds[0].revents & POLLIN)
		{
			struct sockaddr_in clientAddress;
			socklen_t clientAddressLength = sizeof(clientAddress);
			int newClientSocket = accept(server_fd, 
				(struct sockaddr*)&clientAddress, &clientAddressLength);
			if (newClientSocket == -1)
			{
				perror("Failed to accept client connection");
				continue;
			}
			Client *cli = new Client;
			cli->cli_sockaddr = clientAddress;
			cli->set_socket(newClientSocket);
			cli->cli_sockaddr_l = clientAddressLength;
			server.cli_vector.push_back(cli);

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
				if (i + 1 > monitoring) 
					monitoring = i + 1;
				printf("New client connected\n");
			}
		}

		// Check for events on client sockets
		for (i = 1; i < monitoring; ++i) 
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
				{
					parse(buffer, server.cli_vector[i - 1], server);
				}
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
	close(server_fd);




	return 0;
}
