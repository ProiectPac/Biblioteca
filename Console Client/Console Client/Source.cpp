#include<iostream>

#include <winsock2.h>	// contains most of the Winsock functions, structures, and definitions
#include <ws2tcpip.h>	// contains newer functions and structures used to retrieve IP addresses
#pragma comment(lib, "Ws2_32.lib")	//  indicates to the linker that the Ws2_32.lib

#include <fstream>
#include "TCPSocket.h"

int main()
{
	TCPSocket::startUp();
	TCPSocket socket;
	socket.ConnectToServer("localhost", 27015);
	int len = 1025;
	char buffer[1025];

	while (true)
	{
		std::cin.getline(buffer, len);

		std::string line(buffer);

		socket.Send(line);

		line = socket.Receive();
		std::cout << line << '\n';

	}
	return 0;
}