#include <iostream>
#include <cstring>
using namespace std;

struct node {
    int x;
    int y;
} best[30], path[30];
int minsteps = 0xffff;
int depth = 0;

int map[7][7];
int visited[7][7];

void dfs(int xx, int yy) {
    if (xx == 5 && yy == 5) {
        path[depth].x = xx; path[depth].y = yy;
        if (depth < minsteps) {
            minsteps = depth;
            memcpy(best, path, sizeof(best));
        }
        return;
    }
    if (visited[xx][yy]) return;
    if (depth >= minsteps) return;
    visited[xx][yy] = 1;
    path[depth].x = xx; path[depth].y = yy;
    ++depth;
    dfs(xx, yy + 1); dfs(xx + 1, yy); dfs(xx, yy - 1); dfs(xx - 1, yy);
    visited[xx][yy] = 0;
    --depth;
}

int main() {
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
            map[i][j] = visited[i][j] = 1;
    for (int i = 1; i <= 5; ++i) {
        for (int j = 1; j <= 5; ++j) {
            cin >> map[i][j];
            if (map[i][j]) visited[i][j] = 1;
            else visited[i][j] = 0;
        }
    }
    dfs(1, 1);
    if (minsteps != 0xffff)
        for (int i = 0; i <= minsteps; ++i)
            printf("(%d, %d)\n", best[i].x - 1, best[i].y - 1);
}