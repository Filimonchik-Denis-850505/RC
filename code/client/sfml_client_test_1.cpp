#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <tchar.h>
#include <fstream>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <tchar.h>
#include "Base64.h"
#include "Client.h"

#pragma comment (lib, "ws2_32.lib")

using namespace sf;

void createAlphaMat(cv::Mat& mat)
{
	CV_Assert(mat.channels() == 4);
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			cv::Vec4b& bgra = mat.at<cv::Vec4b>(i, j);
			bgra[0] = UCHAR_MAX; // Blue
			bgra[1] = cv::saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
			bgra[2] = cv::saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
			bgra[3] = cv::saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2])); // Alpha
		}
	}
}

void stringToJPG(char *buf, int n)
{
	char *buff= new char[n];
	ZeroMemory(buff, n);
	strcpy(buff, buf);
	std::string dec_jpg = base64_decode(buff);
	std::vector<uchar> data(dec_jpg.begin(), dec_jpg.end());
	cv::Mat img1 = cv::imdecode(cv::Mat(data), 5);
	if (img1.empty()) 
		return;
	cv::Mat mat(768, 1366, CV_8UC4);
	createAlphaMat(mat);

	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	imwrite("filename.jpg", img1, compression_params);
}

Sprite loadPic() {
	Texture pic;
	pic.loadFromFile("filename.jpg");
	Sprite mainPic;
	mainPic.setTexture(pic);
	mainPic.setPosition(0, 0);
	return mainPic;
}

void drawing(RenderWindow &window1, int a, int b) {
	Texture pic;
	pic.loadFromFile("filename.jpg");
	Sprite mainPic;
	mainPic.setTexture(pic);
	mainPic.setPosition(0, 0);
	mainPic.setScale(0.94f, 0.94f);
	window1.clear();
	window1.draw(mainPic);
	window1.display();
}

void func(SOCKET sock, double a, double b, bool l, bool r) {
	std::wcout << "start" << std::endl;
	while (1) {
		int countX, countY;
		double x, y;
		char bufX[50], bufY[50], trash[50];
		std::string strX, strY;
		std::stringstream strXX, strYY;
		POINT curpos;
		GetCursorPos(&curpos);
		x = curpos.x;
		y = curpos.y;
		x = x * a;
		y = y * b;
		strXX << x;
		strYY << y;
		strXX >> strX;
		strYY >> strY;
		strcpy(bufX, strX.c_str());
		strcpy(bufY, strY.c_str());
		countX = strX.size();
		countY = strY.size();
		int a = 0;
		if (Mouse::isButtonPressed(Mouse::Left)) { a = 1; }
		if (Mouse::isButtonPressed(Mouse::Right)) {	a = 2; }
		switch (a)
		{
		case 1:
			Sleep(25);
			send(sock, bufX, countX, 0);
			Sleep(25);
			recv(sock, trash, 2, 0);
			Sleep(25);
			send(sock, bufY, countY, 0);
			Sleep(25);
			recv(sock, trash, 2, 0);
			Sleep(25);
			send(sock, "1", 2, 0);
			break;
		case 2:
			Sleep(25);
			send(sock, bufX, countX, 0);
			Sleep(25);
			recv(sock, trash, 2, 0);
			Sleep(25);
			send(sock, bufY, countY, 0);
			Sleep(25);
			recv(sock, trash, 2, 0);
			Sleep(25);
			send(sock, "2", 2, 0);
			break;
		default:
			Sleep(25);
			send(sock, bufX, countX, 0);
			Sleep(25);
			recv(sock, trash, 2, 0);
			Sleep(25);
			send(sock, bufY, countY, 0);
			Sleep(25);
			recv(sock, trash, 2, 0);
			Sleep(25);
			send(sock, "0", 2, 0);
			break;
		}
		ZeroMemory(trash, 50);
		ZeroMemory(bufY, 50);
		ZeroMemory(bufX, 50);
	} 
}

double takeSize(SOCKET s1, int x) {
	char buf[10];
	double si, s;
	send(s1, "1", 2, 0);
	recv(s1, buf, 10, 0);
	s = atoi(buf);
	si = s / x;
	std::cout << si << std::endl;
	return si;
}

int main()
{
	std::string ipAddress;
	SOCKET sock, control;
	Client reciever, controller;
	size_t port;
	int scrX = GetSystemMetrics(SM_CXSCREEN);
	int scrY = GetSystemMetrics(SM_CYSCREEN);
	double a, b;
	bool l = false, r = false, check = true;
	int quest;
	do {
		reciever.getIP();
		reciever.getPort();
		do {
			    reciever.winsockInit(reciever);
				reciever.startClient(reciever);
				sock = returnSocket(reciever);
			a = takeSize(sock, scrX);
			b = takeSize(sock, scrY);
			port = returnPort(reciever);
			ipAddress = returnIP(reciever);
				setupIP(controller, ipAddress);
				setupPort(controller, port + 1);
				controller.winsockInit(controller);
				controller.startClient(controller);
			std::cout << "3";
			control = returnSocket(controller);
			std::cout << "4";
			RenderWindow window1(VideoMode(scrX, scrY), "CLIENT", Style::Fullscreen);
			char* buf = new char[500000], buff[2] = "1";
			std::thread thr(func, control, a, b, l, r);
			while (1)
			{
				Event event;
				while (window1.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						window1.close();
				}
				
				if (Keyboard::isKeyPressed(Keyboard::Escape)) { window1.close(); }
				ZeroMemory(buf, 500000);
				int sendResult = send(sock, buff, 2, 0);
				if (sendResult != SOCKET_ERROR) {
					int bytesReceived = recv(sock, buf, 500000, 0);
					if (bytesReceived > 0) {
						stringToJPG(buf, 500000);
						drawing(window1, scrX, scrY);
						l = false, r = false;
					}
				}
			}
			reciever.closeClient();
			controller.closeClient();
			thr.detach();
			std::cout << "Reconnect? (Y(1)/N(2)): ";
			std::cin >> quest;
			while (quest != 1 && quest != 2 ) {
				std::cout << "Incorrect input, please retyped" << std::endl;
				std::cin >> quest;
			}
		} while (quest == 1);

		std::cout << "Reconnect to another computer? (Y(1)/N(2)): ";
		std::cin >> quest;
		while (quest != 1 && quest != 2 ) {
			std::cout << "Incorrect input, please retyped" << std::endl;
			std::cin >> quest;
		}
	} while (quest == 1 );
	return 0;
}