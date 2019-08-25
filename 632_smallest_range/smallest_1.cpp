// 你有 k 个升序排列的整数数组。找到一个最小区间，使得 k 个列表中的每个列表至少有一个数包含在其中。
#include <malloc.h>

/**
* Note: The returned array must be malloced, assume caller calls free().
*/

#define MAX_K 3501
int g_id[MAX_K];

// init
void Init(int k)
{
    int i;

    for (i = 0; i < k; i++) {
        g_id[i] = 0;
    }
}

typedef struct {
    int value;
    int id;
} Res;

// min
Res Min(int** nums, int numsSize, int* numsColSize)
{
    Res r;
    int i, id, val, min, mid;

    min = 100000;
    for (i = 0; i < numsSize; i++) {
        if (g_id[i] <= numsColSize[i] - 1) {
            mid = i;
        }
    }

    for (i = 0; i < numsSize; i++) {
        id = g_id[i];
        val = nums[i][id];
        if (val < min) {
            min = val;
            if (id < numsColSize[i] - 1) {
                mid = i;
            }
        }
    }

    r.id = mid;
    r.value = min;
    return r;
}

// max
Res Max(int** nums, int numsSize, int* numsColSize)
{
    Res r;
    int i, id, val, max;

    max = -100000;
    for (i = 0; i < numsSize; i++) {
        id = g_id[i];
        val = nums[i][id];
        if (val > max) {
            max = val;
            if (id < numsColSize[i] - 1) {
            }
        }
    }

    r.id = 0;
    r.value = max;
    return r;
}

typedef struct {
    int left;
    int right;
} Range;

// flag
int Flag(int numsSize, int* numsColSize)
{
    int i, size;

    for (i = 0; i < numsSize; i++) {
        size = numsColSize[i];
        if (g_id[i] < size - 1) {
            return 1;
        }
    }

    return 0;
}

// cmp
int Compare(Range *r1, Range *r2)
{
    int d1 = r1->right - r1->left;
    int d2 = r2->right - r2->left;

    if (d1 > d2) {
        return 1;
    } else if (d1 < d2) {
        return -1;
    } else {
        if (r1->left > r2->left) {
            return 1;
        } else if (r1->left < r2->left) {
            return -1;
        } else {
            return 0;
        }
    }
}

#define MIN(a, b)   ((a) < (b) ? (a) : (b))

// small
Range Small(int** nums, int numsSize, int* numsColSize)
{
    int flag, id;
    Res min, max;
    Range range, small;
    
    small.left = -100000;
    small.right = +100000;

    flag = 1;
    do {
        flag = Flag(numsSize, numsColSize);
        min = Min(nums, numsSize, numsColSize);
        max = Max(nums, numsSize, numsColSize);

        range.left = min.value;
        range.right = max.value;
        if (Compare(&range, &small) < 0) {
            small = range;
        }

        id = g_id[min.id];
        g_id[min.id] = MIN(id + 1, numsColSize[min.id] - 1);
    } while (flag == 1);

    return small;
}

int* smallestRange(int** nums, int numsSize, int* numsColSize, int* returnSize) 
{
    int *res;
    Range range;
    
    Init(numsSize);
    res = (int*)malloc(sizeof(range));

    if (numsSize >= 2) {
        range = Small(nums, numsSize, numsColSize);
        *returnSize = 2;
        res[0] = range.left;
        res[1] = range.right;
    } else if(numsSize == 1) {
        *returnSize = 2;
        res[0] = nums[0][0];
        res[1] = nums[0][0];
    } else {
        *returnSize = 0;
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

int main(void)
{
    Test01();
    Test02();
    Test03();
    Test04();
    Test05();
    Test06();

    return 0;
}
#endif