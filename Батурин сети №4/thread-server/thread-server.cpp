/* TCP – ЭХО СЕРВЕР  для разрыва соединения клиент должен ввести строку “quit” */
#include <iostream>
#include <winsock2.h>  
#include <string>
#include <sstream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)
#pragma comment (lib,"Ws2_32.lib")
using namespace std;
u_short MY_PORT = 666;  // Порт, который слушает сервер         
// макрос для печати количества активных  пользователей 
#define PRINTNUSERS if (nclients)\
  cout << " user on-line " << nclients <<endl;\
  else cout << "No User on line\n";
/* прототип функции, обслуживающий  подключившихся клиентов  */
DWORD WINAPI ConToClient(LPVOID client_socket);
// глобальная переменная – количество  активных пользователей 
int nclients = 0;
struct BodyData
{
    int id;
    string name;
    int height;
    int weight;
    string problem;
};
int main() {
    char buff[1024];    // Буфер для различных нужд
    cout << "TCP SERVER DEMO\n";
    // Шаг 1 - Инициализация Библиотеки Сокетов
    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        cout << "Error WSAStartup \n" << WSAGetLastError();
        return -1;
    } // Ошибка!
// Шаг 2 - создание сокета
    SOCKET mysocket;
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {     // Ошибка! 
        cout << "Error socket \n" << WSAGetLastError();
        WSACleanup();  // Деиницилизация библиотеки Winsock
        return -1;
    }
    // Шаг 3 связывание сокета с локальным адресом
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(MY_PORT);
    local_addr.sin_addr.s_addr = 0;
    // вызываем bind для связывания
    if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {       // Ошибка
        cout << "Error bind \n" << WSAGetLastError();
        closesocket(mysocket);  // закрываем сокет!
        WSACleanup();
        return -1;
    }
    // Шаг 4 ожидание подключений
       // размер очереди – 0x100
    if (listen(mysocket, 0x100))
    {    // Ошибка
        cout << "Error listen: " << WSAGetLastError();
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    cout << "Waiting connections\n";
    // Шаг 5 извлекаем сообщение из очереди
    SOCKET client_socket;    // сокет для клиента
    sockaddr_in client_addr;    // адрес клиента
    // функции accept необходимо передать размер структуры
    int client_addr_size = sizeof(client_addr);
    // цикл извлечения запросов на подключение из  очереди
    while ((client_socket = accept(mysocket, (sockaddr*)
        &client_addr, &client_addr_size)))
    {
        nclients++;   // увеличиваем счетчик  клиентов
        HOSTENT* hst;  // пытаемся получить имя хоста
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
        cout << "+new connect!\n";    // вывод сведений о клиенте
        if (hst) cout << hst->h_name;    else cout << "";
        cout << inet_ntoa(client_addr.sin_addr);
        PRINTNUSERS           cout << endl;
        DWORD thID; // Вызов нового потока для клиента
        CreateThread(NULL, NULL, ConToClient, &client_socket, NULL, &thID);
    }     return 0;
}
/* Эта функция создается в отдельном потоке и  обсуживает очередного подключившегося клиента независимо от остальных */
DWORD WINAPI ConToClient(LPVOID client_socket)
{
    SOCKET my_sock;   int len;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[1024];
    char sHELLO[] = "Hello, Student \r\n";
    send(my_sock, sHELLO, sizeof(sHELLO), 0);
    // отправляем клиенту приветствие 
     // цикл эхо: прием строки и  возвращение ее клиенту
    while (SOCKET_ERROR !=
        (len = recv(my_sock, (char*)&buff[0], 1024, 0)))
    {
        buff[len] = '\0';    cout << "getted:" << buff << endl;
        istringstream s(buff);
        BodyData bD;
        s >> bD.id;
        s >> bD.name;
        s >> bD.height;
        s >> bD.weight;

        double bmi = (double)bD.weight * 10000 / (bD.height * bD.height);

        if (bmi < 18.5) {
            bD.problem = "underweight";
        }
        else if (bmi > 25) {
            bD.problem = "overweight";
        }
        else {
            bD.problem = "ok";
        }
        string answ = to_string(bmi) + " " + bD.problem;
        send(my_sock, answ.c_str(), answ.size(), 0);
    }
    // произошел выход из цикла, соединение c клиентом разорвано
    nclients--;     // уменьшаем счетчик активных клиентов
    cout << "-disconnect\n";
    PRINTNUSERS
        closesocket(my_sock);     return 0;
}