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

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

//---------------------------------------------------------------------------------------------

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
    float *SingleVolume = new float[100]; // Объем поварешки
    float *X = new float[100];            // Координаты
    float *Y = new float[100];            // центра блина
    float Ideal = 0;                      // Кол-во идеальных блинов
    float StepVolume = 0;                 // Свободное место на сковороде
                                          // в этом подходе
    int Step = 0;                         // Кол-во подходов
    int Total = 0;                        // Всего блинов
    int Counter = 0;                      // Счетчик
    int Counter1 = 0;                     // Вспомогательный счетчик
    float  Pi = 3.14159265358979;         // Число π
    
    // Считываем переменные из файла
    input >> Diametr;
    input >> Height;
    input >> TotalVolume;
    int i = TotalVolume;
    while((i >= 0) || (!input.eof()))
    {
        input >> SingleVolume[Counter];
        if(SingleVolume[Counter] <= 0)
        {
            output << "Введены неверные данные!" << endl;
            cout << "Введены неверные данные!" << endl;
            exit(3);
        }
        input >> X[Counter] >> Y[Counter];
        i = i - SingleVolume[Counter];
        ++Counter;
    }
    
    // Дополнительной объявление переменных и присвоение значений
    Counter1 = Counter;
    float *Radius = new float[Counter];   // Массив со значениями радиуса каждого нового блина
    
    // Основной алгоритм
    while(Counter != 0)
    {
        StepVolume = TotalVolume / Height;
        for(int k = 0; k < Counter1; ++k)
        {
            SingleVolume[k] = SingleVolume[k] / Height; // Находим площадь каждого блина
            Radius[k] = sqrt(SingleVolume[k] / Pi);     // Находим радиус этого круга (при условии, что
                                                        // он не пересекается стенками сковороды)
            if((X[k] * X[k] + Y[k] * Y[k]) >= (Diametr * Diametr / 4.0))
            {
                Radius[k] = Diametr;
            }
            
            bool Opportunity = true; // Возможность выпечь блин (true - нет на пути других блинов)
            if(Radius[k] >= Diametr / 2.0)
            {
                Opportunity = false;
                // Radius[k] = Diametr;
            }
            for(int i = 0; i < k; ++i)
            {
                if(Opportunity)
                {
                     if(((pow(X[i] - X[k], 2) + pow(Y[i] - Y[k], 2)) >= pow(Radius[i] + Radius[k], 2)) || ((((pow(X[i] - X[k], 2) + pow(Y[i] - Y[k], 2)) <= pow(Radius[i], 2)) || ((pow(X[i] - X[k], 2) + pow(Y[i] - Y[k], 2)) <= pow(Radius[k], 2))) && ((X[i] == X[k]) && (Y[i] == Y[k]))))
                    {
                        Opportunity = false; // Если блин попадает на уже жарящийся блин
                    }
                }
                else
                {
                    break;
                }
            }
            
            if(Opportunity && (StepVolume + 0.00001 >= (Pi * Radius[k] * Radius[k])))
            {
                if((sqrt(pow(X[k], 2) + pow(Y[k], 2)) + Radius[k]) <= (Diametr / 2.0))
                {
                    if(Radius[k] == Diametr)
                    {
                        continue;
                    }
                    StepVolume = StepVolume - Pi * Radius[k] * Radius[k];
                    Radius[k] = Diametr;
                    ++Ideal;
                    ++Total;
                    --Counter;
                }
                
                if((sqrt(pow(X[k], 2) + pow(Y[k], 2)) + Radius[k]) > (Diametr / 2.0))
                {
                    if(Radius[k] == Diametr)
                    {
                        continue;
                    }
                    Radius[k] = NewRadius(Diametr / 2.0, Radius[k], sqrt(pow(X[k], 2) + pow(Y[k], 2)), SingleVolume[k]);
                    StepVolume = StepVolume - Pi * Radius[k] * Radius[k];
                    ++Total;
                    --Counter;
                }   // Вычисляем радиус блина, соприкасающегося со стенкой
            }
            else
            {
                continue;
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
    
    // Закрываем использованные файлы
    input.close();
    output.close();
    
    // Завершаем выполнение программы
    return 0;
}
