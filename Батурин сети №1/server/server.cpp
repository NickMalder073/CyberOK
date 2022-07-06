#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

struct Person {
    char name[25];
    int math;
    int russ;
    int inf;
}B;

int k = 0;
double ball = 0;//место и бал
int answer;//ответ
long size_pred;

int main() {
    ifstream fR;
    ofstream fA;
    ofstream f;
    f.open("F:\\projects\\плюсы на кладбище\\NETWORKS\\1\\bug\\queue.bin");
    f.close();
    setlocale(LC_ALL, "rus");
    const char* nameR = "F:\\projects\\плюсы на кладбище\\NETWORKS\\1\\bug\\server.bin ";//файл запросов клиентов
    const char* nameA = "F:\\projects\\плюсы на кладбище\\NETWORKS\\1\\bug\\second.bin";//файл ответов сервера
    const char* nameQ = "F:\\projects\\плюсы на кладбище\\NETWORKS\\1\\bug\\queue.bin ";//файл  записи на очередь

    cout << "server is working" << endl;
    fR.open(nameR, ios::binary);//открытие файла REQUEST
    fR.seekg(0, ios::end);
    size_pred = fR.tellg();//стартовая позиция сервера в файле REQUEST
    fR.close();
    while (true) {// начало работы сервера
        fR.open(nameR, ios::binary);
        fR.seekg(0, ios::end);
        while (size_pred >= fR.tellg())
        {
            Sleep(100); fR.seekg(0, ios::end);
        }
        fR.seekg(size_pred, ios::beg);//переход к началу полученного запроса
        fR.read((char*)&B, sizeof(B));//считывание данных клиента

        cout << "Сервер работает с учени(ком/цой) " << B.name << endl;
        size_pred = fR.tellg();
        fR.close();
        ball = (B.math + B.russ + B.inf) / 3;//Считаем сумму баллов
        //В 5 if'aх определяем как ученик сдал экзамен
        if (ball >= 90) {
            answer = 0;
            k++;
        }
        if (ball >= 80 && ball < 90) {
            answer = 1;
            k++;
        }
        if (ball >= 60 && ball < 80) {
            answer = 2;
            k++;
        }
        if (ball >= 40 && ball < 60) {
            answer = 3;
            k++;
        }
        if (ball >= 0 && ball < 40) {
            answer = 4;
            k++;
        }
        fA.open(nameA, ios::binary | ios::app);//открытие файла ANSWER и запись в конец 
        fA.write((char*)&answer, sizeof(answer));//запись ответа клиенту
        fA.close();
        fA.open(nameQ, ios::binary | ios::app);//открытие файла очереди
        fA.write((char*)&k, sizeof(ball));//запись баллов
        fA.write((char*)&k, sizeof(k));//запись номера ученика
        fA.close();
        cout << "Сервер закончил работу с учени(ком/цой): " << B.name << " его/её средний балл = " << ball << " №" << k << endl;
        ball = 0;
    }// while
}