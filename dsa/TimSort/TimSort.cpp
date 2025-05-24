#include "../main.h"
#include "TimSort.h"

const int MIN_MERGE = 32;
int min_gallop = 7;     // ngưỡng để bật chế độ galloping

// Tính độ dài tối thiểu của 1 runs
int calcMinRun(int n)
{
    int r = 0;
    while (n >= MIN_MERGE)
    {
        r |= (n & 1);   // nếu n lẻ, đạt r = 1;
        n >>= 1;        // chia đôi n
    }
    return n + r;       // trả về minRun
}

// Sắp xếp mảng từ left đến right bằng Insertion sort 
void insertionSort(int *arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int val = arr[i], j = i - 1;
        while (j >= left && arr[j] > val)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = val;
    }
}

// Tìm vị trí đầu tiên mà base[pos] >= x (cho phần merge bên trái)
int gallopLeft(int x, int *base, int len)
{
    int ofs = 1, lastOfs = 0;

    if (x > base[0])
    {
        // Nhảy lũy thùa 2 để tìm khoảng cần tìm kiếm nhị phân
        while (ofs < len && x > base[ofs])
        {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
        }
        if (ofs > len)
            ofs = len;
        lastOfs++;
    }
    else
    {
        return 0;
    }
    // Tìm nhị phân trong khoảng từ lastOfs đến ofs
    int low = lastOfs, high = ofs;
    while (low < high)
    {
        int mid = low + (high - low) / 2;
        if (x > base[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}
// Tìm vị trí đầu tiên mà base[pos] > x (cho phần merge bên phải)
int gallopRight(int x, int *base, int len)
{
    int ofs = 1, lastOfs = 0;

    if (x < base[0])
    {
        return 0;
    }
    else
    {
        while (ofs < len && x >= base[ofs])
        {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
        }
        if (ofs > len)
            ofs = len;
        lastOfs++;
    }

    int low = lastOfs, high = ofs;
    while (low < high)
    {
        int mid = low + (high - low) / 2;
        if (x >= base[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

// Hợp nhất 2 Run đã sắp xếp
void mergeAt(int *arr, Run a, Run b)
{
    int *temp = new int[a.length];      // tạo bản sao cho Run a
    memcpy(temp, arr + a.start, a.length * sizeof(int));

    int i = 0, j = b.start, dest = a.start;
    int aLen = a.length, bLen = b.length;

    int countA = 0, countB = 0;     // Đếm số lần bên A hoặc B thắng liên tiếp

    while (i < aLen && j < b.start + bLen)
    {
        if (temp[i] <= arr[j])
        {
            arr[dest++] = temp[i++];
            countA++;
            countB = 0;
        }
        else
        {
            arr[dest++] = arr[j++];
            countB++;
            countA = 0;
        }
        // Nếu một phía thắng liên tiếp nhiều lần, dùng chế độ galloping
        if ((countA | countB) >= min_gallop)
        {
            int gallopA = gallopRight(arr[j], temp + i, aLen - i);
            memcpy(arr + dest, temp + i, gallopA * sizeof(int));
            dest += gallopA;
            i += gallopA;

            if (i >= aLen)
                break;

            int gallopB = gallopLeft(temp[i], arr + j, b.start + bLen - j);
            memcpy(arr + dest, arr + j, gallopB * sizeof(int));
            dest += gallopB;
            j += gallopB;

            if (j >= b.start + bLen)
                break;

            min_gallop += 1;    // Điều chỉnh ngưỡng tùy thuật toán
        }
    }
    // Copy phần còn lại của temp 
    while (i < aLen)
        arr[dest++] = temp[i++];

    delete[] temp;
}

// Hợp nhất các runs trên Stack
void mergeCollapse(stack<Run> &runs, int *arr)
{
    while (runs.size() > 1)
    {
        Run X = runs.top();
        runs.pop();
        Run Y = runs.top();
        runs.pop();

        Run Z;
        if (!runs.empty())
        {
            Z = runs.top();
            runs.pop();
            if (Z.length <= Y.length + X.length)
            {
                // Nếu Z nhỏ nhất thì gộp Z và Y trước
                if (Z.length < X.length)
                {
                    runs.push(Y);
                    mergeAt(arr, Z, Y);
                    Z.length += Y.length;
                    runs.push(Z);
                }
                else
                {
                    mergeAt(arr, Y, X);
                    Y.length += X.length;
                    runs.push(Z);
                    runs.push(Y);
                }
                continue;
            }
            else
            {
                runs.push(Z);
            }
        }
        // Gộp X và Y nếu cần thiết
        if (Y.length <= X.length)
        {
            mergeAt(arr, Y, X);
            Y.length += X.length;
            runs.push(Y);
        }
        else
        {
            runs.push(Y);
            runs.push(X);
            break;
        }
    }
}

void TimSort(int *arr, int n)
{
    int minRun = calcMinRun(n);
    stack<Run> runStack;

    int i = 0;
    while (i < n)
    {
        int runStart = i;
        int runEnd = i + 1;

        // Phát hiện run giảm và đảo ngược thành tăng
        if (runEnd < n && arr[runEnd] < arr[runStart])
        {
            while (runEnd < n && arr[runEnd] < arr[runEnd - 1])
                runEnd++;
            reverse(arr + runStart, arr + runEnd);
        }
        else
        {
            // Run tăng tự nhiên
            while (runEnd < n && arr[runEnd] >= arr[runEnd - 1])
                runEnd++;
        }

        int runLen = runEnd - runStart;
        // Nếu run ngắn hơn minRun, dùng Insertion sort mở rộng
        if (runLen < minRun)
        {
            int end = min(runStart + minRun, n);
            insertionSort(arr, runStart, end - 1);
            runEnd = end;
            runLen = runEnd - runStart;
        }
        // Lưu run vào Stack
        runStack.push({runStart, runLen});
        mergeCollapse(runStack, arr);   // Gộp nếu cần

        i = runEnd;     // Tiếp tục với đoạn kế tiếp
    }
    // Gộp các run còn lại
    while (runStack.size() > 1)
    {
        Run X = runStack.top();
        runStack.pop();
        Run Y = runStack.top();
        runStack.pop();
        mergeAt(arr, Y, X);
        Y.length += X.length;
        runStack.push(Y);
    }
}



