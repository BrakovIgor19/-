// Лабораторная работа № 1
#define _CRT_SECURE_NO_WARNINGS // не помню зачем
#include <iostream>
#include <conio.h> // Для считывания клавиши
#include <clocale> // Для русской локализации
#include <vector> // Для динамического массива
#include <sstream> // Для getline()
#include <windows.h> // Для парса стринга в инты или даблы, для скрывания курсора
#include <fstream> // Для работы с файлами
#include <string> // Для работы со строками
#include <dos.h> // Для Sleep()
#include <mmsystem.h> // Для звука
#include <ctime> // Для рандома

using namespace std;

const string arrayErrors[8] =
{
    "Успешно!", // Код ошибки 0
    "Пустой ввод!", // Код ошибки 1
    "Введены Посторонние символы!", // Код ошибки 2
    "Ошибка ввода!", // Код ошибки 3
    "Количество рабочих цехов <= количеству всех цехов!", // Код ошибки 4
    "Эффективность КС лежит в промежутке (0% - 100%)!", // Код ошибки 5
    "Введите номер не превышающий количество всех объектов!", // Код ошибки 6
    "Нажмите Escape для выхода!" //  код ошибки 7
};

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
     RIGHT = 77, 
     LEFT = 75,
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
    int diameter; // Диаметр трубы
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
/// Создать рандомное число в промежутке
/// </summary>
/// <param name="left">Левая граница</param>
/// <param name="right">Правая граница</param>
/// <returns>Рандомное число в заданном промежутке</returns>
int RandomInt(int left, int right)
{
    srand(time(NULL));
    return left + (rand() % (right - left) + 1);
}

bool CheckingUniquenessID(const int &ID, const vector <Pipe> &pipes)
{
    for (int i = 0; i < pipes.size(); ++i)
    {
        if (ID == pipes[i].id)
        {
            return false;
        }
    }
    return true; 
}
bool CheckingUniquenessID(const int &ID, const vector <KC> &KC)
{
    for (int i = 0; i < KC.size(); ++i)
    {
        if (ID == KC[i].id)
        {
            return false;
        }
    }
    return true;
}

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

void ReadStringWithoutMovingCursor(string &str)
{
    int bufX, bufY;
    bufX = getXcoord(); bufY = getYcoord();
    getline(cin, str);
    gotoxy(bufX, bufY);
}

void AddColorGreen()
{
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfoEx(hConsole, &info);
    info.ColorTable[10] = RGB(19, 246, 118);
    info.ColorTable[2] = RGB(11, 102, 46);
    SetConsoleScreenBufferInfoEx(hConsole, &info);
}

/// <summary>
/// Написать текс по центру в указанной координате
/// </summary>
/// <param name="x">x</param>
/// <param name="textconst">текст</param>
/// <param name="y">y</param>
void WriteTextCenter(const string  &text, const int &x, const int &y)
{
    gotoxy(x - (text.size() / 2), y);
    cout << text;
}

void SkipSpacesStr(const string &str, int &i)
{
    // Пропускаем пробелы если они есть (Ищем индекс строки с которого надо считывать)
    while (true)
    {
        if ((str[i] == ' ') && (i < str.size()))
        {             
                ++i;   
        }
        else
        {
            break;
        }
    }
}

/// <summary>
/// Метод создает динамический двумерный массив и возвращает его адрес в памяти
/// </summary>
/// <param name="KC">Количество строк и столбцов массива</param>
string** CreateTwoDimensionalDynamicArrayStr(int lines, int columns)
{
    string** array = new string * [lines];
    for (int i = 0; i < lines; ++i)
    {
        array[i] = new string[columns];
    }
    return array;
}

void ClearTwoDimensionalDynamicArrayStr(string** array, int lines)
{
    for (int i = 0; i < lines; ++i)
    {
        delete[] array[i];
    }
    delete[] array;
}

void DrawCell(const int &width, const int &height, const char &symbol)
{
    int bufx, bufy;
    for (int i = 0; i < width; ++i)
    {
        cout << symbol;
    }
    bufx = getXcoord() - 1; bufy = getYcoord();
    for (int i = 0; i < height; ++i)
    {
        gotoxy(bufx, bufy + i);
        cout << symbol << endl;
    }
    gotoxy(bufx - (width - 1), bufy);
    bufx = getXcoord(); bufy = getYcoord();
    for (int i = 0; i < height; ++i)
    {
        gotoxy(bufx, bufy + i);
        cout << symbol << endl;
    }
    gotoxy(bufx, bufy + (height - 1));
    for (int i = 0; i < width; ++i)
    {
        cout << symbol;
    }
}

void DrawTable(const int &lines, const vector <int> &heightLines, const int &columns, const vector <int> &widthColumns, const int &left, const int &top, const char &symbol)
{
    int bufX1, bufY1, bufX2;
    gotoxy(left, top);
    for (int i = 0; i < lines; ++i)
    {
        bufX1 = getXcoord();
        bufY1 = getYcoord();
        for (int j = 0; j < columns; ++j)
        {
            bufX2 = getXcoord();
            DrawCell(widthColumns[j], heightLines[i], symbol);
            gotoxy(bufX2 + (widthColumns[j] - 1), bufY1);
        }
        gotoxy(bufX1, bufY1 + (heightLines[i] - 1));
    }
}

