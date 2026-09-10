#include <iostream>
#include <math.h>
#include <random>
#include <time.h>
#include <windows.h>

using namespace std;

double Foo(double x) // Начальная функция
{
    double exp_term = exp(-2.77257 * x * x);
    double part1 = 0.05 * pow(x - 1, 2);
    double part2 = (3 - 2.9 * exp_term) * (1 - cos(x * (4 - 50 * exp_term)));
    return part1 + part2;
}

// Функция генерации нормально распределенного числа (ЦПТ)
/*` NormalRandom_CPT(double mu, double sigma)
{
    double sum = 0;
    for (int i = 0; i < 12; i++)
    {
        sum += (double)rand() / RAND_MAX;
    }
    double z = sum - 6.0;
    return mu + sigma * z;
}
*/
int main()
{
    setlocale(LC_ALL, "");
    srand((unsigned)time(NULL));

    double D = 0.01;
    double sigma = sqrt(D);
    double F = 0.95445161895;
    double xmin = -1;
    double xmax = 1;

    double xc = (double)rand() / RAND_MAX * (xmax - xmin) + xmin;
    double fxc = Foo(xc);

    // Переменные для запоминания лучшего результата за всё время
    double best_xc = xc;
    double best_fxc = fxc;

    cout << "Начальная точка xc: " << xc << ", f(xc) = " << fxc << endl;
    cout << "Целевая точка F: " << F << ", f(F) = " << Foo(F) << endl;

    double T = 1000.0;       // Начальная "температура" (должна быть сопоставима с диапазоном значений функции)
    double alpha = 0.95;  // Коэффициент охлаждения (чем ближе к 1, тем медленнее остывает)
    double T_min = 0.0000001; // Минимальная температура, при которой алгоритм остановится
    double xn;

    // 1. Генерируем новую точку
    random_device rd;
    mt19937 gen(rd());


    normal_distribution<double> distribution(0.0, 0.3);

    int maxIterations = 2000;



    for (int iter = 1; iter <= maxIterations; iter++)
    {

        xn = xc + distribution(gen);

        if (xn < -1)
        {
            xn = -1;
        }
        else if (xn > 1)
        {
            xn = 1;
        }


        double fxn = Foo(xn);
        double delta = fxn - fxc; // Разница между новым и текущим значением

        // 2. Логика принятия решения (Ядро имитации отжига)
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
            double r = (double)rand() / double(RAND_MAX); // Случайное число от 0 до 1

            if (r < probability)
            {
                accept = true;
            }
        }

        // 3. Если решили принять новую точку
        if (accept)
        {
            xc = xn;
            fxc = fxn;

            // Обновляем абсолютный рекорд, если текущая точка стала лучше всех предыдущих
            if (fxc < best_fxc)
            {
                best_xc = xc;
                best_fxc = fxc;
            }
        }

        // 4. Охлаждение системы
        T = T * alpha;

        // Вывод прогресса каждые 10 итераций
        if (iter % 2 == 0)
        {
            cout << "Итерация " << iter
                 << "  Температура: " << T
                 << "  Текущая f(xc): " << fxc
                 << "  ЛУЧШАЯ f(x): " << best_fxc
                 << " xc = " << best_xc << endl;
        }

        // Если температура упала ниже минимума, дальнейший поиск бессмысленен
        if (T < T_min)
        {
            cout << "\n[Досрочная остановка: достигнута минимальная температура]" << endl;
            break;
        }
    }

    cout << "Результат:" << endl;
    cout << "Лучшая найденная точка: xc = " << best_xc << endl;
    cout << "Значение функции в ней: f(xc) = " << best_fxc << endl;
    cout << "Для сравнения, f(F) = " << Foo(F) << endl;

    return 0;
}
