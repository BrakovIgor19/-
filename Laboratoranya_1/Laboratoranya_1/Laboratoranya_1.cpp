// Лабораторная работа № 1
#include <iostream>
#include <conio.h> // Для считывания клавиши
#include <clocale> // Для русской локализации
#include <vector> // Для динамического массива
#include <sstream> // Для getline
#include <windows.h> // Для парса стринга в инты или даблы
using namespace std;

// Труба
struct Pipe
{
    unsigned int id; // Идентификатор трубы
    double lenght; // Длина трубы
    double diameter; // Диаметр трубы
    string signRepair; // Признак "в ремонте"
};
// Компрессорная станция 
struct CompressorStation
{
    unsigned int id; // Идентификатор трубы
    string name; // Название станции
    unsigned int numberWorkshops; // Количество цехов в станции
    unsigned int effectiveness; // Эффективность станции
};
/// <summary>
/// Метод проверет строку на цифры
/// </summary>
/// <param name="">Строка для проверки</param>
/// <returns>Возвращает правду если все цифры и неправду если хотя бы один символ строки не число</returns>
bool СheckingNumbersStringInt(string str)
{
    if ((str.size() == 0) || (str[0] == '0'))
    {
        return false;
    }
    for (int i = 0; i < str.size(); ++i)
    {        
        if (isdigit(str[i]))
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}
/// <summary>
/// Метод проверяет строку на число с плавающей точкой
/// </summary>
/// <param name="str">Строка</param>
/// <returns>Возвращает правду если строку можно распарсить в даблы</returns>
bool СheckingNumbersStringDouble(string str)
{
    if ((str.size() == 0) || (str[0] == '0'))
    {
        return false;
    }
    int buf = 0;
    for (int i = 0; i < str.size(); ++i)
    {
        if ((isdigit(str[i]) || (str[i] == '.')) && (buf < 2) && (str[0] != '.'))
        {
            if (str[i] == '.')
            {
                ++buf;
            }
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}
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
/// <summary>
/// Метод добавляет трубу
/// </summary>
/// <param name="pipes">Массив труб</param>
void AddPipe(vector <Pipe>& pipes)
{   
    system("cls");
    // Буферные переменные
    int key; string buf;
    pipes.resize(pipes.size() + 1);
    // Добавление id
    while (true)
    {
        cout << " id: ";
        getline(cin, buf);
        if (СheckingNumbersStringInt(buf))
        {
            cout << "\n";
            pipes[pipes.size() - 1].id = stoi(buf);
            break;            
        }
        else
        {
            cout << " Введите целое положительное число! \n";
        } 
    }
    // Добавление длины
    while (true)
    {
        cout << " Длина: ";
        getline(cin, buf);
        if (СheckingNumbersStringDouble(buf))
        {
            cout << "\n";
            pipes[pipes.size() - 1].lenght = stod(buf);
            break;
        }
        else
        {
            cout << " Введите вещественное положительное число! \n";
        }
    }
    // Добавление диаметра
    while (true)
    {
        cout << " Диаметр: ";
        getline(cin, buf);
        if (СheckingNumbersStringDouble(buf))
        {
            cout << "\n";
            pipes[pipes.size() - 1].id = stod(buf);
            break;
        }
        else
        {
            cout << " Введите вещественное положительное число! \n";
        }
    }
    // Добавление признака "в ремонте"
    while (true)
    {
        cout << " Нажмите 'n' если труба в ремонте или 'y' если труба исправна" << endl;
        key = _getch();
        if ((key == 110) || (key == 226))
        {
            pipes[pipes.size() - 1].signRepair = "В ремонте";
            break;
        }
        else if ((key == 121) || (key == 173))
        {
            pipes[pipes.size() - 1].signRepair = "Исправна";
            break;
        }
        else
        {
            cout << "\n Нажмите на одну из предложенных клавиш!";
        }
    }
}

int main()
{   // Создание Массива труб и массива КС-ок
    vector <Pipe> pipes; pipes.resize(0); vector <CompressorStation> compressorStations; compressorStations.resize(0);
    // Включение русского языка в консоли
    setlocale(LC_CTYPE, "rus");
    // Буферные переменные
    bool flag1 = false, flag2 = false;
    // Работа с меню
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
            case 49: // Добавить трубу на клавишу 1
                AddPipe(pipes);
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

