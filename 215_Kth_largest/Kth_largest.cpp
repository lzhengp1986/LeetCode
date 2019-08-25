// 在未排序的数组中找到第 k 个最大的元素。请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。
#include <stdio.h>
#include <string.h>

#define SWAP(a, b, t)   {(t) = (a); (a) = (b); (b) = (t);}

// 使用快排
void Qsort(int *d, int l, int r, int K)
{
    int i, j;
    int key, tmp;

    if (l >= r) {
        return;
    }

    i = l;
    j = r;
    key = d[l];

    while (i != j) {
        while ((d[j] >= key) && (i < j)) { j--; }
        while ((d[i] <= key) && (i < j)) { i++; }
        if (i < j) {
            SWAP(d[i], d[j], tmp);
        }
    }

    if (i != l) {
        d[l] = d[i];
        d[i] = key;
    }

    if (K < i) {
        Qsort(d, l, i - 1, K);
    } else if (K > i) {
        Qsort(d, j + 1, r, K);
    }
}

// topK
int topKth(int *d, int n, int k)
{
    int res = 0;

    Qsort(d, 0, n - 1, n - k);
    if (k <= n) {
        res = d[n - k];
    }

    return res;
}

int findKthLargest(int* nums, int numsSize, int k) 
{
    return topKth(nums, numsSize, k);
}

// test
#define UT_TEST

#ifdef UT_TEST
#include <stdio.h>
#include <assert.h>

void Test01(void)
{
    int d[] = { 0 };
    int size = sizeof(d) / sizeof(int);
    int k = 1;

    int res = findKthLargest(d, size, k);
    assert(res == 0);
    printf("%s OK!\n", __func__);
}

void Test02(void)
{
    int d[] = { 10 };
    int size = sizeof(d) / sizeof(int);
    int k = 1;

    int res = findKthLargest(d, size, k);
    assert(res == 10);
    printf("%s OK!\n", __func__);
}

void Test03(void)
{
    int d[] = { 0, 10 };
    int size = sizeof(d) / sizeof(int);
    int k = 1;

    int res = findKthLargest(d, size, k);
    assert(res == 10);
    printf("%s OK!\n", __func__);
}

void Test04(void)
{
    int d[] = { 0, 1, 2, 10, 10 };
    int size = sizeof(d) / sizeof(int);
    int k = 1;

    int res = findKthLargest(d, size, k);
    assert(res == 10);
    printf("%s OK!\n", __func__);
}

void Test05(void)
{
    int d[] = { 0, 1, 2, 10, 1, 5, 6, 7 };
    int size = sizeof(d) / sizeof(int);
    int k = 2;

    int res = findKthLargest(d, size, k);
    assert(res == 7);
    printf("%s OK!\n", __func__);
}

void Test06(void)
{
    int d[] = { 3,2,1,5,6,4 };
    int size = sizeof(d) / sizeof(int);
    int k = 2;

    int res = findKthLargest(d, size, k);
    assert(res == 5);
    printf("%s OK!\n", __func__);
}

void Test07(void)
{
    int d[] = { 3,2,3,1,2,4,5,5,6 };
    int size = sizeof(d) / sizeof(int);
    int k = 4;

    int res = findKthLargest(d, size, k);
    assert(res == 4);
    printf("%s OK!\n", __func__);
}

int main(void)
{
    Test01();
    Test02();
    Test03();
    Test04();
    Test05();
    Test06();
    Test07();

    return 0;
}
#endif