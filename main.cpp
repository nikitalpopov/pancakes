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

void Hello()
{
    cout
    << "--------------------------------------------"
    << endl
    << "--                                        --"
    << endl
    << "--                 HELLO!                 --"
    << endl
    << "--                                        --"
    << endl
    << "--------------------------------------------"
    << endl << endl;
}

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
    Hello();
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
    
    float Diametr = 0;                    // Диаметр сковороды
    float Height = 0;                     // Толщина блина
    float TotalVolume = 0;                // Объем кастрюли
    float *SingleVolume = new float[100]; // Объем поварешки
    float *X = new float[100];            // Координаты
    float *Y = new float[100];            // центра блина
    float Ideal = 0;                        // Кол-во идеальных блинов
    int StepVolume = 0;                   // Свободное место на сковороде
                                          // в этом подходе
    int Step = 0;                         // Кол-во подходов
    int Counter = 0;                      // Счетчик
    int Total = 0;                        // Всего блинов
    
    input >> Diametr;
    cout << Diametr << endl;
    input >> Height;
    cout << Height << endl;
    input >> TotalVolume;
    cout << TotalVolume << endl;
    int i = TotalVolume;
    while(i >= 0)
    {
        input >> SingleVolume[Counter];
        if(SingleVolume[Counter] <= 0)
        {
            cout << "Введены неверные данные!" << endl;
            exit(3);
        }
        cout << SingleVolume[Counter] << endl;
        input >> X[Counter] >> Y[Counter];
        cout << X[Counter] << endl << Y[Counter] << endl;
        i = i - SingleVolume[Counter];
        ++Counter;
    }
    
    int k = Counter - 1;
    float *Radius = new float[Counter];
    float  Pi = 3.14159265358979;
    while(Counter != 0)
    {
        StepVolume = TotalVolume / Height;
        ++Step;
        cout << Step << " ------ \n";
        for(; k > 0; --k)
        {
            SingleVolume[k] = SingleVolume[k] / Height; // Находим площадь каждого блина
            Radius[k] = sqrt(SingleVolume[k] / Pi);     // Находим радиус этого круга (при условии, что
                                                        // он не пересекается стенками сковороды)
            if((X[k] * X[k] + Y[k] * Y[k]) >= (Diametr * Diametr / 4.0))
            {
                Radius[k] = Diametr;
            }
            if(Radius[k] >= Diametr / 2.0)
            {
                cout << k << "ый блин не может быть выпечен\n";
                --Counter;
            }
            
            bool Opportunity = true; // Возможность выпечь блин (true - нет на пути других блинов)
            for(int i = Counter - 1; i > k; --i)
            {
                if(Opportunity)
                {
                    // cout << "// " << sqrt(pow(X[i] - X[k], 2) + pow(Y[i] - Y[k], 2)) << ">=" << Radius[i] + Radius[k] << endl;
                    if((sqrt(pow(X[i] - X[k], 2) + pow(Y[i] - Y[k], 2))) >= Radius[i] + Radius[k])
                    {
                        Opportunity = false; // Если блин попадает на уже жарящийся блин
                    }
                }
                else
                {
                    break;
                }
            }
            
            cout << "Step " << Step << endl;
            cout << "Opportunity " << Opportunity << endl;
            cout << StepVolume << " > " << Pi * Radius[k] * Radius[k] << endl;
            if(Opportunity || (StepVolume > (Pi * Radius[k] * Radius[k])))
            {
                // cout << "/ " << sqrt(pow(X[k], 2) + pow(Y[k], 2)) + Radius[k] << " <= " << (Diametr / 2.0) << endl;
                if((sqrt(pow(X[k], 2) + pow(Y[k], 2)) + Radius[k]) <= (Diametr / 2.0))
                {
                    if(Radius[k] == Diametr)
                    {
                        break;
                    }
                    ++Ideal;
                    cout << Ideal << " ---- \n";
                    ++Total;
                    StepVolume = StepVolume - Pi * Radius[k] * Radius[k];
                    Radius[k] = Diametr;
                    --Counter;
                }
                
                if((sqrt(pow(X[k], 2) + pow(Y[k], 2)) + Radius[k]) > (Diametr / 2.0))
                {
                    if(Radius[k] == Diametr)
                    {
                        break;
                    }
                    ++Total;
                    Radius[k] = NewRadius(Diametr / 2.0, Radius[k], sqrt(pow(X[k], 2) + pow(Y[k], 2)), SingleVolume[k]);
                    StepVolume = StepVolume - Pi * Radius[k] * Radius[k];
                    --Counter;
                }   // Вычисляем радиус блина, соприкасающегося со стенкой
                cout << "Total " << Total << endl;
            }
            else
            {
                break;
            }
        }
    }
    
    output << "Кол-во идеальных блинов: " << Ideal << endl;
    Ideal = (Ideal / Total) * 100.0;
    output << "Т.е. " << Ideal << " процентов от всех блинов" << endl;
    output << "Кол-во подходов: " << Step; // НЕПРАВИЛЬНО СЧИТАЕТ, ОСТАЛЬНОЕ ВЕРНО, ВРОДЕ
    
    return 0;
}
