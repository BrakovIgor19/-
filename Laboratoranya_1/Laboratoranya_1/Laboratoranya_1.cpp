// Лабораторная работа № 1
#include <iostream>
#include <conio.h> // Для считывания клавиши
#include <clocale> // Для русской локализации
#include <vector> // Для динамического массива
#include <sstream> // Для getline
#include <windows.h> // Для парса стринга в инты или даблы
#include <fstream> // Для работы с файлами
using namespace std;

// Труба
struct Pipe
{
    int id; // Идентификатор трубы
    double lenght; // Длина трубы
    double diameter; // Диаметр трубы
    string signRepair; // Признак "в ремонте"
};
// Компрессорная станция 
struct CompressorStation
{
    int id; // Идентификатор трубы
    string name; // Название станции
    int numberWorkshops; // Количество цехов в станции
    int numberWorkshopsOperation; // Количество цехов в работе
    int effectiveness; // Эффективность станции
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
///Метод проверяет строку на число с плавающей точкой
/// </summary>
/// <param name="str">Строка</param>
/// <returns>Возвращает правду если строку можно распарсить в даблы</returns>
bool СheckingNumbersStringDouble(string str)
{
    if ((str.size() == 0) || ((str[1] == '0') && (str[0] == '0')) || (str[0] == '.'))
    {
        return false;
    }
    int buf = 0;
    for (int i = 0; i < str.size(); ++i)
    {
        if ((isdigit(str[i]) || (str[i] == '.')) && (buf < 2))
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
    cout << "\tТруба №" << pipes.size() << "\n\n";
    // Добавление id
    while (true)
    {
        cout << " id: ";
        getline(cin, buf);
        if (СheckingNumbersStringInt(buf))
        {
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
            pipes[pipes.size() - 1].diameter = stod(buf);
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
        cout << "\n Нажмите 'n' если труба в ремонте или 'y' если труба исправна" << endl;
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
            cout << " Нажмите на одну из предложенных клавиш!";
        }
    }
}
/// <summary>
/// Метод добавляет компрессорную станцию
/// </summary>
/// <param name="pipes">Массив КС-ок</param>
void AddCompressorStation(vector <CompressorStation>& compressorStations)
{
    system("cls");
    // Буферные переменные
    int buf1; string buf;
    compressorStations.resize(compressorStations.size() + 1);
    cout << "\tКомпрессорная станция №" << compressorStations.size() << "\n\n";
    // Добавление id
    while (true)
    {
        cout << " id: ";
        getline(cin, buf);
        if (СheckingNumbersStringInt(buf))
        {
            compressorStations[compressorStations.size() - 1].id = stoi(buf);
            break;
        }
        else
        {
            cout << " Введите целое положительное число! \n";
        }
    }
    // Добавление названия
    while (true)
    {
        cout << " Название: ";
        getline(cin, buf);
        if (buf.size() > 0)
        {
            compressorStations[compressorStations.size() - 1].name = buf;
            break;
        }
        else
        {
            cout << " Введите название!\n";
        }
    }
    // Добавление количества цехов
    while (true)
    {
        cout << " Количество цехов: ";
        getline(cin, buf);
        if (СheckingNumbersStringInt(buf))
        {
            compressorStations[compressorStations.size() - 1].numberWorkshops = stoi(buf);
            break;
        }
        else
        {
            cout << " Введите целое положительное число! \n";
        }
    }
    // Добавление количества цехов в работе
    while (true)
    {
        cout << " Количество цехов в работе: ";
        getline(cin, buf);
        if (СheckingNumbersStringInt(buf))
        {
            buf1 = stoi(buf);
            if (buf1 <= compressorStations[compressorStations.size() - 1].numberWorkshops)
            {
                compressorStations[compressorStations.size() - 1].numberWorkshopsOperation = buf1;
                break;
            }
            else
            {
                cout << " Количество рабочих цехов должно быть меньше либо равняться количеству всех цехов компрессорной станции!\n";
            }
        }
        else
        {
            cout << " Введите целое положительное число! \n";
        }
    }
    // Добавление эффективности
    while (true)
    {
        cout << " Эффективность станции: ";
        getline(cin, buf);
        if (СheckingNumbersStringInt(buf))
        {
            compressorStations[compressorStations.size() - 1].effectiveness = stoi(buf);
            break;
        }
        else
        {
            cout << " Введите целое положительное число! \n";
        }
    }
}
/// <summary>
/// Метод рисует вывод всех труб
/// </summary>
/// <param name="pipes">Массив труб</param>
void ShowAllPipes(vector <Pipe> pipes)
{
    cout << "\n\t\t\t\tТрубы\n" << endl;
    cout << " Номер трубы" << "\tid" << "\tДлина" << "\t\tДиаметр" << "\t\tПризнак в ремонте" << endl;
    for (int i = 0; i < pipes.size(); ++i)
    {
        cout << "   " << i + 1 << "\t\t" << pipes[i].id << "\t" << pipes[i].lenght << "\t\t" << pipes[i].diameter << "\t\t" << pipes[i].signRepair << endl;
    }
    cout << "\n";
}
/// <summary>
/// Метод рисует вывод всех компрессорных станций
/// </summary>
/// <param name="pipes">Массив КС-ок</param>
void ShowAllCompressionStations(vector <CompressorStation> compressorStations)
{
    cout << "\t\t\tКомпрессорные станции - КС\n" << endl;
    cout << " Номер КС" << "\tid" << "    Название" << "\t\tКол-во цехов" << "\tЦехов в работе" << "\t Эффект-ть" << endl;
    for (int i = 0; i < compressorStations.size(); ++i)
    {
        cout << "   " << i + 1 << "\t\t" << compressorStations[i].id << "\t" << compressorStations[i].name << "\t\t" << compressorStations[i].numberWorkshops << "\t\t" << compressorStations[i].numberWorkshopsOperation << "\t\t" << compressorStations[i].effectiveness << endl;
    }
    cout << "\n";
}
/// <summary>
/// Метод показывает все объекты
/// </summary>
/// <returns></returns>
void ShowAllObjects(const vector <Pipe>& pipes, const vector <CompressorStation>& compressorStations)
{
    system("cls");
    ShowAllPipes(pipes);
    ShowAllCompressionStations(compressorStations);
}
/// <summary>
/// Метод сохраняет данные
/// </summary>
/// <param name="pipes">Трубы</param>
/// <param name="compressorStations">КС-ки</param>
void SaveData(const vector <Pipe>& pipes, const vector <CompressorStation>& compressorStations)
{
    ofstream fout;
    fout.open("data.txt", ios::out);
    fout << "\n\t\t\t\tТрубы\n" << endl;
    fout << " Номер трубы" << "\tid" << "\tДлина" << "\t\tДиаметр" << "\t\tПризнак в ремонте" << endl;
    for (int i = 0; i < pipes.size(); ++i)
    {
        fout << "   " << i + 1 << "\t\t" << pipes[i].id << "\t" << pipes[i].lenght << "\t\t" << pipes[i].diameter << "\t\t" << pipes[i].signRepair << endl;
    }
    fout << "\n";
    fout << "\t\t\tКомпрессорные станции - КС\n" << endl;
    fout << " Номер КС" << "\tid" << "    Название" << "\t\tКол-во цехов" << "\tЦехов в работе" << "\tЭффект-ть" << endl;
    for (int i = 0; i < compressorStations.size(); ++i)
    {
        fout << "   " << i + 1 << "\t\t" << compressorStations[i].id << "\t" << compressorStations[i].name << "\t\t" << compressorStations[i].numberWorkshops << "\t\t" << compressorStations[i].numberWorkshopsOperation << "\t\t" << compressorStations[i].effectiveness << endl;
    }
    fout << "\n";
    fout.close();
}

int main()
{   // Создание Массива труб и массива КС-ок
    vector <Pipe> pipes; pipes.resize(0); vector <CompressorStation> compressorStations; compressorStations.resize(0);
    // Включение русского языка в консоли
    setlocale(LC_CTYPE, "rus");
    // Буферные переменные
    bool flag1, flag2 = false; int key, buf;
    // Работа с меню
    while (true)
    {
        system("cls");
        Gotomenu();
        key = _getch();
        //Проверяем нажатую клавишу
        while (true)
        {
            flag1 = false;
            switch (key)
            {
            case 49: // Добавить трубу на клавишу 1
                AddPipe(pipes);
                flag1 = true;
                break;
            case 50: // Добавить комрессорную станцию на клавишу 2
                AddCompressorStation(compressorStations);
                flag1 = true;
                break;
            case 51:    // Просмотр всех объектов на клавишу 3
                ShowAllObjects(pipes, compressorStations);
                cout << "\n\n\n Нажмите Escape чтобы вернуть в меню";
                while (true)
                {
                    buf = _getch();
                    if (buf == 27)
                    {
                        flag1 = true;
                        break;
                    }
                    else
                    {
                        cout << "\n Нажмите Escape для выхода!";
                    }
                }                
                break;
            case 52:    // клавиша 4
                flag1 = true;
                break;
            case 53:    // клавиша 5
                flag1 = true;
                break;
            case 54:    // Сохранить все данные в файлы на клавишу 6
                SaveData(pipes, compressorStations);
                flag1 = true;
                break;
            case 55:    // клавиша 7
                flag1 = true;
                break;
            case 27:    // клавиша Escape
                flag1 = true, flag2 = true;
                break;
            default:
                flag1 = true;
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

