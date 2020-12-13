#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"
#include "Base64.h"
#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>

using namespace cv;

void createAlphaMat(Mat& mat)
{
    CV_Assert(mat.channels() == 4);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            Vec4b& bgra = mat.at<Vec4b>(i, j);
            bgra[0] = UCHAR_MAX; // Blue
            bgra[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
            bgra[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
            bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2])); // Alpha
        }
    }
}

std::string encodePic() {
	int count = 0;
	Mat img = cv::imread("0.bmp");

	std::vector<int> compression_params;
	compression_params.push_back(IMWRITE_JPEG_QUALITY);
	compression_params.push_back(20);
	imwrite("lowQ.jpg", img, compression_params);
	img = cv::imread("lowQ.jpg");

	std::vector<uchar> buf;
	imencode("lowQ.jpg", img, buf);
	auto* enc_msg = reinterpret_cast<unsigned char*>(buf.data());
	std::string encoded = base64_encode(enc_msg, buf.size());
	std::string str = encoded;
	std::cout << str.size() << std::endl;
	return encoded;
}

void saveFile() {
	HDC hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	DWORD dwWidth, dwHeight, dwBPP, dwNumColors;
	dwWidth = GetDeviceCaps(hdc, HORZRES);
	dwHeight = GetDeviceCaps(hdc, VERTRES);
	dwBPP = GetDeviceCaps(hdc, BITSPIXEL);
	if (dwBPP <= 8) {
		dwNumColors = GetDeviceCaps(hdc, NUMCOLORS);
		dwNumColors = 256;
	}
	else {
		dwNumColors = 0;
	}
	HDC hdc2 = CreateCompatibleDC(hdc);
	if (hdc2 == NULL) {
		DeleteDC(hdc);
	}
	LPVOID pBits;
	HBITMAP bitmap;
	BITMAPINFO bmInfo;

	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = dwWidth;
	bmInfo.bmiHeader.biHeight = dwHeight;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = (WORD)dwBPP;
	bmInfo.bmiHeader.biCompression = BI_RGB;
	bmInfo.bmiHeader.biSizeImage = 0;
	bmInfo.bmiHeader.biXPelsPerMeter = 0;
	bmInfo.bmiHeader.biYPelsPerMeter = 0;
	bmInfo.bmiHeader.biClrUsed = dwNumColors;
	bmInfo.bmiHeader.biClrImportant = dwNumColors;

	bitmap = CreateDIBSection(hdc, &bmInfo, DIB_PAL_COLORS, &pBits, NULL, 0);
	if (bitmap == NULL) {
		DeleteDC(hdc);
		DeleteDC(hdc2);
	}
	HGDIOBJ gdiobj = SelectObject(hdc2, (HGDIOBJ)bitmap);
	if ((gdiobj == NULL) || (gdiobj == (void*)(LONG_PTR)GDI_ERROR)) {
		DeleteDC(hdc);
		DeleteDC(hdc2);
	}
	if (!BitBlt(hdc2, 0, 0, dwWidth, dwHeight, hdc, 0, 0, SRCCOPY)) {
		DeleteDC(hdc);
		DeleteDC(hdc2);
	}
	RGBQUAD colors[256];
	if (dwNumColors != 0)
		dwNumColors = GetDIBColorTable(hdc2, 0, dwNumColors, colors);

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bitmapinfoheader;

	bmfh.bfType = 0x04D42;
	bmfh.bfSize = ((dwWidth * dwHeight * dwBPP) / 8) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dwNumColors * sizeof(RGBQUAD));
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (dwNumColors * sizeof(RGBQUAD));
	bitmapinfoheader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfoheader.biWidth = dwWidth;
	bitmapinfoheader.biHeight = dwHeight;
	bitmapinfoheader.biPlanes = 1;
	bitmapinfoheader.biBitCount = (WORD)dwBPP;
	bitmapinfoheader.biCompression = BI_RGB;
	bitmapinfoheader.biSizeImage = 0;
	bitmapinfoheader.biXPelsPerMeter = 0;
	bitmapinfoheader.biYPelsPerMeter = 0;
	bitmapinfoheader.biClrUsed = dwNumColors;
	bitmapinfoheader.biClrImportant = 0;
	std::ofstream file;
	file.open("0.bmp", std::ios::binary | std::ios::trunc | std::ios::out);
	file.write((char*)&bmfh, sizeof(BITMAPFILEHEADER));
	file.write((char*)&bitmapinfoheader, sizeof(BITMAPINFOHEADER));

	if (dwNumColors != 0)
		file.write((char*)colors, sizeof(RGBQUAD) * dwNumColors);
	file.write((char*)pBits, (dwWidth * dwHeight * dwBPP) / 8);

	DeleteObject(bitmap);
	DeleteDC(hdc2);
	DeleteDC(hdc);
}

