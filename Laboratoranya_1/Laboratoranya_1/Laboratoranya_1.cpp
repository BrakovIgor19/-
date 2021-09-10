// Лабораторная работа № 1
#include <iostream>
#include <conio.h> // Для считывания клавиши
#include <clocale> // Для русской локализации
#include <vector> // Для динамического массива
#include <sstream> // Для getline()
#include <windows.h> // Для парса стринга в инты или даблы, для скрывания курсора
#include <fstream> // Для работы с файлами
#include <string> // Для работы со строками

using namespace std;

enum Menu
{
    addPipe = 0,
    addKC  = 1,
    showObjects = 2,
    editPipe = 3,
    editKC = 4,
    save = 5,
    download = 6,
    Exit = 7
};

enum ConsoleKey
{
     UP = 72,
     DOWN = 80,
     ESC = 27,
     ENTER = 13
};

enum ConsoleColor
{
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

// Труба
struct Pipe
{
    int id; // Идентификатор трубы
    double lenght; // Длина трубы
    double diameter; // Диаметр трубы
    string signRepair; // Признак "в ремонте"
};
// Компрессорная станция 
struct KC
{
    int id; // Идентификатор трубы
    string name; // Название станции
    int numberWorkshops; // Количество цехов в станции
    int numberWorkshopsOperation; // Количество цехов в работе
    int effectiveness; // Эффективность станции
};
/// <summary>
/// gotoxy для новых C++
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(int x, int y)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(hStdOut, c);
}
/// <summary>
/// Узнать координату х
/// </summary>
/// <returns></returns>
int getXcoord()
{
    CONSOLE_SCREEN_BUFFER_INFO info_x;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdOut, &info_x);
    return info_x.dwCursorPosition.X;
}
/// <summary>
/// Узнать координату y
/// </summary>
/// <returns></returns>
int getYcoord()
{
    CONSOLE_SCREEN_BUFFER_INFO info_y;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdOut, &info_y);
    return info_y.dwCursorPosition.Y;
}
/// <summary>
/// Метод изменяет цвет текста и фона
/// </summary>
/// <param name="text">Цвет текста</param>
/// <param name="background">Цвет заднего фона</param>
void ChangeConsoleColor(ConsoleColor text, ConsoleColor background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //Получение дескриптора консоли
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));  //Изменение заднего фона и шрифта консоли
}
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
/// Метод устанавливает размеры консоли по умолчанию
/// </summary>
void SetConsoleAttributes()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD crd = { 95, 26 };
    SMALL_RECT src = { 0, 0, crd.X - 1, crd.Y - 1 };
    SetConsoleWindowInfo(hStdOut, true, &src);
    SetConsoleScreenBufferSize(hStdOut, crd);
    ChangeConsoleColor(LightGreen, Black);
}
/// <summary>
/// Метод скрывает курсор
/// </summary>
/// <param name="showFlag">True - мигать, False - скрыть</param>
void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
/// <summary>
/// Запоминание цвета фона и текста текущей консоли
/// </summary>
/// <param name="pipes"></param>
void GetConsoleColors(HANDLE& hStdOut, CONSOLE_SCREEN_BUFFER_INFO& start_attribute)
{
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // Получение дескриптора консоли
    GetConsoleScreenBufferInfo(hStdOut, &start_attribute); // Запоминание начальных параметров
}
/// <summary>
/// Метод для возвращения в меню после выполнения метода
/// </summary>
void BackMenu()
{
    int buf;
    cout << "\n\n\n Нажмите Escape чтобы вернуться в меню";
    while (true)
    {
        buf = _getch();
        if (buf == 27)
        {
            break;
        }
        else
        {
            cout << "\n Нажмите Escape для выхода!";
        }
    }
}
/// <summary>
/// Метод для перехода на страницу с меню с возвратом первой строчки меню 
/// </summary>
void ShowMenu(vector <string> Menu, int* firstLineMenu, int bufActiveMenu)
{
    SetConsoleAttributes();
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdOut, &csbiInfo);
    int xCenter = (csbiInfo.srWindow.Right - csbiInfo.srWindow.Left) / 2; int yCenter = (csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top) / 2; int xBuf; // находим координаты х и y центра окна
    *firstLineMenu = yCenter - Menu.size() / 2; //Возвращаем первую строчку меню
    for (int i = 0; i < Menu.size(); ++i)
    {
        if (i + *firstLineMenu != bufActiveMenu)
            { ChangeConsoleColor(Green, Black); }
        else
            { ChangeConsoleColor(LightGreen, Black); }

        xBuf = (xCenter - Menu[i].size() / 2); // Находим координату х каждой строки для написания по середине
        gotoxy(xBuf, i + *firstLineMenu);
        cout << Menu[i];
    }
    ChangeConsoleColor(LightGreen, Black);

}
/// <summary>
/// Метод для перехода на страницу с меню без возврата первой строчки меню
/// </summary>
void ShowMenu(vector <string> Menu, int bufActiveMenu)
{
    SetConsoleAttributes();
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdOut, &csbiInfo);
    int xCenter = (csbiInfo.srWindow.Right - csbiInfo.srWindow.Left) / 2; int yCenter = (csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top) / 2; int xBuf; // находим координаты х и y центра окна
    int firstLineMenu = yCenter - Menu.size() / 2; // Первая строчка меню
    for (int i = 0; i < Menu.size(); ++i)
    {
        if (i + firstLineMenu != bufActiveMenu)
        {
            ChangeConsoleColor(Green, Black);
        }
        else
        {
            ChangeConsoleColor(LightGreen, Black);
        }

        xBuf = (xCenter - Menu[i].size() / 2); // Находим координату х каждой строки для написания по середине
        gotoxy(xBuf, i + firstLineMenu);
        cout << Menu[i];
    }
    ChangeConsoleColor(LightGreen, Black);

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
    system("cls");
}
/// <summary>
/// Метод добавляет компрессорную станцию
/// </summary>
/// <param name="pipes">Массив КС-ок</param>
void AddKC(vector <KC>& KC)
{
    system("cls");
    // Буферные переменные
    int buf1; string buf;
    KC.resize(KC.size() + 1);
    cout << "\tКомпрессорная станция №" << KC.size() << "\n\n";
    // Добавление id
    while (true)
    {
        cout << " id: ";
        getline(cin, buf);
        if (СheckingNumbersStringInt(buf))
        {
            KC[KC.size() - 1].id = stoi(buf);
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
            KC[KC.size() - 1].name = buf;
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
            KC[KC.size() - 1].numberWorkshops = stoi(buf);
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
            if (buf1 <= KC[KC.size() - 1].numberWorkshops)
            {
                KC[KC.size() - 1].numberWorkshopsOperation = buf1;
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
            KC[KC.size() - 1].effectiveness = stoi(buf);
            break;
        }
        else
        {
            cout << " Введите целое положительное число! \n";
        }
    }
    system("cls");
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
void ShowAllCompressionStations(vector <KC> KC)
{
    cout << "\t\t\tКомпрессорные станции - КС\n" << endl;
    cout << " Номер КС" << "\tid" << "    Название" << "\t\tКол-во цехов" << "\tЦехов в работе" << "\t Эффект-ть" << endl;
    for (int i = 0; i < KC.size(); ++i)
    {
        cout << "   " << i + 1 << "\t\t" << KC[i].id << "\t" << KC[i].name << "\t\t" << KC[i].numberWorkshops << "\t\t" << KC[i].numberWorkshopsOperation << "\t\t" << KC[i].effectiveness << endl;
    }
    cout << "\n";
}
/// <summary>
/// Метод показывает все объекты
/// </summary>
/// <returns></returns>
void ShowAllObjects(const vector <Pipe>& pipes, const vector <KC>& KC)
{
    system("cls");
    ShowAllPipes(pipes);
    ShowAllCompressionStations(KC);
    BackMenu();
    system("cls");
}

/// <summary>
/// Метод сохраняет данные
/// </summary>
/// <param name="pipes">Трубы</param>
/// <param name="compressorStations">КС-ки</param>
void SaveData(const vector <Pipe>& pipes, const vector <KC>& KC)
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
    for (int i = 0; i < KC.size(); ++i)
    {
        fout << "   " << i + 1 << "\t\t" << KC[i].id << "\t" << KC[i].name << "\t\t" << KC[i].numberWorkshops << "\t\t" << KC[i].numberWorkshopsOperation << "\t\t" << KC[i].effectiveness << endl;
    }
    fout << "\n";
    fout.close();
}

void EditPipe(vector <Pipe>& pipes)
{
    bool flag1 = false; int activeMenuItem, bufActiveMenuItem, key, bufKey, buf1 = 0, bufX, bufY; string buf2; const int menuItems = 4; HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO start_attribute;
    system("cls");
    cout << " Количество труб: " << pipes.size() << "\n\n";
    // Спрашиваем номер трубы и возвращаем номер трубы "buf1"
    while (true)
    {
        if (pipes.size() == 0)
        {
            cout << " Трубы еще не подвезли!";
            flag1 = true;
            break;
        }
        cout << " Введите номер трубы: ";
        getline(cin, buf2);
        if (СheckingNumbersStringInt(buf2))
        {
            buf1 = stoi(buf2);
            if (buf1 <= pipes.size())
            {
                break;
            }
            else
            {
                cout << " Введите номер трубы не превышающий количество труб!\n";
            }
        }
        else
        {
            cout << " Введите целое положительное число! \n";
        }
    }
    if (flag1)
    {
        BackMenu();
    }
    else
    {
        // Выводим данные по данной трубе и иструкции для пользователя 
        system("cls");
        cout << "\t                 Труба № " << buf1 << endl
            << "\n\t                 id: "; bufX = getXcoord(); bufY = getYcoord(); cout << pipes[buf1 - 1].id << endl
            << "\t              Длина: " << pipes[buf1 - 1].lenght << endl
            << "\t            Диаметр: " << pipes[buf1 - 1].diameter << endl
            << "\tПризнак 'в ремонте': " << pipes[buf1 - 1].signRepair << endl;
        gotoxy(0, 8);
        cout << "Используйте стрелки 'Вверх' и 'Вниз' для перемещения по данным" << endl
            << "Чтобы изменить данные нажмите 'Enter' и введите данные " << endl
            << "Чтобы изменить признак 'в ремонте' выберите этот пункт нажмите Enter и выберите стрелками состояние " << endl
            << "Для выхода в меню нажмите 'Escape'";
        gotoxy(bufX, bufY);
        activeMenuItem = bufY;
        ShowConsoleCursor(false);
        // Работа с данными
        while (true)
        {
            // Запоминаем цвет консоли
            GetConsoleColors(hStdOut, start_attribute);
            bufActiveMenuItem = activeMenuItem;
            // Перекрашиваем данную клетку
            ChangeConsoleColor(LightCyan, Blue);
            switch (activeMenuItem)
            {
                case 2: cout << pipes[buf1 - 1].id; break;
                case 3: cout << pipes[buf1 - 1].lenght; break;
                case 4: cout << pipes[buf1 - 1].diameter; break;
                case 5: cout << pipes[buf1 - 1].signRepair; break;
            }
            gotoxy(bufX, activeMenuItem);
            // Считывание адского кода стрелки
            key = _getch();
            if (key == 224)
            {
                key = _getch();
            }
            // Обработа введенной клавиши
            switch (key)
            {
                case 27:  flag1 = true; break; // Клавиша Escape
                case 72:  --activeMenuItem; break; // Клавиша стрелка вверх
                case 80:  ++activeMenuItem; break; // Клавиша стрелка вниз
                case 13: // Клавиша Enter
                    // Вводим новые данные с проверкой
                {   
                    // Изменяем расцветку
                    ChangeConsoleColor(Blue, LightCyan);
                    switch (activeMenuItem)
                    {
                    case 2: cout << pipes[buf1 - 1].id; break;
                    case 3: cout << pipes[buf1 - 1].lenght; break;
                    case 4: cout << pipes[buf1 - 1].diameter; break;
                    case 5: cout << pipes[buf1 - 1].signRepair; break;
                    }
                    gotoxy(bufX, activeMenuItem);
                    // Приводим к виду ввода
                    gotoxy(bufX, activeMenuItem);
                    switch (activeMenuItem)
                    {
                        case 2: buf2 = to_string(pipes[buf1 - 1].id); break;
                        case 3: buf2 = to_string(pipes[buf1 - 1].lenght); break;
                        case 4: buf2 = to_string(pipes[buf1 - 1].diameter); break;
                        case 5: buf2 = pipes[buf1 - 1].signRepair; break;
                    }
                    cout << " ";
                    SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                    for (int i = 0; i < (buf2.size() - 1); ++i) { cout << " "; }
                    gotoxy(bufX, activeMenuItem);
                    ChangeConsoleColor(Blue, LightCyan);
                    switch (activeMenuItem)
                    {
                    case 2:
                        // Изменение id
                        while (true)
                        {
                            getline(cin, buf2);
                            if (СheckingNumbersStringInt(buf2))
                            {
                                pipes[buf1 - 1].id = stoi(buf2);
                                break;
                            }
                            else
                            {   // Выводим ошибку и заново делаем ввод
                                gotoxy(bufX + 5, activeMenuItem);
                                SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                                cout << "\t Введите целое положительное число!";
                                ChangeConsoleColor(Blue, LightCyan);
                                gotoxy(bufX, activeMenuItem);
                                buf2 = to_string(pipes[buf1 - 1].id); break;
                                cout << " ";
                                SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                                for (int i = 0; i < (buf2.size() - 1); ++i) { cout << " "; }
                                gotoxy(bufX, activeMenuItem);
                                ChangeConsoleColor(Blue, LightCyan);
                            }
                        }
                        break;
                    case 3:
                        // Изменение Длины
                        while (true)
                        {
                            getline(cin, buf2);
                            if (СheckingNumbersStringDouble(buf2))
                            {
                                pipes[buf1 - 1].lenght = stod(buf2);
                                break;
                            }
                            else
                            {   // Выводим ошибку и заново делаем ввод
                                gotoxy(bufX + 5, activeMenuItem);
                                SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                                cout << "\t Введите вещественное положительное число!";
                                ChangeConsoleColor(Blue, LightCyan);
                                gotoxy(bufX, activeMenuItem);
                                buf2 = to_string(pipes[buf1 - 1].lenght); break;
                                cout << " ";
                                SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                                for (int i = 0; i < (buf2.size() - 1); ++i) { cout << " "; }
                                gotoxy(bufX, activeMenuItem);
                                ChangeConsoleColor(Blue, LightCyan);
                            }
                        }
                        break;
                    case 4:
                        // Изменение Диаметра трубы
                        while (true)
                        {
                            getline(cin, buf2);
                            if (СheckingNumbersStringDouble(buf2))
                            {
                                pipes[buf1 - 1].diameter = stoi(buf2);
                                break;
                            }
                            else
                            {   // Выводим ошибку и заново делаем ввод
                                gotoxy(bufX + 5, activeMenuItem);
                                SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                                cout << "\t Введите вещественное положительное число!";
                                ChangeConsoleColor(Blue, LightCyan);
                                gotoxy(bufX, activeMenuItem);
                                buf2 = to_string(pipes[buf1 - 1].diameter); break;
                                cout << " ";
                                SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                                for (int i = 0; i < (buf2.size() - 1); ++i) { cout << " "; }
                                gotoxy(bufX, activeMenuItem);
                                ChangeConsoleColor(Blue, LightCyan);
                            }
                        }
                        break;
                    case 5:
                        bufKey = _getch();
                        if ((bufKey == 75) || (bufKey == 77))
                        {
                            SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                            cout << "          ";
                            gotoxy(bufX, activeMenuItem);
                            ChangeConsoleColor(LightCyan, Blue);
                            if (pipes[buf1 - 1].signRepair == "В ремонте")
                            {
                                cout << "Исправна";
                            }
                            else
                            {
                                cout << "В ремонте";
                            }


                        }
                        break;
                    }
                    break;
                }
            }
            if (flag1)
            {
                break;
            }
            // Обработка выхода за границы
            if (activeMenuItem < bufY)
            {
                activeMenuItem = bufY;
            }
            else if (activeMenuItem > bufY + (menuItems - 1))
            {
                activeMenuItem = bufY + (menuItems - 1);
            }
            // Красим назад перед переходом и возвращаем 
            SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
            gotoxy(bufX, bufActiveMenuItem);
            switch (bufActiveMenuItem)
            {
            case 2: cout << pipes[buf1 - 1].id; break;
            case 3: cout << pipes[buf1 - 1].lenght; break;
            case 4: cout << pipes[buf1 - 1].diameter; break;
            case 5: cout << pipes[buf1 - 1].signRepair; break;
            }
            // Переход
            gotoxy(bufX, activeMenuItem);
        }
        SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
    }
}

int main()
{
    // Инициализация массивов труб и КС-ок
    vector <Pipe> pipes; pipes.resize(0); vector <KC> KC; KC.resize(0);

    // Меню 
    vector <string> Menu = { "Добавить трубу",
                             "Добавить КС",
                             "Просмотр всех объектов",
                             "Редактировать трубу",
                             "Редактировать КС",
                             "Сохранить",
                             "Загрузить",
                             "Выход",
                             "435345",
                             "dfgdsfg"};

    // Включение русского языка в консоли
    setlocale(LC_CTYPE, "rus");

    // Выключаем курсор
    ShowConsoleCursor(false);

    // Переменная для сохранения кода клавиши
    int keyMenu;

    // Устанавливаем меню, находим первую строчку меню и скрываем курсор
    SetConsoleAttributes();
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdOut, &csbiInfo);
    int firstLineMenu; int activeMenu = (csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top) / 2 - Menu.size() / 2 ;
    ShowMenu(Menu, &firstLineMenu, activeMenu);
    ShowConsoleCursor(false);

    // Работа с меню
    while (true)
    {
        ShowMenu(Menu, activeMenu);
        ShowConsoleCursor(false);

        // Спрашиваем клавишу и считываем ее код
        keyMenu = _getch();
        if (keyMenu == -32)
            { keyMenu = _getch();}

        //Проверяем нажатую клавишу
        switch (keyMenu)
        {
            case ESC:
                exit(0);
                break;
            case DOWN:
                if (activeMenu < firstLineMenu + Menu.size() - 1)
                { ++activeMenu; }                                                              
                break;
            case UP:
                if (activeMenu > firstLineMenu)
                { --activeMenu; }
                break;
            case ENTER:
                ShowConsoleCursor(true);
                if (firstLineMenu + addPipe == activeMenu)
                {
                    AddPipe(pipes); // Добавить трубу
                }
                else if (firstLineMenu + addKC == activeMenu)
                {
                    AddKC(KC); // Добавить КС
                }
                else if (firstLineMenu + showObjects == activeMenu)
                {
                    ShowAllObjects(pipes, KC); // Показать все объекты
                }
                else if (firstLineMenu + editPipe == activeMenu)
                {
                    EditPipe(pipes); // Редактировать Трубу
                }
                else if (firstLineMenu + editKC == activeMenu)
                {
                   
                }
                else if (firstLineMenu + save == activeMenu)
                {
                    SaveData(pipes, KC); // Сохранить
                }
                else if (firstLineMenu + download == activeMenu)
                {
                    
                }
                else if (firstLineMenu + Exit == activeMenu)
                {
                    exit(0); // Выход 
                }
        }
    }
    return 0;
}

