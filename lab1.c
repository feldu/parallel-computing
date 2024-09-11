#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <inttypes.h>

// here we assume that unsigned is 32bit integer
unsigned rand_r(unsigned *seed)
{
  (*seed) ^= (*seed) >> 11;
  (*seed) ^= (*seed) << 7 & 0x9D2C5680;
  (*seed) ^= (*seed) << 15 & 0xEFC60000;
  (*seed) ^= (*seed) >> 18;
  return (*seed);
}

// Функция для обмена значений
void swap(double *xp, double *yp)
{
    double temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Функция сортировки выбором
void selectionSort(double arr[], int n)
{
    int i, j, min_idx;

    // Проходим по всему массиву
    for (i = 0; i < n - 1; i++)
    {
        // Находим минимальный элемент в оставшейся части массива
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Меняем минимальный элемент с первым элементом неотсортированной части
        if (min_idx != i)
            swap(&arr[min_idx], &arr[i]);
    }
}

int main(int argc, char *argv[])
{
    int N;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]);
    gettimeofday(&T1, NULL);
    int N2 = N / 2;
    int A = 416.0;
    double X = 0;
    for (int i = 0; i < 100; ++i)
    {
        unsigned seed = i;
        double* M1 = (double *)malloc(sizeof(double) * N);
        double* M2 = (double *)malloc(sizeof(double) * N2);
        double *restrict M2_old = (double *)malloc(N2 * sizeof(double));
        /* GENERATE */
        /* Заполнить массивы исходных данных */
        for (int j = 0; j < N; ++j) {
            
            M1[j] = rand_r(&seed)%A + 1;
            if (j < N2) {
                M2[j] = (rand_r(&seed)%A*9) + A;
            }
        }
        /* MAP*/
        // Кубический корень (cbrt) после деления на e
        for (int j = 0; j < N; ++j)
        {
            M1[j] = cbrt(M1[j] / M_E);
        }
        // Копирование массива M2
        for (int j = 0; j < N2; ++j)
        {
            M2_old[j] = M2[j];
        }
        // В М2 каждый элемент поочерёдно сложить с предыдущим
        for (int k = 1; k < N2; ++k)
        {
            M2[k] = M2[k] + M2_old[k - 1];
        }
        // Модуль синуса суммы текущего и предыдущего значений
        M2[0] = fabs(sin(M2[0]));
        for (int k = 1; k < N2; ++k)
        {
            M2[k] = fabs(sin(M2[k] + M2_old[k - 1]));
        }

        /* MERGE */
        // Выбираем меньшее из M1[k] и M2[k]
        for (int k = 0; k < N2; ++k)
        {
            M2[k] = fmin(M1[k], M2[k]);
        }

        /* SORT */
        selectionSort(M2, N2);
        /* REDUCE */
        /*
         Рассчитать сумму синусов тех элементов массива М2,
         которые при делении на минимальный ненулевой элемент массива М2
         дают чётное число
        */
        for (int j = 0; j < N2; j++) {
            if (M2[0]!= 0 && (int)(M2[j] / M2[0]) % 2 == 0) {
                X += sin(M2[j]);
            }
        }
        free(M1);
        free(M2);
        free(M2_old);
    }
    //printf("X= %f\n", X);
    gettimeofday(&T2, NULL); /* запомнить текущее время T2 */
    delta_ms = (T2.tv_sec - T1.tv_sec) * 1000 +
               (T2.tv_usec - T1.tv_usec) / 1000;
    // printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms); /* T2 - T1 */
    printf("%ld\n", delta_ms);
    return 0;
}