void FillTable(string** array, const vector <int> &heightLines, const vector <int> &widthColumns, const int &left, const int &top)
{
    int bufX1, bufY1, bufX2;
    gotoxy(left, top);
    for (int i = 0; i < heightLines.size(); ++i)
    {
        // Запоминаем кординаты Left и Top каждой строки таблицы
        bufX1 = getXcoord();
        bufY1 = getYcoord();

        // Заполняем строку
        for (int j = 0; j < widthColumns.size(); ++j)
        {
            // Запоминаем Left каждой ячейки 
            bufX2 = getXcoord();

            // Перемещаемся в центр клетки
            gotoxy(bufX2 + widthColumns[j] / 2, bufY1 + heightLines[i] / 2);

            // Распечатываем текст по центру клетки
            WriteTextCenter(array[i][j], getXcoord(), getYcoord());
            
            // Перемещаемся на Left следующей клетки
            gotoxy(bufX2 + (widthColumns[j] - 1), bufY1);
        }
        // Опускаемся на Top следующей строки
        gotoxy(bufX1, bufY1 + (heightLines[i] - 1));
    }
}

/// <summary>
/// Метод изменяет цвет текста и фона
/// </summary>
/// <param name="text">Цвет текста</param>
/// <param name="background">Цвет заднего фона</param>
void SetConsoleColor(ConsoleColor text, ConsoleColor background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //Получение дескриптора консоли
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));  //Изменение заднего фона и шрифта консоли
}

/// <summary>
/// Метод проверет строку на вещественное число
/// </summary>
/// <param name="">Строка для проверки</param>
/// <returns>Возвращает коды ошибок и -1 если их нет</returns>
int СheckingStringNumberReal(const string &str)
{
    //Пропускаем первые пробелы
    int i = 0;
    SkipSpacesStr(str, i);

    // Код ошибки 1 - пустой ввод
    if ((str.size() == 0) || (i == str.size()))
    {
        return 1;
    }

    // Код ошибки 3 - ошибка ввода
    if (((str[i] == '0') && (str[i + 1] == '0')) || (str[i] == ','))
    {
        return 3;
    }

    // Код ошибки 2 - содержание посторонних символов
    int k = 0;
    for (; i < str.size(); ++i)
    {
        // Пропускаем пробелы после числа
        if (str[i] == ' ')
        {
            SkipSpacesStr(str, i);
            if (i == str.size())
            {
                return 0;
            }
            else
            {
                return 3;
            }
        }

        if (str[i] == ',')
        {
            ++k;
            if (k > 1)
            {
                return 3;
            }
            continue;
        }
        if (isdigit(str[i]))
        {
            continue;
        }
        else
        {
            return 2;
        }
    }
    return 0;
}

/// <summary>
///Метод проверяет строчку на целое число
/// </summary>
/// <param name="str">Строка</param>
/// <returns>Возвращает коды ошибок и -1 если их нет</returns>
int СheckingStringNumberInt(const string &str)
{
    //Пропускаем первые пробелы
    int i = 0;
    SkipSpacesStr(str, i);

    // Код ошибки 1 - пустой ввод
    if ((str.size() == 0) || (i == str.size()))
    {
        return 1;
    }

    // Код ошибки 3 - ошибка ввода
    if (str[i] == '0')
    {
        return 3;
    }

    // Код ошибки 2 - содержание посторонних символов
    for (; i < str.size(); ++i)
    {
        // Пропускаем пробелы после числа
        if (str[i] == ' ')
        {
            SkipSpacesStr(str, i);
            if (i == str.size())
            {
                return 0;
            }
            else
            {
                return 3;
            }
        }
        if (isdigit(str[i]))
        {
            continue;
        }
        else
        {
            return 2;
        }
    }
    return 0;
}

int СheckingStringName(const string &str)
{
    //Пропускаем первые пробелы
    int i = 0;
    SkipSpacesStr(str, i);

    // Код ошибки 1 - пустой ввод
    if ((str.size() == 0) || (i == str.size()))
    {
        return 1;
    }

    // Пропускаем пробелы после имени
    if (str[i] == ' ')
    {
        SkipSpacesStr(str, i);
        if (i == str.size())
        {
            return 0;
        }
        else
        {
            return 3;
        }
    }

    return 0;
}

/// <summary>
/// Метод устанавливает размеры консоли по умолчанию
/// </summary>
void SetConsoleAttributes(const int width, const int height, ConsoleColor fontColor, ConsoleColor backgroundColor, const int fontSize)
{   
    AddColorGreen();
    // Получение дескриптора консоли
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // Высота и ширина консоли
    COORD crd = { width, height };
    COORD crdBuf = { width, height + 300 };
    // Левая верхняя и правая нижняя координата
    SMALL_RECT src = { 0, 0, crd.X - 1, crd.Y - 1 };
    // Установить размеры окна консоли
    SetConsoleWindowInfo(hStdOut, TRUE, &src);
    // Установить размеры буфера консоли
    SetConsoleScreenBufferSize(hStdOut, crdBuf);
    // Установить цвет шрифта и фона
    SetConsoleColor(fontColor, backgroundColor);
    // Получение текущего шрифта
    CONSOLE_FONT_INFOEX fontInfo;
    // эта строка нужна
    fontInfo.cbSize = sizeof(fontInfo);
    // Получить текущий шрифт
    GetCurrentConsoleFontEx(hStdOut, TRUE, &fontInfo);
    // Изменение шрифта и размера шрифта консоли
    wcscpy(fontInfo.FaceName, L"Lucida Console");
    fontInfo.dwFontSize.Y = fontSize;
    // Установить новый шрифт консоли
    SetCurrentConsoleFontEx(hStdOut, TRUE, &fontInfo); // Установить новый
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
void GetConsoleColors(HANDLE &hStdOut, CONSOLE_SCREEN_BUFFER_INFO &start_attribute)
{
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // Получение дескриптора консоли
    GetConsoleScreenBufferInfo(hStdOut, &start_attribute); // Запоминание начальных параметров
}

void ShowError(const int& errorCode, const string& str)
{
    if (errorCode == 0)
    {

        PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
        int bufX = getXcoord(), bufY = getYcoord();
        ShowConsoleCursor(false);
        gotoxy(bufX + str.size() + 5, bufY);
        SetConsoleColor(LightGreen, Black);
        cout << arrayErrors[errorCode];
        Sleep(1200);
        gotoxy(bufX, bufY);
        for (int i = 0; i < str.size() + 5 + arrayErrors[errorCode].size(); ++i) { cout << " "; }
        gotoxy(bufX, bufY);
        cout << str << endl;
        ShowConsoleCursor(true);
    }
    else
    {
        PlaySoundA("ui_hacking_passbad.wav", NULL, SND_ASYNC);
        int bufX = getXcoord(), bufY = getYcoord();
        ShowConsoleCursor(false);
        gotoxy(bufX + str.size() + 5, bufY);
        SetConsoleColor(LightGreen, Black);
        cout << arrayErrors[errorCode];
        Sleep(1200);
        gotoxy(bufX, bufY);
        for (int i = 0; i < str.size() + 5 + arrayErrors[errorCode].size(); ++i) { cout << " "; }
        gotoxy(bufX, bufY);
        ShowConsoleCursor(true);
    }
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
        ShowConsoleCursor(false);
        buf = _getch();
        if (buf == 27)
        {
            break;
        }
        else
        {
            ShowError(7, "");
        }
    }
    PlaySoundA("ui_menu_cancel.wav", NULL, SND_ASYNC);
}

