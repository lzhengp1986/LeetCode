#include <stdio.h>
#include <malloc.h>

// 维测
enum {
    MALLOC_ERR,
    ERR_NUM
} Error;

int g_stat[ERR_NUM];

// 方向
typedef enum {
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST,
    NORTH,
    NORTH_EAST,
    DIR_NUM
} Dir;

typedef struct {
    int x;
    int y;
} Point;

#define VALID       1
#define INVALID     0
#define SUCCESS     0
#define FAILURE     1
#define IS_VALID_RANGE(x, N)   (((x) >= 0) && ((x) < (N)))

// 新坐标
int NextPoint(Point* cur, Point* next, int d, int n)
{
    Point tab[DIR_NUM] = { {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1} };
    Point pnt = { cur->x + tab[d].x, cur->y + tab[d].y };

    if (IS_VALID_RANGE(pnt.x, n) && IS_VALID_RANGE(pnt.y, n)) {
        next->x = pnt.x;
        next->y = pnt.y;
        return VALID;
    }

    return INVALID;
}

// 判断坐标是否空
int IsEmpty(int **grid, int n, Point* p)
{
    if (IS_VALID_RANGE(p->x, n) && IS_VALID_RANGE(p->y, n)) {
        return ((grid[p->y][p->x] == 0) ? VALID : INVALID);
    } 

    return INVALID;
}

typedef struct {
    Point pnt;
    short dist;
} Node;

typedef struct {
    int front;
    int tear;
    Node* q;
} Queue;

typedef struct {
    short dist;
    short flag;
} Map;

Map **g_map;
Queue g_queue, *g_que;

#define IS_SAME_POINT(p1, p2) (((p1).x == (p2).x) && ((p1).y == (p2).y))

// 入队列
int Enq(Point* p, short dist, int n)
{
    Node node = { {p->x, p->y}, dist };

    if (g_que->tear < (n * n)) {
        // 是否已在队列中
        for (int i = g_que->front; i < g_que->tear; i++) {
            if (IS_SAME_POINT(g_que->q[i].pnt, *p)) {
                return FAILURE;
            }
        }

        g_que->q[g_que->tear++] = node;
        return SUCCESS;
    }

    return FAILURE;
}

// 出队列
int Deq(Node* node, int n)
{
    if ((g_que->front < g_que->tear) && (g_que->front < (n * n))) {
        *node = g_que->q[g_que->front++];
        return SUCCESS;
    }

    return FAILURE;
}

#define INVALID_DIST -1

// 初始化
void Init(int** grid, int n)
{
    // init queue
    Node* node = (Node*)malloc(sizeof(Node) * (n * n + 1));
    if (node == NULL) {
        g_stat[MALLOC_ERR]++;
    }

    g_queue.front = 0;
    g_queue.tear = 0;
    g_queue.q = node;
    g_que = &g_queue;

    // init flag
    Map* flag = (Map*)malloc(sizeof(Map) * (n * n));
    if (flag == NULL) {
        g_stat[MALLOC_ERR]++;
    }
    Map** addr = (Map**)malloc(sizeof(Map*) * n);
    if (addr == NULL) {
        g_stat[MALLOC_ERR]++;
    }

    g_map = addr;
    for (int i = 0; i < n; i++) {
        Map* row = flag + i * n;
        for (int j = 0; j < n; j++) {
            row[j].dist = INVALID_DIST;
            row[j].flag = INVALID;
        }
        addr[i] = row;
    }
    
    // init q
    Point p = { 0, 0 };
    Enq(&p, 1, n);
}

// 算距离
void CalcDist(int** grid, int n)
{
    Map *map;
    Node node;
    Point *pnt, next;

    int i, d, ret;

    while ((ret = Deq(&node, n)) == SUCCESS) {
        pnt = &(node.pnt);
        d = node.dist;

        if (IsEmpty(grid, n, pnt)) {
            map = &g_map[pnt->y][pnt->x];
            if (map->flag == INVALID) {
                // 更新当前位置的距离
                map->flag = VALID;
                map->dist = d;

                // 把周围未访问的点放进去
                for (i = 0; i < DIR_NUM; i++) {
                    if ((ret = NextPoint(pnt, &next, i, n)) == VALID) {
                        if (g_map[next.y][next.x].flag == INVALID) {
                            Enq(&next, d + 1, n);
                        }
                    }
                }
            }
        }
    }
}

// 1091. 二进制矩阵中的最短路径
int shortestPathBinaryMatrix(int** grid, int gridSize, int* gridColSize)
{
    Init(grid, gridSize);
    CalcDist(grid, gridSize);
    return g_map[gridSize - 1][gridSize - 1].dist;
}

#define TEST

#ifdef TEST
#include <assert.h>

void Test01(void)
{
#define N 1
    int map[N][N] = { 0 };
    int *grid[N] = { map[0] };
    int col[N] = { N };

    int d = shortestPathBinaryMatrix(grid, N, col);
    assert(d == 1);
#undef N
}

void Test02(void)
{
#define N 1
    int map[N][N] = { 1 };
    int *grid[N] = { map[0] };
    int col[N] = { N };

    int d = shortestPathBinaryMatrix(grid, N, col);
    assert(d == -1);
#undef N
}

void Test03(void)
{
#define N 2
    int map[N][N] = { {0,1}, {1,0} };
    int *grid[N] = { map[0], map[1] };
    int col[N] = { N, N };

    int d = shortestPathBinaryMatrix(grid, N, col);
    assert(d == 2);
#undef N
}

void Test04(void)
{
#define N 3
    int map[N][N] = { {0,0,0},{1,1,0},{1,1,0} };
    int *grid[N] = { map[0], map[1], map[2] };
    int col[N] = { N, N, N };

    int d = shortestPathBinaryMatrix(grid, N, col);
    assert(d == 4);
#undef N
}

void Test05(void)
{
#define N 3
    int map[N][N] = { {0,0,0},{0,1,0},{0,0,0} };
    int *grid[N] = { map[0], map[1], map[2] };
    int col[N] = { N, N, N };

    int d = shortestPathBinaryMatrix(grid, N, col);
    assert(d == 4);
#undef N
}

int main(void)
{
    Test01();
    Test02();
    Test03();
    Test04();
    Test05();

    return 0;
}

#endif