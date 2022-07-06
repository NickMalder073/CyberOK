#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

struct Person {
    char name[25];
    int math;
    int russ;
    int inf;
} A;

void main() {
    setlocale(LC_ALL, "rus");
    const char* nameR = "F:\\projects\\плюсы на кладбище\\NETWORKS\\1\\bug\\server.bin ";//файл для сервера
    const char* nameA = "F:\\projects\\плюсы на кладбище\\NETWORKS\\1\\bug\\second.bin";//файл для клиента
    const char* nameQ = "F:\\projects\\плюсы на клад бище\\NETWORKS\\1\\bug\\queue.bin ";//файл с номером очереди
    ofstream f_REQ, fr;
    ifstream f_ANS, fa;
    long pred_size;
    long size_predQueue;
    int answer, k;
    while (true)
    {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        cout << "Введите Фамилию и баллы по 3 предметам: Математика, Русский, Информатика" << endl;
        cin >> A.name >> A.math >> A.russ >> A.inf;
        cout << "Ответ для пользователя " << A.name << endl;
        cout << "Учени(к/ца) " << A.name << " с баллами по Математике " << A.math << ", Руссикий язык " << A.russ << " и Информатике " << A.inf << endl;

        f_REQ.open(nameR, ios::app | ios::binary);//запись в файл
        f_REQ.write((char*)&A, sizeof(A));
        f_REQ.close();

        f_ANS.open(nameA, ios::binary);
        fa.open(nameQ, ios::binary);
        if (!f_ANS.is_open() || !fa.is_open())//Если с файлом что-то не так выводим ERROR
            cout << "ERROR" << endl;
        else
        {
            f_ANS.seekg(0, ios::end);
            fa.seekg(0, ios::end);
            size_predQueue = fa.tellg();
            pred_size = f_ANS.tellg();
            while (pred_size >= f_ANS.tellg())
            {
                Sleep(100);
                f_ANS.seekg(0, ios::end);
            }
            while (size_predQueue >= fa.tellg())
            {
                Sleep(100);
                fa.seekg(0, ios::end);
            }
            f_ANS.seekg(pred_size, ios::beg);
            fa.seekg(size_predQueue, ios::beg);
            f_ANS.read((char*)&answer, sizeof(answer));
            fa.read((char*)&k, sizeof(k));
            fa.close();
            f_ANS.close();
        }
        switch (answer) {
        case 0: {cout << "Ученик отлично сдал экзамены!" << endl; break; }
        case 1: {cout << "Ученик хорошо сдал экзамены! " << endl; break; }
        case 2: {cout << "Ученик средне сдал экзамены! " << endl; break; }
        case 3: {cout << "Ученик плохо сдал экзамены! " << endl; break; }
        case 4: {cout << "Ученик не сдал экзамены! " << endl; };

        }
    }
}