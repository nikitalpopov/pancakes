//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//
//  11. Задача о блинах
//  Есть сковорода известного диаметра. Есть тесто в кастрюле известного объема.
//  Известно, сколько теста набирают в поварешку для каждого будущего блина.
//  Известно, в какую точку на сковороде выливают это тесто. Известно, что тесто растекается
//  в форме круга, если ему на пути не встречаются другие блины и стенки сковороды.
//  Известно, что тесто не растекается поверх уже жарящихся блинов.
//  Известно, что блины при этом получаются одной (известной) толщины.
//  Известно, что при заполнении известной части сковороды блинами добавление новых блинов
//  будет возможно только после выпекания всех уже размещенных.
//
//  Каково количество подходов заполнения сковороды блинами?
//  Каков процент идеальных (круглых) блинов?
//
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//
//  main.cpp
//  Pancakes
//
//  Created by Никита Попов on 09.12.14.
//  Copyright (c) 2014 Никита Попов. All rights reserved.
//
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//
//  Описание ошибок ("Program ended with exit code: *k*")
//  1 : Не открывается файл с исходными данными
//  2 : Не открывается файл с выходными данными
//  3 : Две точки или два минуса в числе в исходных данных
//  4 : Символ, отличный от цифр, "-", ".", " " в исходных данных
//
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <string>

using namespace std;

//---------------------------------------------------------------------------------------------

void Is_Float(const char * argv)
{
    fstream input(argv);
    int Minus = 0; // Кол-во минусов в строке
    int Dot = 0;   // Кол-во точек в строке
    // int Space = 0; // Кол-во пробелов в строке
    string str;
    for(int k = 0; k < 20; ++k)
    {
        while (!input.eof())
        {
            stringstream ss;
            char c;
            while(input.get(c))
            {
                if(((c >= '0') && (c <= '9')) || (c == '-') || (c == '.') || (c == '\n') || (c == ' '))
                {
                    
                    if(c == '\n')
                    {
                        Minus = 0;
                        Dot = 0;
                    }
                    if(c == '-')
                    {
                        ++Minus;
                    }
                    if(c == '.')
                    {
                        ++Dot;
                    }
                    if((Dot > 1) || (Minus > 1))
                    {
                        cout << "Обнаружена ошибка! Проверьте входные данные!\n";
                        exit(3);
                    }
                    ss << ((isdigit(c) || (c == '-') || (c == '.')) ? c : ' ');
                    continue;
                }
                else
                {
                    cout << "Обнаружена ошибка! Проверьте входные данные!\n";
                    exit(4);
                }
            }
            
            while(ss >> str)
            {

            }
        }
    }
    input.close();
}

float F1(float R, float r, float D)
{
    return 2.0 * acos((R * R - r * r + D * D) / (2.0 * R * D));
}

float F2(float R, float r, float D)
{
    return 2.0 * acos((r * r - R * R + D * D) / (2.0 * r * D));
}

float NewRadius(float R, float r, float D, float Square)
{
    bool i = true;
    while(i)
    {
        float Square1 = (((R * R * F1(R, r, D)) - sin(F1(R, r, D))) / 2.0) + (((r * r * F2(R, r, D)) - sin(F2(R, r, D))) / 2.0);
        if(Square1 > Square) i = false;
        r += 0.0001;
    }
    return r;
}

//---------------------------------------------------------------------------------------------

