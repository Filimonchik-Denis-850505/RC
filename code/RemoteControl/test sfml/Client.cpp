#include "Client.h"

bool Client::startClient(Client& client)
{
	//create socket
	client.connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connection == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(client.port);
	hint.sin_addr.s_addr = inet_addr(client.ipAdress.c_str());
	int connResult = connect(connection, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(connection);
		WSACleanup();
		return 0;
	}
	return 0;
}

bool Client::closeClient()
{
	closesocket(connection);
	WSACleanup();
	std::cout << "Connection was stopped" << std::endl;
	return 0;
}

void Client::winsockInit(Client& client)
{
	client.ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
	}
}

void Client::getPort()
{
	std::cout << "Enter port [.....]" << std::endl;
	std::cin >> port;
}

void Client::getIP() {
	std::cout << "Enter IP [***.***.***.***]" << std::endl;
	std::cin >> ipAdress;
}

SOCKET returnSocket(Client& client) {
	return client.connection;
}

SOCKET returnPort(Client& client) {
	return client.port;
}

std::string returnIP(Client& client) {
	return client.ipAdress;
}

void setupIP(Client& client, std::string str) {
	client.ipAdress = str;
}

void setupPort(Client& client, size_t a) {
	client.port = a;
}