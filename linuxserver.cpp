#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>

#pragma comment(lib,"Ws2_32.lib")


using namespace std;

const int BUFLEN = 512;

int main()
{
	// Инициализировать Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Сбой WSAStartup с ошибкой: " << iResult << endl;
		return 1;
	}

	// Создайте сокет
	SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		cout << "сбой сокета с ошибкой: " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	// Подключитесь к серверу
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1"); // Замените на IP-адрес сервера Linux
	clientService.sin_port = htons(2525); // Замените на номер порта сервера Linux
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		cout << "подключение не удалось с ошибкой: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Отправка данных на сервер
	string dataToSend = "Это данные, которые должны быть записаны в текстовый файл.";
	iResult = send(ConnectSocket, dataToSend.c_str(), dataToSend.length(), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "отправить не удалось с ошибкой: " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Получите ответ от сервера
	char recvbuf[BUFLEN];
	int recvbuflen = BUFLEN;
	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		cout << "Полученные байты: " << iResult << endl;

		// Запишите данные в текстовый файл
		ofstream outfile;
		outfile.open("received_data.txt");
		outfile << recvbuf;
		outfile.close();
	}
	else if (iResult == 0) {
		cout << "Соединение закрыто" << endl;
	}
	else {
		cout << "сбой recv с ошибкой: " << WSAGetLastError() << endl;
	}

	// Убирать
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
