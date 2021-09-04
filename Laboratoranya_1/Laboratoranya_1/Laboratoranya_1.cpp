// Лабораторная работа № 1
#include <iostream>
#include <conio.h> // Для считывания клавиши
#include <clocale> // Для русской локализации

using namespace std;

// Труба
struct Pipe
{
    int id; // Идентификатор трубы
    double lenght; // Длина трубы
    double diameter; // Диаметр трубы
    bool signRepair; // Признак "в ремонте"
};

// Компрессорная станция
struct CompressorStation
{
    int id; // Идентификатор трубы
    string name; // Название станции
    int numberWorkshops; // Количество цехов в станции
    int effectiveness; // Эффективность станции
};

/// <summary>
/// Метод для перехода на страницу с меню 
/// </summary>
void Gotomenu()
{
    cout << "\t\t Меню" << endl
        << "\t 1. Добавить трубу" << endl
        << "\t 2. Добавить КС" << endl
        << "\t 3. Просмотр всех объектов" << endl
        << "\t 4. Редактировать трубу" << endl
        << "\t 5. Редактировать КС" << endl
        << "\t 6. Сохранить" << endl
        << "\t 7. Загрузить" << endl
        << "       Esc. Выход" << endl;
}

int main()
{  
    //Включение русского языка в консоли
    setlocale(LC_CTYPE, "rus");
    // Буферные переменные
    bool flag1 = false, flag2 = false;
    //Очистка консоли, вывод начального меню и считывание клавиши
    while (true)
    {
        system("cls");
        Gotomenu();
        int key = _getch();
        //Проверяем нажатую клавишу
        while (true)
        {
            switch (key)
            {
            case 49:    // клавиша 1
                flag1 = true;
                break;
            case 50:    // клавиша 2
                flag1 = true;
                break;
            case 51:    // клавиша 3
                flag1 = true;
                break;
            case 52:    // клавиша 4
                flag1 = true;
                break;
            case 53:    // клавиша 5
                flag1 = true;
                break;
            case 54:    // клавиша 6
                flag1 = true;
                break;
            case 55:    // клавиша 7
                flag1 = true;
                break;
            case 27:    // клавиша Escape
                flag1 = true, flag2 = true;
                break;
            default:
                cout << "Нажмите на одну из предложенных клавиш!" << endl;
            }
            if (flag1)
            {
                break;
            }
        }
        if (flag2)
        {
            break;
        }
    }
    return 0;
}

