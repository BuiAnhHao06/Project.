#include "main.h"
#include "inputArray/inputArray.h"
#include "TimSort/TimSort.h"
#include "BlockSort/BlockSort.h"

int *copyArray(int *a, int n)
{
    int *tmp = (int *)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
    {
        tmp[i] = a[i];
    }
    return tmp;
}

int main()
{
    int cnt[]{10000, 100000, 1000000};
    int choose;
    do
    {
        cout << "\n\n1. Input random data."
                "\n2.Input sorted data."
                "\n3.Input reverse sorted data."
                "\n4.Exit."
                "\nSelec 1 - 4:  ";
        cin >> choose;
        if (choose == 4)
            return 0;
        for (int i = 0; i < 3; i++)
        {
            int *a;
            inputArray(a, cnt[i], choose);
            cout << cnt[i] << "\n";

            for (int j = 0; j < 2; j++)
            {
                int *tmp = copyArray(a, cnt[i]);
                auto start = high_resolution_clock::now();
                if (j == 0)
                    TimSort(tmp, cnt[i]);
                else
                    BlockSort(tmp, cnt[i]);
                auto stop = high_resolution_clock::now();
                auto time = duration_cast<microseconds>(stop - start);
                if (j == 0)
                    cout << "TimSort: " << time.count() / 1000000.0 << " time\n";
                else
                    cout << "BlockSort: " << time.count() / 1000000.0 << "time\n";
                free(tmp);
            }
            free(a);
        }
    } while (choose != 4);
}