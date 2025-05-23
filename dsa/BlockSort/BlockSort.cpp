#include "../main.h"
#include "BlockSort.h"

// Insertion Sort dung de sap xep cac phan tu trong block
void InsertionSort(int *a, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        int key = a[i];
        int j = i - 1;
        while (j >= left && a[j] > key)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

// Merge 2 block lien tiep [left..mid] va [mid+1..right]
void merge(int *a, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = a[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = a[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
        a[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1)
        a[k++] = L[i++];
    while (j < n2)
        a[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Block Sort chinh
void BlockSort(int *a, int n)
{
    int blockSize = sqrt(n); // Chia mang thanh block kich thuoc gan bang \sqrt{n}

    // B1: Sap xep lai cac Block bang Intersection Sort
    for (int i = 0; i < n; i += blockSize)
    {
        int right = min(i + blockSize - 1, n - 1);
        InsertionSort(a, i, right);
    }

    // B2: Merge dan lai cac block
    for (int size = blockSize; size < n; size *= 2)
    {
        for (int left = 0; left < n; left += 2 * size)
        {
            int mid = min(left + size - 1, n - 1);
            int right = min(left + 2 * size - 1, n - 1);
            if (mid < right)
                merge(a, left, mid, right);
        }
    }
}