/// <summary>
/// Метод для перехода на страницу с меню с возвратом первой строчки меню 
/// </summary>
void ShowMenu(vector <string> Menu, int* firstLineMenu, int bufActiveMenu)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdOut, &csbiInfo);
    int xCenter = csbiInfo.dwSize.X / 2; int yCenter = (csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top) / 2; int xBuf; // находим координаты х и y центра окна
    *firstLineMenu = yCenter - Menu.size() / 2; //Возвращаем первую строчку меню
    for (int i = 0; i < Menu.size(); ++i)
    {
        if (i + *firstLineMenu != bufActiveMenu)
            { SetConsoleColor(Green, Black); }
        else
            { SetConsoleColor(LightGreen, Black); }
        WriteTextCenter(Menu[i], xCenter, i + *firstLineMenu);

    }
    SetConsoleColor(LightGreen, Black);

}

/// <summary>
/// Метод для перехода на страницу с меню без возврата первой строчки меню
/// </summary>
void ShowMenu(vector <string> Menu, int bufActiveMenu, const int &firstLineMenu)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdOut, &csbiInfo);
    int xCenter = csbiInfo.dwSize.X / 2; int yCenter = firstLineMenu + Menu.size() / 2; int xBuf; // находим координаты х и y центра окна
    for (int i = 0; i < Menu.size(); ++i)
    {
        if (i + firstLineMenu != bufActiveMenu)
        {
            SetConsoleColor(Green, Black);
        }
        else
        {
            SetConsoleColor(LightGreen, Black);
        }
        WriteTextCenter(Menu[i], xCenter, i + firstLineMenu);
    }
    SetConsoleColor(LightGreen, Black);

}

/// <summary>
/// Метод добавляет трубу в вектор труб
/// </summary>
/// <param name="pipes">Массив труб</param>
void AddPipeInVector(vector <Pipe> &pipes)
{   
    // Очищаем экран перед добавление данных для трубы
    system("cls");

    // Буферные переменные
    int key, bufX, bufY; string strBuf; Pipe pipe; 
    cout << "\tТруба №" << pipes.size() + 1 << "\n\n";

    // Cоздание уникального id трубы
    int idBuf;
    while (true)
    {
        idBuf = RandomInt(1, 9999);
        if (CheckingUniquenessID(idBuf, pipes))
        {
            pipe.id = idBuf;
            break;
        }
    }

    // Добавление длины
    cout << " Длина: ";
    while (true)
    {   
        ReadStringWithoutMovingCursor(strBuf);
        if (СheckingStringNumberReal(strBuf) == 0)
        {
            ShowError(0, strBuf);
            pipe.lenght = stod(strBuf);
            break;
        }
        else
        {
            ShowError(СheckingStringNumberReal(strBuf), strBuf);
        }
    }

    // Добавление диаметра
    cout << " Диаметр: ";
    while (true)
    {
        ReadStringWithoutMovingCursor(strBuf);
        if (СheckingStringNumberInt(strBuf) == 0)
        {
            ShowError(0, strBuf);
            pipe.diameter = stoi(strBuf);
            break;
        }
        else
        {
            ShowError(СheckingStringNumberInt(strBuf), strBuf);
        }
    }

    // Добавление признака "в ремонте"
    cout << "\n Нажмите 'n' если труба в ремонте или 'y' если труба исправна";
    while (true)
    {
        ShowConsoleCursor(false);
        bufX = getXcoord(); bufY = getYcoord();
        key = _getch();
        gotoxy(bufX, bufY);
        if ((key == 110) || (key == 226))
        {
            ShowError(0, "");
            pipe.signRepair = "В ремонте";
            break;
        }
        else if ((key == 121) || (key == 173))
        {
            ShowError(0, "");
            pipe.signRepair = "Исправна";
            break;
        }
        else
        {
            ShowError(3, "");
        }
    }

    // Добавление трубы в вектор труб
    pipes.push_back(pipe);
    system("cls");
}

