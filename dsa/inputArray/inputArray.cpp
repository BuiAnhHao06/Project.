#include "../main.h"
#include "inputArray.h"

void input1(int *&a, int n)
{
    for (int i = 0; i < n; i++)
        a[i] = rand() % n;
}

void input2(int *&a, int n)
{
    for (int i = 0; i < n; i++)
        a[i] = i;
}

void input3(int *&a, int n)
{
    for (int i = 0; i < n; i++)
        a[i] = n - i;
}

void inputArray(int *&a, int n, int choose)
{
    a = (int *)calloc(n, sizeof(int));
    if (choose == 1)
        input1(a, n);
    else if (choose == 2)
        input2(a, n);
    else
        input3(a, n);
}