void sendScreenSize(SOCKET s1, int scr) {
	size_t count;
	std::string str;
	char buf[10];
	std::stringstream st;
	st << scr;
	st >> str;
	recv(s1, buf, 10, 0);
	ZeroMemory(buf, 10);
	strcpy(buf, str.c_str());
	count = str.size();
	send(s1, buf, count, 0);
}

void controlM(SOCKET s1) {
	while (true) {
		char bufX[50], bufY[50], check[50];
		recv(s1, bufX, 4, 0);
		Sleep(25);
		send(s1, "1", 2, 0);
		Sleep(25);
		recv(s1, bufY, 4, 0);
		Sleep(25);
		send(s1, "1", 2, 0);
		Sleep(25);
		recv(s1, check, 2, 0);
		Sleep(25);
		int x, y, ch = 0;
		POINT cur_pos;
		x = atoi(bufX);
		y = atoi(bufY);
		SetCursorPos(x, y);
		ch = atoi(check);
		if (1 == ch) {
			INPUT Inputs[1] = { 0 };
			Inputs[0].type = INPUT_MOUSE;
			Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, Inputs, sizeof(INPUT));
			Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, Inputs, sizeof(INPUT));
		}
		if (2 == ch)
		{
			INPUT Inputs[1] = { 0 };
			Inputs[0].type = INPUT_MOUSE;
			Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			SendInput(1, Inputs, sizeof(INPUT));
			Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			SendInput(1, Inputs, sizeof(INPUT));
		}
		ZeroMemory(check, 50);
		ZeroMemory(bufX, 50);
		ZeroMemory(bufY, 50);
		
	}
}

int main()
{
	int quest;
	size_t port;
	SOCKET sendPic, control;
	Server videoSender, mouseControl;
	int count;
	char buf[500000], buff[10];
	std::string pic;
	do {
		int scrX = GetSystemMetrics(SM_CXSCREEN);
		int scrY = GetSystemMetrics(SM_CYSCREEN);
			videoSender.addictionalInfo();
			videoSender.getPort(videoSender);
			videoSender.startServer(videoSender, 0);
			sendPic = returnSocket(videoSender);
		sendScreenSize(sendPic, scrY);
		sendScreenSize(sendPic, scrX);
			port = returnPort(videoSender);
		    mouseControl.winsockInit(mouseControl);
			mouseControl.startServer(mouseControl, port + 1);
			control = returnSocket(mouseControl);
		std::thread thr(controlM, control);
		while (true) {
			count = 0;
			ZeroMemory(buf, 500000);
			ZeroMemory(buff, 10);
			pic.erase();
			saveFile();
			pic = encodePic();
			strcpy(buf, pic.c_str());
			count += pic.size();
			int bytesReceived = recv(sendPic, buff, 10, 0);
			if (bytesReceived == SOCKET_ERROR)
			{
				std::cerr << "Error in recv(). Quitting" << std::endl;
				break;
			}
			if (bytesReceived == 0)
			{
				std::cout << "Client disconected" << std::endl;
				break;
			}
			send(sendPic, buf, count, 0);
		}
		videoSender.closeServer();
		mouseControl.closeServer();
		thr.join();
		std::cout << "Restart server? (Y(1))/(N(2)): ";
		std::cin >> quest;
		while (quest != 1 && quest != 2) {
			std::cout << "Incorrect input, please retyped" << std::endl;
			std::cin >> quest;
			fflush(stdin);
		}
	} while (quest == 1);
	return 0;
}