/// <summary>
/// Метод добавляет компрессорную станцию в вектор КСs
/// </summary>
/// <param name="pipes">Массив КС-ок</param>
void AddKCInVector(vector <KC> &KCs)
{   
    // Очищаем экран перед добавление данных для трубы
    system("cls");

    // Буферные переменные
    int key, buf, bufX, bufY; string strBuf; KC KC ;
    cout << "\tКомпрессорная станция №" << KCs.size() + 1 << "\n\n";

    // Cоздание уникального id KC
    int idBuf;
    while (true)
    {
        idBuf = RandomInt(1, 9999);
        if (CheckingUniquenessID(idBuf, KCs))
        {
            KC.id = idBuf;
            break;
        }
    }

    // Добавление Название
    cout << " Название: ";
    while (true)
    {
        ReadStringWithoutMovingCursor(strBuf);
        if (СheckingStringName(strBuf) == 0)
        {
            ShowError(0, strBuf);
            KC.name = strBuf;
            break;
        }
        else
        {
            ShowError(СheckingStringName(strBuf), strBuf);
        }
    }

    // Добавление количества цехов
    cout << " Количество цехов: ";
    while (true)
    {
        ReadStringWithoutMovingCursor(strBuf);
        if (СheckingStringNumberInt(strBuf) == 0)
        {
            ShowError(0, strBuf);
            KC.numberWorkshops = stoi(strBuf);
            break;
        }
        else
        {
            ShowError(СheckingStringNumberInt(strBuf), strBuf);
        }        
    }

    // Добавление количества цехов в работе
    cout << " Количество цехов в работе: ";
    while (true)
    {
        ReadStringWithoutMovingCursor(strBuf);
        if (СheckingStringNumberInt(strBuf) == 0)
        {
            buf = stoi(strBuf);
            if (buf <= KC.numberWorkshops)
            {
                ShowError(0, strBuf);
                KC.numberWorkshopsOperation = buf;
                break;
            }
            else
            {
                ShowError(4, strBuf);
            }
        }
        else
        {
            ShowError(СheckingStringNumberInt(strBuf), strBuf);
        }
    }

    // Добавление эффективности
    cout << " Эффективность станции: ";
    while (true)
    {
        ReadStringWithoutMovingCursor(strBuf);
        if (СheckingStringNumberInt(strBuf) == 0)
        {
            buf = stoi(strBuf);
            if ((buf >= 0) && (buf <= 100))
            {
                ShowError(0, strBuf);
                KC.effectiveness = buf;
                break;
            }
            else
            {
                ShowError(5, strBuf);
            }
        }
        else
        {
            ShowError(СheckingStringNumberInt(strBuf), strBuf);
        }
    }

    // Добавление КС в вектор KCs
    KCs.push_back(KC);
    system("cls");
}

