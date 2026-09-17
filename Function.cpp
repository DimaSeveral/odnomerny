#include <iostream>
#include <math.h>
#include <random>
#include <time.h>

using namespace std;

double Foo(const double* x)
{
    double x1 = x[0];
    double x2 = x[1];
    double part1 = -1 + pow((sin(sqrt(x1*x1 + x2*x2))), 2);
    double part2 = 1 + 0.001 * (x1*x1 + x2*x2);
    double result = part1 / part2;
    return result;
}

int main()
{
    setlocale(LC_ALL, "");

    double minXY = -10.0;
    double maxXY = 10.0;
    int n = 2;

    double T_start = 1000.0;
    double alpha = 0.998;
    double T_min = 0.0000001;
    int maxIterations = 4000;
    int numRuns = 100; // Количество запусков внешнего цикла

    // Генератор случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> uniform_rl(minXY, maxXY);
    normal_distribution<double> distribution(0.0, 1.0);
    uniform_real_distribution<double> prob_dist(0.0, 0.8);

    //  массив для хранения истории лучших значений
    double* history_BestValues = new double[numRuns];

    // внешний цикл (10 итераций)
    for (int run = 1; run <= numRuns; ++run)
    {
        double T = T_start; // Сброс температуры для каждого запуска

        double* curr = new double[n];
        double* next = new double[n];
        double* best = new double[n];

        // Начальная точка
        for (int i = 0; i < n; ++i)
        {
            curr[i] = uniform_rl(gen);
            best[i] = curr[i];
        }

        double Value_Curr = Foo(curr);
        double Value_Best = Value_Curr;

        // Внутренний цикл имитации отжига
        for (int iter = 1; iter <= maxIterations; iter++)
        {
            for (int i = 0; i < n; ++i)
            {
                next[i] = curr[i] + distribution(gen);
                if (next[i] < minXY) next[i] = minXY;
                else if (next[i] > maxXY) next[i] = maxXY;
            }

            double Next_Value = Foo(next);
            double delta = Next_Value - Value_Curr;

            bool accept = false;
            if (delta < 0)
            {
                accept = true;
            }
            else
            {
                double probability = exp(-delta / T);
                if (prob_dist(gen) < probability)
                {
                    accept = true;
                }
            }

            if (accept)
            {
                for (int i = 0; i < n; ++i) curr[i] = next[i];
                Value_Curr = Next_Value;

                // Исправленная логика обновления абсолютного рекорда
                if (Value_Curr < Value_Best)
                {
                    for (int i = 0; i < n; ++i) best[i] = curr[i];
                    Value_Best = Value_Curr;
                }
            }

            T = T * alpha;

            //if (T < T_min) break;
        }

        cout << "Лучшее значение: f = " << Value_Best
             <<  endl;

        // Индекс run - 1, так как нумерация массивов начинается с 0
        history_BestValues[run - 1] = Value_Best;

        // Освобождаем память внутреннего цикла
        delete[] curr;
        delete[] next;
        delete[] best;
    };

    double summa = 0.0;

    cout << "История лучших значений по запускам:" << endl;
    for (int i = 0; i < numRuns; ++i)
    {
        summa += history_BestValues[i]; // Накапливаем сумму
    }

    double average = summa / numRuns;

    cout << "\nСумма всех лучших значений (summa) : " << summa << endl;
    cout << "Среднее значение (average)         : " << average << endl;

    delete[] history_BestValues;

    return 0;
}
