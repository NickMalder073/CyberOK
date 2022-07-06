#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// подавление предупреждений библиотеки winsock2
#include <winsock2.h>
#include <string>
#include <windows.h>
#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable: 4996) // подавление предупреждения 4996
using namespace std;
#define PORT 999
#define SERVERADDR "127.0.0.1"//порт сервера

struct Person
{
	char name[25];
	double maht;
	int inf;
	int rus;
}B;
int main() {
	char buff[10 * 1024];
	if (WSAStartup(0x0202, (WSADATA*)&buff)) {
		cout << "Error WSAStartup \n" << WSAGetLastError(); // Ошибка!
		return -1;
	}
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	SOCKET ms = socket(AF_INET, SOCK_DGRAM, 0);//сокет клиента
	if (ms == INVALID_SOCKET)
	{
		cout << "SOCKET() ERROR: " << WSAGetLastError() << "\n";
		WSACleanup();
		return -1;
	}
	hostent* hst;
	sockaddr_in Daddr;
	Daddr.sin_family = AF_INET;
	Daddr.sin_port = htons(PORT);
	if (inet_addr(SERVERADDR))
		Daddr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else if (hst = gethostbyname(SERVERADDR))
		Daddr.sin_addr.s_addr = ((unsigned long**)hst->h_addr_list)[0][0];
	else {
		cout << "Unknown Host: " << WSAGetLastError() << "\n";
		closesocket(ms);
		WSACleanup();
		return -1;
	}
	while (1) {
		cout << "Нажмите любую клавишу чтобы продолжить или stop = завершение  " << endl;
		string s;
		getline(cin, s);
		if (s == "stop")
			break;
		sendto(ms, (char*)&s[0], s.size(), 0, (sockaddr*)&Daddr, sizeof(Daddr));
		sockaddr_in SRaddr;
		int SRaddr_size = sizeof(SRaddr);
		cout << "Введите запрос: Фамилия, математика, информатика и русский язык" << endl;
		cin >> B.name >> B.maht >> B.inf >> B.rus;
		getchar();
		sendto(ms, (char*)&B, sizeof(B), 0, (sockaddr*)&Daddr, sizeof(Daddr));//передача сообщения
		int n = recvfrom(ms, buff, sizeof(buff), 0, (sockaddr*)&SRaddr, &SRaddr_size);
		if (n == SOCKET_ERROR)
		{
			cout << "recform error: " << WSAGetLastError() << "\n";
			closesocket(ms);
			WSACleanup();
			return -1;
		}
		buff[n] = '\0';
		cout << buff << "\n";
		cout << endl;

	}
	closesocket(ms);
	WSACleanup();
	system("pause");
	return 0;
}