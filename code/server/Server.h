#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WS2tcpip.h>
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <fstream>
#include <ctime>
#include <thread>
#include <chrono>

#pragma comment (lib, "ws2_32.lib")

class Server {
protected:
	SOCKET Connection;
	WORD ver;
	WSAData WSData;
	size_t port;
public:
	bool startServer(Server &server, size_t port);
	bool closeServer();
	void winsockInit(Server &server);
	void addictionalInfo();
	void getPort(Server &server);
	friend SOCKET returnSocket(Server& server);
	friend size_t returnPort(Server& server);
};