/// <summary>
/// Метод показывает все объекты
/// </summary>
/// <returns></returns>
void ShowAllObjects(const vector <Pipe>& pipes, const vector <KC>& KC)
{
    system("cls");

    // Данные из структуры переводим в двумерный массив и подготавливаем размеры строк и столбцов таблицы
    string** array1 = CreateTwoDimensionalDynamicArrayStr(pipes.size() + 1, 5);
    array1[0][0] = "Номер"; array1[0][1] = "id"; array1[0][2] = "Длина"; array1[0][3] = "Диаметр"; array1[0][4] = "Признак 'в ремонте'";
    for (int i = 1; i < pipes.size() + 1; ++i) { array1[i][0] = to_string(i); }
    for (int i = 1; i < pipes.size() + 1; ++i) { array1[i][1] = to_string(pipes[i - 1].id); }
    for (int i = 1; i < pipes.size() + 1; ++i) { array1[i][2] = to_string(pipes[i - 1].lenght); }
    for (int i = 1; i < pipes.size() + 1; ++i) { array1[i][3] = to_string(pipes[i - 1].diameter); }
    for (int i = 1; i < pipes.size() + 1; ++i) { array1[i][4] = pipes[i - 1].signRepair; }
    vector <int> heightLines; heightLines.resize(pipes.size() + 1); for (int i = 0; i < pipes.size() + 1; ++i) { heightLines[i] = 5; }
    vector <int> widthColumns; widthColumns.resize(5); widthColumns[0] = 9; widthColumns[1] = 8; widthColumns[2] = 15; widthColumns[3] = 15; widthColumns[4] = 25;

    // Рисуем трубы
    DrawTable(pipes.size() + 1, heightLines, 5, widthColumns, 2, 2, '#');
    FillTable(array1, heightLines, widthColumns, 2, 2);

    // Очищаем память от вспомогательного массива
    ClearTwoDimensionalDynamicArrayStr(array1, pipes.size() + 1);

    // Перемещаемся в свободное место и запоминаем координаты
    gotoxy(2, 2 + 5 * (pipes.size() + 1) + 3);
    int bufX = getXcoord(), bufY = getYcoord();

    // Данные из структуры переводим в двумерный массив и подготавливаем размеры строк и столбцов таблицы
    string** array2 = CreateTwoDimensionalDynamicArrayStr(KC.size() + 1, 6);
    array2[0][0] = "Номер"; array2[0][1] = "id"; array2[0][2] = "Название"; array2[0][3] = "Кол-во цехов"; array2[0][4] = "Рабочих цехов"; array2[0][5] = "Эффективность";
    for (int i = 1; i < KC.size() + 1; ++i) { array2[i][0] = to_string(i); }
    for (int i = 1; i < KC.size() + 1; ++i) { array2[i][1] = to_string(KC[i - 1].id); }
    for (int i = 1; i < KC.size() + 1; ++i) { array2[i][2] = KC[i - 1].name; }
    for (int i = 1; i < KC.size() + 1; ++i) { array2[i][3] = to_string(KC[i - 1].numberWorkshops); }
    for (int i = 1; i < KC.size() + 1; ++i) { array2[i][4] = to_string(KC[i - 1].numberWorkshopsOperation); }
    for (int i = 1; i < KC.size() + 1; ++i) { array2[i][5] = to_string(KC[i - 1].effectiveness); }
    vector <int> heightLines1; heightLines1.resize(KC.size() + 1); for (int i = 0; i < KC.size() + 1; ++i) { heightLines1[i] = 5; }
    vector <int> widthColumns1; widthColumns1.resize(6); widthColumns1[0] = 9; widthColumns1[1] = 8; widthColumns1[2] = 15; widthColumns1[3] = 18; widthColumns1[4] = 19; widthColumns1[5] = 17;

    // Рисуем КС-ки
    DrawTable(KC.size() + 1, heightLines1, 6, widthColumns1, bufX, bufY, '#');
    FillTable(array2, heightLines1, widthColumns1, bufX, bufY);

    // Очищаем память от вспомогательного массива
    ClearTwoDimensionalDynamicArrayStr(array2, KC.size() + 1);

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

/// <summary>
/// Метод редактирует одну выбранную трубу
/// </summary>
/// <param name="pipes">Массив труб</param>
void EditPipe(vector <Pipe>& pipes)
{
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO start_attribute;
    bool flag1 = false; int activeMenuItem, bufActiveMenuItem, key, numberPipe, bufX, bufY; string buf2; const int menuItems = 4;
    system("cls");
    cout << " Количество труб: " << pipes.size() << "\n\n";

    // Проверяем наличие труб
    if (pipes.size() == 0)
    {
        cout << " Трубы еще не подвезли!";
        BackMenu();
        system("cls");
        return;
    }
    

    // Спрашиваем номер трубы и возвращаем номер трубы "buf1"
    cout << " Введите номер трубы: ";
    while (true)
    {
        ReadStringWithoutMovingCursor(buf2);
        if (СheckingStringNumberInt(buf2) == 0)
        {
            numberPipe = stoi(buf2);
            if (numberPipe <= pipes.size())
            {
                break;
            }
            else
            {
                ShowError(6, buf2);
            }
        }
        else
        {
            ShowError(СheckingStringNumberInt(buf2), buf2);
        } 
    }

    // Выводим данные по данной трубе и иструкции для пользователя 
    system("cls");
    cout << "\t                 Труба № " << numberPipe << endl
        << "\n\t                 id: "; bufX = getXcoord(); bufY = getYcoord(); cout << pipes[numberPipe - 1].id << endl
        << "\t              Длина: " << pipes[numberPipe - 1].lenght << endl
        << "\t            Диаметр: " << pipes[numberPipe - 1].diameter << endl
        << "\tПризнак 'в ремонте': " << pipes[numberPipe - 1].signRepair << endl;
    gotoxy(0, 8);
    cout << "Используйте стрелки 'Вверх' и 'Вниз' для перемещения по данным" << endl
        << "Чтобы изменить данные и ввести новые нажмите 'Enter' " << endl
        << "Чтобы изменить признак 'в ремонте' выберите этот пункт и стрелками выберете состояние " << endl
        << "Для выхода в меню нажмите 'Escape'";
    gotoxy(bufX, bufY);
    activeMenuItem = bufY;
    ShowConsoleCursor(false);

    // Работа с данными
    while (true)
    {
        ShowConsoleCursor(false);

        // Запоминаем цвет консоли
        GetConsoleColors(hStdOut, start_attribute);
        bufActiveMenuItem = activeMenuItem;

        // Перекрашиваем данную клетку
        SetConsoleColor(Black, LightGreen);
        switch (activeMenuItem)
        {
            case 2: cout << pipes[numberPipe - 1].id; break;
            case 3: cout << pipes[numberPipe - 1].lenght; break;
            case 4: cout << pipes[numberPipe - 1].diameter; break;
            case 5: cout << pipes[numberPipe - 1].signRepair; break;
        }
        gotoxy(bufX, activeMenuItem);

        // Считывание   кода стрелки
        key = _getch();
        if (key == -32)
        {
            key = _getch();
        }
        // Обработка введенной клавиши
        switch (key)
        {
            case ESC: PlaySoundA("ui_menu_cancel.wav", NULL, SND_ASYNC);  flag1 = true; break; // Клавиша Escape
            case UP:  PlaySoundA("ui_pipboy_highlight.wav", NULL, SND_ASYNC);  --activeMenuItem; break; // Клавиша стрелка вверх
            case DOWN:  PlaySoundA("ui_pipboy_highlight.wav", NULL, SND_ASYNC);  ++activeMenuItem; break; // Клавиша стрелка вниз
            case ENTER:  PlaySoundA("ui_pipboy_select.wav", NULL, SND_ASYNC); // Клавиша Enter
                // Вводим новые данные с проверкой
            {

                // Приводим к виду ввода
                gotoxy(bufX, activeMenuItem);
                SetConsoleColor(LightGreen, Black);
                switch (activeMenuItem)
                {
                    case 2: buf2 = to_string(pipes[numberPipe - 1].id); break;
                    case 3: buf2 = to_string(pipes[numberPipe - 1].lenght); break;
                    case 4: buf2 = to_string(pipes[numberPipe - 1].diameter); break;
                    case 5: buf2 = pipes[numberPipe - 1].signRepair; break;
                }
                for (int i = 0; i < buf2.size(); ++i) { cout << " "; }
                gotoxy(bufX, activeMenuItem);
                SetConsoleColor(Black, LightGreen);
                ShowConsoleCursor(true);
                switch (activeMenuItem)
                {
                case 2:
                    // Изменение id
                    while (true)
                    {
                        ReadStringWithoutMovingCursor(buf2);
                        ShowConsoleCursor(false);
                        if (СheckingStringNumberInt(buf2) == 0)
                        {
                            PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                            pipes[numberPipe - 1].id = stoi(buf2);
                            gotoxy(bufX, activeMenuItem);
                            ShowConsoleCursor(false);
                            break;
                        }
                        else
                        { 
                            // Выводим ошибку и заново делаем ввод
                            ShowError(СheckingStringNumberInt(buf2), buf2);
                            SetConsoleColor(Black, LightGreen);
                            cout << pipes[numberPipe - 1].id;
                            gotoxy(bufX, activeMenuItem);
                            break;
                        }
                    }
                    break;
                case 3:
                    // Изменение Длины
                    while (true)
                    {
                        ReadStringWithoutMovingCursor(buf2);
                        ShowConsoleCursor(false);
                        if (СheckingStringNumberReal(buf2) == 0)
                        {
                            PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                            pipes[numberPipe - 1].lenght = stod(buf2);
                            gotoxy(bufX, activeMenuItem);
                            ShowConsoleCursor(false);
                            break;
                        }
                        else
                        { 
                            // Выводим ошибку и заново делаем ввод
                            ShowError(СheckingStringNumberReal(buf2), buf2);
                            SetConsoleColor(Black, LightGreen);
                            cout << pipes[numberPipe - 1].lenght;
                            gotoxy(bufX, activeMenuItem);
                            break;
                        }
                    }
                    break;
                case 4:
                    // Изменение Диаметра трубы
                    while (true)
                    {
                        ReadStringWithoutMovingCursor(buf2);
                        ShowConsoleCursor(false);
                        if (СheckingStringNumberInt(buf2) == 0)
                        {
                            PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                            pipes[numberPipe - 1].diameter = stod(buf2);
                            gotoxy(bufX, activeMenuItem);
                            ShowConsoleCursor(false);
                            break;
                        }
                        else
                        {
                            // Выводим ошибку и заново делаем ввод
                            ShowError(СheckingStringNumberInt(buf2), buf2);
                            SetConsoleColor(Black, LightGreen);
                            cout << pipes[numberPipe - 1].diameter;
                            gotoxy(bufX, activeMenuItem);
                            break;
                        }
                    }
                    break;
                }
                break;
            }
            case LEFT:
                if (activeMenuItem == 5)
                {
                    PlaySoundA("ui_repairweapon_02.wav", NULL, SND_ASYNC);
                    SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                    cout << "          ";
                    gotoxy(bufX, activeMenuItem);
                    SetConsoleColor(Black, LightGreen);
                    if (pipes[numberPipe - 1].signRepair == "В ремонте")
                    {
                        pipes[numberPipe - 1].signRepair = "Исправна";
                        cout << pipes[numberPipe - 1].signRepair;
                    }
                    else
                    {
                        pipes[numberPipe - 1].signRepair = "В ремонте";
                        cout << pipes[numberPipe - 1].signRepair;
                    }
                    gotoxy(bufX, activeMenuItem);
                }
                break;
            case RIGHT:

                if (activeMenuItem == 5)
                {
                    PlaySoundA("ui_repairweapon_02.wav", NULL, SND_ASYNC);
                    SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
                    cout << "          ";
                    gotoxy(bufX, activeMenuItem);
                    SetConsoleColor(Black, LightGreen);
                    if (pipes[numberPipe - 1].signRepair == "В ремонте")
                    {
                        pipes[numberPipe - 1].signRepair = "Исправна";
                        cout << pipes[numberPipe - 1].signRepair;
                    }
                    else
                    {
                        pipes[numberPipe - 1].signRepair = "В ремонте";
                        cout << pipes[numberPipe - 1].signRepair;
                    }
                    gotoxy(bufX, activeMenuItem);
                }
                break;
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
        case 2: cout << pipes[numberPipe - 1].id; break;
        case 3: cout << pipes[numberPipe - 1].lenght; break;
        case 4: cout << pipes[numberPipe - 1].diameter; break;
        case 5: cout << pipes[numberPipe - 1].signRepair; break;
        }
        // Переход
        gotoxy(bufX, activeMenuItem);
    }

    SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);    
    system("cls");
}