int main(int argc, const char * argv[])
{
    Is_Float(argv[1]);
    
    // Подключаем файлы как параметры строки
    fstream input(argv[1]);
    fstream output(argv[2], ios_base::out | ios_base::trunc);
    {
        if(!input.is_open())
        {
            cout << "ERROR! Can't open input file!\n";
            exit(1);
        }
        else if(!output.is_open())
        {
            cout << "ERROR! Can't open output file!\n";
            exit(2);
        }
    } // Проверка на доступность файлов
    
    // Объявляем переменные
    float Diametr = 0;                    // Диаметр сковороды
    float Height = 0;                     // Толщина блина
    float TotalVolume = 0;                // Объем кастрюли
    float TotalVolume1 = 0;
    float *SingleVolume = new float[100]; // Объем поварешки
    float *X = new float[100];            // Координаты
    float *Y = new float[100];            // центра блина
    float Ideal = 0;                      // Кол-во идеальных блинов
    float StepVolume = 0;                 // Свободное место на сковороде
                                          // в этом подходе
    int Step = 0;                         // Кол-во подходов
    int Total = 0;                        // Всего блинов
    int Pancakes_Per_Step = 0;            // Кол-во блинов, выпеченных за подход
    int Counter = 0;                      // Счетчик
    int Counter1 = 0;                     // Вспомогательный счетчик
    float  Pi = 3.14159265358979;         // Число π
    
    // Считываем переменные из файла
    input >> Diametr;
    input >> Height;
    input >> TotalVolume;
    int i = TotalVolume;
    TotalVolume1 = TotalVolume;
    while(!input.eof())
    {
        input >> SingleVolume[Counter];
        if(TotalVolume1 - SingleVolume[Counter] < 0)
        {
            break;
        }
        TotalVolume1 = TotalVolume1 - SingleVolume[Counter];
        input >> X[Counter] >> Y[Counter];
        i = i - SingleVolume[Counter];
        ++Counter;
    }

    // Дополнительной объявление переменных и присвоение значений
    Counter1 = Counter;
    TotalVolume1 = TotalVolume / Height + 0.001;
    float *Radius = new float[Counter1];   // Массив со значениями радиуса каждого нового блина
    for(int i = 0; i < Counter1; ++i)
    {
        Radius[i] = 0;
    }
    float *Usage = new float[2 * Counter1]; // Массив  со значениями использования блина (0 - нет, 1 - да)
    for(int i = 0; i < 2 * Counter1; ++i)   // Первые Counter1 элементов - значения в текущем круге,
    {                                       // вторые Counter1 элементов - значения в общем
        Usage[i] = 0;
    }
    
    // Основной алгоритм
    output << argv[1] << endl;
    while(Counter > 0)
    {
        Pancakes_Per_Step = 0;
        StepVolume = TotalVolume / Height;
        for(int i = 0; i < Counter1; ++i)
        {
            Usage[i] = 0;
        }
        
        for(int k = 0; k < Counter1; ++k)
        {
            SingleVolume[k] = SingleVolume[k] / Height; // Находим площадь каждого блина
            Radius[k] = sqrt(SingleVolume[k] / Pi);     // Находим радиус этого круга (при условии, что
                                                        // он не пересекается стенками сковороды)
            if((X[k] * X[k] + Y[k] * Y[k]) >= (Diametr * Diametr / 4.0) || (Radius[k] >= Diametr / 2.0))
            {
                Usage[k] = 1; // Невозможно выпечь блин
                Usage[k + Counter1] = 1;
            }

            for(int i = 0; i < k; ++i)
            {
                if((Usage[i] == 1) && (Usage[i + Counter1] == 1))
                {
                     if(((pow(X[i] - X[k], 2) + pow(Y[i] - Y[k], 2)) <= pow(Radius[i] + Radius[k], 2) || ((pow((X[i] - X[k]), 2) + pow((X[i] - X[k]), 2)) == 0 && Radius[i] == Radius[k])))
                    {
                        Usage[k] = 1; // Если блин попадает на уже жарящийся блин
                    }
                }
                else
                {
                    continue;
                }
            }
            
            if(TotalVolume1 < Pi * Radius[k] * Radius[k])
            {
                continue;
            }

            if(((Usage[k] == 0) && (Usage[k + Counter1] == 0)) && (StepVolume + 0.1 >= (Pi * Radius[k] * Radius[k])))
            {
                if((sqrt(pow(X[k], 2) + pow(Y[k], 2)) + Radius[k]) <= (Diametr / 2.0))
                {
                    StepVolume = StepVolume - Pi * Radius[k] * Radius[k];
                    TotalVolume1 = TotalVolume1 - Pi * Radius[k] * Radius[k];
                    Usage[k] = 1;
                    Usage[k + Counter1] = 1;
                    ++Pancakes_Per_Step;
                    ++Ideal;
                    ++Total;
                    --Counter;
                }
                
                if((sqrt(pow(X[k], 2) + pow(Y[k], 2)) + Radius[k]) > (Diametr / 2.0))
                {
                    Radius[k] = NewRadius(Diametr / 2.0, Radius[k], sqrt(pow(X[k], 2) + pow(Y[k], 2)), SingleVolume[k]);
                    StepVolume = StepVolume - Pi * Radius[k] * Radius[k];
                    TotalVolume1 = TotalVolume1 - Pi * Radius[k] * Radius[k];
                    Usage[k] = 1;
                    Usage[k + Counter1] = 1;
                    ++Pancakes_Per_Step;
                    ++Total;
                    --Counter;
                }   // Вычисляем радиус блина, соприкасающегося со стенкой
            }
        }
        
        if(Total > 0)
        {
            ++Step;
        }
        else
        {
            break;
        }
        if((Step > 0) && (Pancakes_Per_Step == 0))
        {
            --Step;
            break;
        }
    }

    // Вывод результатов
    output << argv[1] << endl;
    if((Total == 0) && (Ideal == 0))
    {
        output << "Испечено 0 блинов" << endl;
    }
    else
    {
        output << "Кол-во идеальных блинов: " << Ideal << endl;
        Ideal = (Ideal / Total) * 100.0;
        output << "Т.е. " << Ideal << " процентов от всех (" << Total << ") блинов" << endl;
        output << "Кол-во подходов: " << Step;
    }
    
    // Удаляем динамические массивы из памяти
    delete []SingleVolume;
    delete []X;
    delete []Y;
    delete []Radius;
    delete []Usage;
    
    // Закрываем использованные файлы
    input.close();
    output.close();
    
    // Завершаем выполнение программы
    return 0;
}
