#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable: 4996)
using namespace std;
#define PORT 999

const string ex = "Вы отлично сдали экзамены!";
const string gd = "Вы хорошо сдали экзамены!";
const string nm = "Вы средне сдали экзамены!";
const string bd = "Вы плохо сдали экзамены!";
const string sb = "Вы не сдали экзамены!!";

struct Person {
	char name[25];
	double maht;
	int inf;
	int rus;
} A;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	char buff[1024];
	if (WSAStartup(0x0202, (WSADATA*)&buff[0])) //инициализация в библиотеке.1)версия в библиотеке 2)структура для записи
	{
		cout << "Error WSAStartup \n" << WSAGetLastError(); // Ошибка!
		return -1;
	}
	// char buf[64] = { 0 };
	SOCKET Lsock = socket(AF_INET, SOCK_DGRAM, 0);
	if (Lsock == INVALID_SOCKET)
	{
		cout << "SOCKET() ERROR: " << WSAGetLastError() << "\n";
		WSACleanup();
		return -1;
	}
	sockaddr_in Laddr; //структуры
	Lsock = socket(AF_INET, SOCK_DGRAM, 0);//создание слушащего сокета сервера.1)Домен 2)Потоковый режим 3)TCP
	Laddr.sin_family = AF_INET;
	Laddr.sin_addr.s_addr = INADDR_ANY;
	Laddr.sin_port = htons(PORT);
	if (bind(Lsock, (sockaddr*)&Laddr, sizeof(Laddr)))
	{
		cout << "BIND ERROR: " << WSAGetLastError();
		closesocket(Lsock);
		WSACleanup();
		return -1;
	}
	while (1) {//работа сервера с клиентом

		sockaddr_in Caddr;
		int Caddr_size = sizeof(Caddr);
		int bsize = recvfrom(Lsock, &buff[0], sizeof(buff) - 1, 0, (sockaddr*)&Caddr, &Caddr_size);//получуние сообщение 
		if (bsize == SOCKET_ERROR)  cout << "recform error: " << WSAGetLastError();
		hostent* hst;
		hst = gethostbyaddr((char*)&Caddr.sin_addr, 4, AF_INET);
		cout << "NEW DATAGRAM!\n" << ((hst) ? hst->h_name : "Unkown host") << "\n" << inet_ntoa(Caddr.sin_addr) << "\n" << ntohs(Caddr.sin_port) << '\n';
		int bsize2 = recvfrom(Lsock, (char*)&A, sizeof(buff) - 1, 0, (sockaddr*)&Caddr, &Caddr_size);
		if (bsize2 == SOCKET_ERROR)  cout << "recform2 error: " << WSAGetLastError();
		cout << A.name << " " << A.maht << " " << A.inf << " " << A.rus << endl;
		int ball = (A.maht + A.inf + A.rus) / 3;
		if (ball >= 90) {
			sendto(Lsock, &ex[0], ex.size(), 0, (sockaddr*)&Caddr, sizeof(Caddr));
		}
		if (ball >= 80 && ball < 90) {
			sendto(Lsock, &gd[0], gd.size(), 0, (sockaddr*)&Caddr, sizeof(Caddr));
		}
		if (ball >= 60 && ball < 80) {
			sendto(Lsock, &nm[0], nm.size(), 0, (sockaddr*)&Caddr, sizeof(Caddr));
		}
		if (ball >= 40 && ball < 60) {
			sendto(Lsock, &bd[0], bd.size(), 0, (sockaddr*)&Caddr, sizeof(Caddr));
		}
		if (ball >= 0 && ball < 40) {
			sendto(Lsock, &sb[0], sb.size(), 0, (sockaddr*)&Caddr, sizeof(Caddr));
		}
		cout << endl;
	}
	system("pause");
	return 0;
}