/// <summary>
/// Метод редактирует одну выбранную КС-ку
/// </summary>
/// <param name="KC">Массив КС-ок</param>
void EditKC(vector <KC>& KC)
{
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO start_attribute;
    bool flag1 = false; int buf3, activeMenuItem, bufActiveMenuItem, key, numberKC, bufX, bufY; string buf2; const int menuItems = 5;
    system("cls");
    cout << " Количество компрессорных станций: " << KC.size() << "\n\n";

    // Проверяем наличие труб
    if (KC.size() == 0)
    {
        cout << " Компрессорные станции ещё не построены!";
        BackMenu();
        system("cls");
        return;
    }

    // Спрашиваем номер трубы и возвращаем номер трубы "buf1"
    cout << " Введите номер компрессорной станции: ";
    while (true)
    {
        ReadStringWithoutMovingCursor(buf2);
        if (СheckingStringNumberInt(buf2) == 0)
        {
            numberKC = stoi(buf2);
            if (numberKC <= buf2.size())
            {
                break;
            }
            else
            {
                ShowError(6, buf2);
            }
        }
        else
        {
            ShowError(СheckingStringNumberInt(buf2), buf2);
        }
    } 
    // Выводим данные по данной КС и иструкции для пользователя 
    system("cls");
    cout << "\t                    Компрессорная стация № " << numberKC << endl
        << "\n\t                      id: "; bufX = getXcoord(); bufY = getYcoord(); cout << KC[numberKC - 1].id << endl
        << "\t                Название: " << KC[numberKC - 1].name << endl
        << "\t            Кол-во цехов: " << KC[numberKC - 1].numberWorkshops << endl
        << "\tКоличество рабочих цехов: " << KC[numberKC - 1].numberWorkshopsOperation << endl
        << "\t           Эффективность: " << KC[numberKC - 1].effectiveness << endl;
    gotoxy(0, 8);
    cout << "Используйте стрелки 'Вверх' и 'Вниз' для перемещения по данным" << endl
        << "Чтобы изменить данные и ввести новые нажмите 'Enter' " << endl
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
        ShowConsoleCursor(false);

        // Перекрашиваем данную клетку
        SetConsoleColor(Black, LightGreen);
        switch (activeMenuItem)
        {
        case 2: cout << KC[numberKC - 1].id; break;
        case 3: cout << KC[numberKC - 1].name; break;
        case 4: cout << KC[numberKC - 1].numberWorkshops; break;
        case 5: cout << KC[numberKC - 1].numberWorkshopsOperation; break;
        case 6: cout << KC[numberKC - 1].effectiveness; break;
        }
        gotoxy(bufX, activeMenuItem);

        // Считывание   кода стрелки
        key = _getch();
        if (key == -32)
        {
            key = _getch();
        }
        // Обработа введенной клавиши
        switch (key)
        {
        case ESC: PlaySoundA("ui_menu_cancel.wav", NULL, SND_ASYNC);  flag1 = true; break; // Клавиша Escape
        case UP:  PlaySoundA("ui_pipboy_highlight", NULL, SND_ASYNC); --activeMenuItem; break; // Клавиша стрелка вверх
        case DOWN:  PlaySoundA("ui_pipboy_highlight", NULL, SND_ASYNC); ++activeMenuItem; break; // Клавиша стрелка вниз
        case ENTER:  PlaySoundA("ui_pipboy_select.wav", NULL, SND_ASYNC); // Клавиша Enter
        // Вводим новые данные с проверкой
        {

            // Приводим к виду ввода
            gotoxy(bufX, activeMenuItem);
            SetConsoleColor(LightGreen, Black);
            switch (activeMenuItem)
            {
            case 2: buf2 = to_string(KC[numberKC - 1].id); break;
            case 3: buf2 = KC[numberKC - 1].name; break;
            case 4: buf2 = to_string(KC[numberKC - 1].numberWorkshops); break;
            case 5: buf2 = to_string(KC[numberKC - 1].numberWorkshopsOperation); break;
            case 6: buf2 = to_string(KC[numberKC - 1].effectiveness); break;
            }
            for (int i = 0; i < buf2.size(); ++i) { cout << " "; }
            gotoxy(bufX, activeMenuItem);
            SetConsoleColor(Black, LightGreen);
            ShowConsoleCursor(true);
            switch (activeMenuItem)
            {
            case 2:
                // Изменение id
                while (true)
                {
                    ReadStringWithoutMovingCursor(buf2);
                    ShowConsoleCursor(false);
                    if (СheckingStringNumberInt(buf2) == 0)
                    {
                        PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                        KC[numberKC - 1].id = stoi(buf2);
                        gotoxy(bufX, activeMenuItem);
                        ShowConsoleCursor(false);
                        break;
                    }
                    else
                    {
                        // Выводим ошибку и заново делаем ввод
                        ShowError(СheckingStringNumberInt(buf2), buf2);
                        SetConsoleColor(Black, LightGreen);
                        cout << KC[numberKC - 1].id;
                        gotoxy(bufX, activeMenuItem);
                        break;
                    }
                    
                }
                break;
            case 3:
                // Изменение Имени
                while (true)
                {
                    getline(cin, buf2);
                    PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                    KC[numberKC - 1].name = buf2;
                    gotoxy(bufX, activeMenuItem);
                    ShowConsoleCursor(false);
                    break;                                                
                }
                break;
            case 4:
                // Изменение Количества цехов КС
                while (true)
                {
                    ReadStringWithoutMovingCursor(buf2);
                    ShowConsoleCursor(false);
                    if (СheckingStringNumberInt(buf2) == 0)
                    {
                        PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                        KC[numberKC - 1].numberWorkshops = stoi(buf2);
                        gotoxy(bufX, activeMenuItem);
                        ShowConsoleCursor(false);
                        break;
                    }
                    else
                    {
                        // Выводим ошибку и заново делаем ввод
                        ShowError(СheckingStringNumberInt(buf2), buf2);
                        SetConsoleColor(Black, LightGreen);
                        cout << KC[numberKC - 1].numberWorkshops;
                        gotoxy(bufX, activeMenuItem);
                        break;
                    }
                }
                break;
            case 5:
                // Изменение Количества рабочих цехов КС
                while (true)
                {
                    ReadStringWithoutMovingCursor(buf2);
                    ShowConsoleCursor(false);
                    if (СheckingStringNumberReal(buf2) == 0)
                    {
                        buf3 = stoi(buf2);
                        if (buf3 <= KC[numberKC - 1].numberWorkshopsOperation)
                        {
                            PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                            KC[numberKC - 1].numberWorkshopsOperation = stoi(buf2);
                            gotoxy(bufX, activeMenuItem);
                            ShowConsoleCursor(false);
                            break;
                        }
                        else
                        { 
                            // Выводим ошибку на меньшее количество цехов и заново делаем ввод
                            ShowError(6, buf2);
                            SetConsoleColor(Black, LightGreen);
                            cout << KC[numberKC - 1].numberWorkshopsOperation;
                            gotoxy(bufX, activeMenuItem);
                            break;
                        }
                    }
                    else
                    {
                        // Выводим ошибку и заново делаем ввод
                        ShowError(СheckingStringNumberInt(buf2), buf2);
                        SetConsoleColor(Black, LightGreen);
                        cout << KC[numberKC - 1].numberWorkshopsOperation;
                        gotoxy(bufX, activeMenuItem);
                        break;
                    }
                }
                break;
            case 6:
                // Изменение Эффективности цеха
                while (true)
                {
                    ReadStringWithoutMovingCursor(buf2);
                    ShowConsoleCursor(false);
                    if (СheckingStringNumberInt(buf2) == 0)
                    {
                        buf3 = stoi(buf2);
                        if ((buf3 >= 0) && (buf3 <= 100))
                        {
                            PlaySoundA("ui_hacking_passgood.wav", NULL, SND_ASYNC);
                            KC[numberKC - 1].numberWorkshopsOperation = stoi(buf2);
                            gotoxy(bufX, activeMenuItem);
                            ShowConsoleCursor(false);
                            break;
                        }
                        else
                        {
                            // Выводим ошибку на меньшее количество цехов и заново делаем ввод
                            ShowError(5, buf2);
                            SetConsoleColor(Black, LightGreen);
                            cout << KC[numberKC - 1].numberWorkshopsOperation;
                            gotoxy(bufX, activeMenuItem);
                            break;
                        }
                    }
                    else
                    {
                        // Выводим ошибку и заново делаем ввод
                        ShowError(СheckingStringNumberInt(buf2), buf2);
                        SetConsoleColor(Black, LightGreen);
                        cout << KC[numberKC - 1].effectiveness;
                        gotoxy(bufX, activeMenuItem);
                        break;
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
        case 2: cout << KC[numberKC - 1].id; break;
        case 3: cout << KC[numberKC - 1].name; break;
        case 4: cout << KC[numberKC - 1].numberWorkshops; break;
        case 5: cout << KC[numberKC - 1].numberWorkshopsOperation; break;
        case 6: cout << KC[numberKC - 1].effectiveness; break;
        }
        // Переход
        gotoxy(bufX, activeMenuItem);
    }
    SetConsoleTextAttribute(hStdOut, start_attribute.wAttributes);
    system("cls");
}

int main()
{
    // Инициализация массивов труб и КС-ок
    vector <Pipe> pipes; pipes.resize(0); vector <KC> KC; KC.resize(0);

    // Меню и задаем название меню 
    vector <string> Menu = { "Добавить трубу",
                             "Добавить КС",
                             "Просмотр всех объектов",
                             "Редактировать трубу",
                             "Редактировать КС",
                             "Сохранить",
                             "Загрузить",
                             "Выход", };
    SetConsoleTitle(L"Лабораторная работа №1 Баркова Игоря - создание модели трубопроводного транспорта газа или нефти");

    // Включение русского языка в консоли
    setlocale(LC_CTYPE, "rus");

    // Переменная для сохранения кода клавиши
    int keyMenu;

    // Устанавливаем меню, находим первую строчку меню и скрываем курсор
    SetConsoleAttributes(87, 27, LightGreen, Black, 25);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdOut, &csbiInfo);
    int firstLineMenu; int activeStr = (csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top) / 2 - Menu.size() / 2; int activeMenu;
    ShowMenu(Menu, &firstLineMenu, activeStr);
    ShowConsoleCursor(false);

    // Работа с меню
    while (true)
    {
        ShowMenu(Menu, activeStr, firstLineMenu);
        ShowConsoleCursor(false);

        // Спрашиваем клавишу и считываем ее код
        keyMenu = _getch();
        if (keyMenu == -32)
            { keyMenu = _getch();}

        //Проверяем нажатую клавишу
        switch (keyMenu)
        {
            case ESC:
                PlaySoundA("ui_menu_cancel.wav", NULL, SND_ASYNC);
                Sleep(1000);
                exit(0);
                break;
            case DOWN:
                PlaySoundA("ui_menu_prevnext.wav", NULL, SND_ASYNC);
                if (activeStr < firstLineMenu + Menu.size() - 1)
                { ++activeStr; }
                break;
            case UP:
                PlaySoundA("ui_menu_prevnext.wav", NULL, SND_ASYNC);
                if (activeStr > firstLineMenu)
                { --activeStr; }
                break;
            case ENTER:
                PlaySoundA("ui_pipboy_select.wav", NULL, SND_ASYNC);
                ShowConsoleCursor(true);
                activeMenu = activeStr - firstLineMenu;
                switch (activeMenu)
                {
                case addPipe:
                        AddPipeInVector(pipes); // Добавить трубу
                        break;
                case addKC:
                        AddKCInVector(KC); // Добавить КС
                        break;
                case showObjects:
                        ShowAllObjects(pipes, KC); // Показать все объекты
                        break;
                case editPipe:
                        EditPipe(pipes); // Редактировать Трубу
                        break;
                case editKC:   
                        EditKC(KC); // Редактировать КС
                        break;
                case save:
                        SaveData(pipes, KC); // Сохранить
                        break;
                case download: 

                case Exit:
                        PlaySoundA("ui_menu_cancel.wav", NULL, SND_ASYNC);
                        Sleep(1000);
                        exit(0); // Выход 
                }                
        }
    }
    return 0;
}

