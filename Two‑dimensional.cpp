#include <iostream>
#include <math.h>
#include <random>
#include <time.h>
#include <windows.h>

using namespace std;

double Foo(const double*x)
{
    double x1 = x[0];
    double x2 = x[1];
    double part1 = -1 + pow((sin(sqrt(x1*x1 + x2*x2))), 2);
    double part2 = 1 + 0.001 * (x1*x1 + x2*x2);
    double result = part1 / part2;
    return result;
}
/*
double generateRand (double minimum, double maximum)
{
    return minimum + double(rand()) / double(RAND_MAX) * (maximum-minimum);
}*/
int main()
{
    setlocale(LC_ALL, "");
    srand((unsigned)time(NULL));

    double minXY = -10.0;
    double maxXY = 10.0;
    int n = 2; //Количество элементов



    double T = 1000.0;       // Начальная "температура" (должна быть сопоставима с диапазоном значений функции)
    double alpha = 0.95;  // Коэффициент охлаждения (чем ближе к 1, тем медленнее остывает)
    double T_min = 0.0000001; // Минимальная температура, при которой алгоритм остановится
    double xn;

    //Генератор случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> unif(minXY, maxXY);
    normal_distribution<double> distribution(0.0, 0.5);
    uniform_real_distribution<double> prob_dist(0.0, 1.0);

    double*curr = new double[n];
    double*next = new double[n];
    double*best = new double[n];

    int maxIterations = 2000;
//начальная точка
    for (int i = 0; i < n; ++i)
    {
        curr[i] = unif(gen);
        best[i] = curr[i];
    }

    double Value_Curr = Foo(curr);
    double Value_Best = Value_Curr;

    cout << "Начальная точка: " << curr[0] << " ; " << curr[1] << ", f = " << Value_Curr << endl;

   //Цикл имитации отжига
    for (int iter = 1; iter <= maxIterations; iter++)
    {
        //Генерация новой точки
        for (int i = 0; i < n; ++i)
        {
            next[i] = curr[i] + distribution(gen);

            if (next[i] < minXY)
            {
                next[i] = minXY;
            }
            else if (next[i] > maxXY)
            {
                next[i] = maxXY;
            }
        }

        double Next_Value = Foo(next);
        double delta = Next_Value - Value_Curr;





        // Логика принятия решения
        bool accept = false;

        if (delta < 0)
        {
            // Если стало ЛУЧШЕ (функция уменьшилась) - принимаем всегда
            accept = true;
        }
        else
        {
            // Если стало ХУЖЕ - принимаем с вероятностью, зависящей от температуры
            double probability = exp(-delta / T); // Формула отжига
             // Случайное число от 0 до 1

            if (prob_dist(gen) < probability)
            {
                accept = true;//Ухудшение принимает с шансом
            }
        }

        //  Если решили принять новую точку
        if (accept)
        {
           for (int i = 0; i < n; ++i)
           {
               curr[i] = next[i];
           }
            Value_Curr = Next_Value;
           if (Value_Curr < Value_Best) //Обновляем абсолютный рекорд
           {
               for (int i = 0; i < n; ++i)
               {
                    best[i] = curr[i];
               }

           }
           Value_Best = Value_Curr;
        }

        //  Охлаждение системы
        T = T * alpha;

        // Вывод прогресса каждые 10 итераций
        if (iter % 2 == 0)
        {
            cout << "Итерация " << iter
                 << "  Температура: " << T

                 << "  ЛУЧШАЯ f: " << Value_Best
                 << " при [" << best[0] << ", " << best[1] << "]" << endl;
        }

        // Если температура упала ниже минимума, дальнейший поиск бессмысленен
        if (T < T_min)
        {
            cout << "\n[Досрочная остановка: достигнута минимальная температура]" << endl;
            break;
        }
    }

    cout << "Результат:" << endl;
    cout << "Лучшая найденная точка: xc = " << "Лучшая найденная точка: [" << best[0] << ", " << best[1] << endl;
    cout << "Значение функции в ней: f(xc) = " << Value_Best << endl;
    cout << "Для сравнения, f(0.0; 0.0) = " << Foo(new double[2] {0.0, 0.0}) << endl;

    delete[] curr;
    delete[] next;
    delete[] best;

    return 0;
}
