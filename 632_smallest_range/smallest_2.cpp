// 你有 k 个升序排列的整数数组。找到一个最小区间，使得 k 个列表中的每个列表至少有一个数包含在其中。
#include <malloc.h>

/**
* Note: The returned array must be malloced, assume caller calls free().
*/

typedef struct {
    int value;
    int id;
} Data;

// 拷贝
Data* Copy(int *num, int n, int id)
{
    int i;
    Data *res;

    res = (Data*)malloc(sizeof(Data)*n);
    if (res == NULL) {
        return NULL;
    }

    // remain
    i = 0;
    while (i < n) {
        res[i].value = num[i];
        res[i].id = id;
        i++;
    }

    return res;
}

// 合并
Data* SubMerge(Data *num1, Data *num2, int n1, int n2)
{
    int i, j, k, n;
    Data *res, d1, d2;
    
    n = n1 + n2;
    res = (Data*)malloc(sizeof(Data)*n);
    if (res == NULL) {
        return NULL;
    }

    k = 0;
    i = 0;
    j = 0;

    // insert
    while ((i < n1) && (j < n2)) {
        d1 = num1[i];
        d2 = num2[j];

        if (d1.value < d2.value) {
            res[k] = d1;
            i++;
        } else {
            res[k] = d2;
            j++;
        }

        k++;
    }

    // remain
    while (i < n1) {
        res[k++] = num1[i++];
    }
    while (j < n2) {
        res[k++] = num2[j++];
    }

    free(num1);
    free(num2);
    return res;
}

#define MAX_K   3501
Data *g_d[MAX_K];
int g_n[MAX_K];

// 合并
void Merge(int** nums, int numsSize, int* numsColSize)
{
    int i, n, n1, n2;
    Data *p1, *p2;

    // init
    for (i = 0; i < numsSize; i++) {
        g_d[i] = Copy(nums[i], numsColSize[i], i);
        g_n[i] = numsColSize[i];
    }

    // merge
    n = numsSize;
    while (n > 1) {
        for (i = 0; i < n; i += 2) {
            n1 = g_n[i], n2 = g_n[i + 1];
            p1 = g_d[i], p2 = g_d[i + 1];
            g_d[i] = NULL, g_d[i + 1] = NULL;

            g_d[i / 2] = SubMerge(p1, p2, n1, n2);
            g_n[i / 2] = n1 + n2;
        }

        if (n & 0x1) {
            g_d[i / 2] = g_d[n - 1];
            g_n[i / 2] = g_n[n - 1];
            g_d[n - 1] = NULL;
            n++;
        }
        
        n /= 2;
    }
}

int g_map[MAX_K];

// 窗
int IsValid(Data *d, int start, int end, int K)
{
    int i, id;

    // init
    for (i = 0; i < K; i++) {
        g_map[i] = 0;
    }

    // set
    for (i = start; i <= end; i++) {
        id = d[i].id;
        if (g_map[id] == 0) {
            g_map[id] = 1;
        }
    }

    // judge
    for (i = 0; i < K; i++) {
        if (g_map[i] == 0) {
            return 0;
        }
    }

    return 1;
}

typedef struct {
    int left;
    int right;
} Res;

// 找最小窗
Res Small(Data *d, int n, int K)
{
    int i, start, l, r, dr, ds, flag;
    Res small;

    i = 0;
    start = 0;
    small.left = 0;
    small.right = d[n - 1].value;
    ds = small.right - small.left;

    while (i < n) {
        // 调整窗到最小
        flag = 0;
        while (IsValid(d, start, i, K)) {
            start++;
            flag = 1;
        }
        if (flag == 1) {
            // 计算最小窗
            l = d[start - 1].value;
            r = d[i].value;
            dr = r - l;
            if (dr < ds) {
                ds = dr;
                small.left = l;
                small.right = r;
            }
        }

        i++;
    }

    return small;
}

