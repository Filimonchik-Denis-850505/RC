#include "Server.h"

bool Server::startServer(Server& server, size_t a) {
	SOCKET listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		WSACleanup();
		return 1;
	}
	sockaddr_in client;
	int len = sizeof(sockaddr_in);
	client.sin_family = AF_INET;
	client.sin_port = htons(port + a);
	client.sin_addr.s_addr = (INADDR_ANY);

	bind(listening, (sockaddr*)&client, sizeof(client));

	listen(listening, SOMAXCONN);
	server.Connection = accept(listening, (sockaddr*)&client, &len);
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " Connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " Connected on port " << ntohs(client.sin_port) << std::endl;
	}

	closesocket(listening);
	return 0;
}

bool Server::closeServer() {
	closesocket(Connection);
	WSACleanup();
	std::cout << "Main connection was stopped" << std::endl;
	return 0;
}

void Server::addictionalInfo() {
	Server s;
	s.winsockInit(s);
	char selfName[15];
	gethostname(selfName, sizeof(selfName));
	std::cout << "Host name is " << selfName << "." << std::endl;

	struct hostent* phe = gethostbyname(selfName);
	if (phe == 0) {
		std::cerr << "Yow! Bad host lookup." << std::endl;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		std::cout << "Address " << i << ": " << inet_ntoa(addr) << std::endl;
	}
}

void Server::getPort(Server& s) {

	std::cout << "Enter port [.....]" << std::endl;
	std::cin >> s.port;
}

void Server::winsockInit(Server& server) {
	server.ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &WSData);
	if (wsOk != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
	}
}

SOCKET returnSocket(Server& server) {
	return server.Connection;
}

size_t returnPort(Server& server) {
	return server.port;
}