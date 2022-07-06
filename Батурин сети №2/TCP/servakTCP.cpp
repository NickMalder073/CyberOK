#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include<conio.h>
#pragma comment (lib, "Ws2_32.lib")
using namespace std;
#define SRV_PORT 1234
#define BUF_SIZE 64


const string ex = "Вы отлично сдали экзамены!";
const string gd = "Вы хорошо сдали экзамены!";
const string nm = "Вы средне сдали экзамены!";
const string bd = "Вы плохо сдали экзамены!";
const string sb = "Вы не сдали экзамены!!";

struct Person
{
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
    SOCKET s, s_new;
    int from_len, a;
    char buf[BUF_SIZE] = { 0 };
    sockaddr_in sin, from_sin; //структуры
    s = socket(AF_INET, SOCK_STREAM, 0);//создание слушащего сокета сервера.1)Домен 2)Потоковый режим 3)TCP
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0; //адресс ip узла
    sin.sin_port = htons(SRV_PORT);//номер порта в сетевом формате
    bind(s, (sockaddr*)&sin, sizeof(sin));//связывание сокета и структуры
    string msg, msg1;
    listen(s, 3);//создание очереди сокетов
    while (1) {//работа сервера с клиентом
        from_len = sizeof(from_sin);
        s_new = accept(s, (sockaddr*)&from_sin, &from_len);//запрос на установку соединения 1)сокет прослушивающийся сервером
        cout << "client connected! " << endl;
        //msg = QUESTION;
        while (1) {//общение с отдельным клиентом   
            from_len = recv(s_new, (char*)&A, BUF_SIZE, 0);//прием сообщения
            cout << A.name << " " << A.maht << " " << A.inf << " " << A.rus << endl;
            int ball = (A.maht + A.inf + A.rus) / 3;
            if (ball >= 90)
                send(s_new, (char*)&ex[0], ex.size(), 0);
            if (ball >= 80 && ball < 90) {
                send(s_new, (char*)&gd[0], gd.size(), 0);
            }
            if (ball >= 60 && ball < 80) {
                send(s_new, (char*)&nm[0], nm.size(), 0);
            }
            if (ball >= 40 && ball < 60) {
                send(s_new, (char*)&bd[0], bd.size(), 0);
            }
            if (ball >= 0 && ball < 40) {
                send(s_new, (char*)&sb[0], sb.size(), 0);
            }
            from_len = recv(s_new, (char*)&buf, BUF_SIZE, 0);
            buf[from_len] = 0;
            msg1 = (string)buf;
            if (msg1 == "stop") break;
        }
        cout << "client is lost";
        cout << " " << endl;
        closesocket(s_new);//закрытие сокета
    }
    return 0;
}