int* smallestRange(int** nums, int numsSize, int* numsColSize, int* returnSize) 
{
    Res small;
    int *res;

    *returnSize = 0;
    res = (int*)malloc(sizeof(int) * 2);
    if (res == NULL) {
        return NULL;
    }

    if (numsSize > 1) {
        Merge(nums, numsSize, numsColSize);
        small = Small(g_d[0], g_n[0], numsSize);
        res[0] = small.left;
        res[1] = small.right;
        *returnSize = 2;
    } else if (numsSize == 1) {
        res[0] = nums[0][0];
        res[1] = nums[0][0];
        *returnSize = 2;
    }

    return res;
}

// test
#define UT_TEST

// test
#ifdef UT_TEST
#include <stdio.h>
#include <assert.h>

void Test01(void)
{
    int a[] = { 4,10,15,24,26 };

    int *nums[] = { a };
    int size[] = { 5 };
    int K = sizeof(size) / sizeof(int);

    int *ret;
    int retSize;
    ret = smallestRange(nums, K, size, &retSize);
    assert(ret[0] == 4);
    assert(ret[1] == 4);
    assert(retSize == 2);
    free(ret);

    printf("%s OK!\n", __func__);
}

void Test02(void)
{
    int a[] = { 4,10,15,24,26 };
    int b[] = { 0,9,12,20 };

    int *nums[] = { a, b };
    int size[] = { 5, 4 };
    int K = sizeof(size) / sizeof(int);

    int *ret;
    int retSize;
    ret = smallestRange(nums, K, size, &retSize);
    assert(ret[0] == 9);
    assert(ret[1] == 10);
    assert(retSize == 2);
    free(ret);

    printf("%s OK!\n", __func__);
}

void Test03(void)
{
    int a[] = { 4,10,15,24,26 };
    int b[] = { 0,9,12,20 };
    int c[] = { 5,18,22,30 };

    int *nums[] = { a, b, c };
    int size[] = { 5, 4, 4 };
    int K = sizeof(size) / sizeof(int);

    int *ret;
    int retSize;
    ret = smallestRange(nums, K, size, &retSize);
    assert(ret[0] == 20);
    assert(ret[1] == 24);
    assert(retSize == 2);
    free(ret);

    printf("%s OK!\n", __func__);
}

void Test04(void)
{
    int a[] = { 4,10,15,21,24,26 };
    int b[] = { 0,9,12,20 };
    int c[] = { 5,18,21,22,30 };

    int *nums[] = { a, b, c };
    int size[] = { 6, 4, 5 };
    int K = sizeof(size) / sizeof(int);

    int *ret;
    int retSize;
    ret = smallestRange(nums, K, size, &retSize);
    assert(ret[0] == 20);
    assert(ret[1] == 21);
    assert(retSize == 2);
    free(ret);

    printf("%s OK!\n", __func__);
}

void Test05(void)
{
    int a[] = { 10 };
    int b[] = { 11 };

    int *nums[] = { a, b };
    int size[] = { 1, 1 };
    int K = sizeof(size) / sizeof(int);

    int *ret;
    int retSize;
    ret = smallestRange(nums, K, size, &retSize);
    assert(ret[0] == 10);
    assert(ret[1] == 11);
    assert(retSize == 2);
    free(ret);

    printf("%s OK!\n", __func__);
}

void Test06(void)
{
    int a[] = { 1,3,5,7,9,10 };
    int b[] = { 2,4,6,8,10 };

    int *nums[] = { a, b };
    int size[] = { 6, 5 };
    int K = sizeof(size) / sizeof(int);

    int *ret;
    int retSize;
    ret = smallestRange(nums, K, size, &retSize);
    assert(ret[0] == 10);
    assert(ret[1] == 10);
    assert(retSize == 2);
    free(ret);

    printf("%s OK!\n", __func__);
}

void Test07(void)
{
    int a[] = { 1,4,7,10 };
    int b[] = { 2,5,8,11 };
    int c[] = { 3,6,9,12 };

    int *nums[] = { a, b, c };
    int size[] = { 4, 4, 4 };
    int K = sizeof(size) / sizeof(int);

    int *ret;
    int retSize;
    ret = smallestRange(nums, K, size, &retSize);
    assert(ret[0] == 1);
    assert(ret[1] == 3);
    assert(retSize == 2);
    free(ret);

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