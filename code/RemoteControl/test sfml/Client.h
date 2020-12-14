#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <tchar.h>
#include <fstream>
#include <WS2tcpip.h>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <string>

#pragma comment (lib, "ws2_32.lib")

class Client {
protected:
	SOCKET connection;
	WORD ver;
	WSAData data;
	std::string ipAdress;
	size_t port = 5000;
public:
	bool startClient(Client& client);
	bool closeClient();
	void winsockInit(Client& client);
	void getPort();
	void getIP();
	friend SOCKET returnSocket(Client& client);
	friend size_t returnPort(Client& client);
	friend std::string returnIP(Client& client);
	friend void setupIP(Client& client, std::string str);
	friend void setupPort(Client& client, size_t port);
};
