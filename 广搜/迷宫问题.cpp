#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

struct node {
    int xx;
    int yy;
    int pre;
    node () {}
    node (int x, int y, int p) {
        xx = x;
        yy = y;
        pre = p;
    }
};


int main() {
    int maze[10][10];
    int visited[10][10];
    node path[100];
    memset(visited, 0, sizeof(visited));
    memset(maze, 1, sizeof(maze));
    for (int i = 1; i <= 5; ++i) {
        for (int j = 1; j <= 5; ++j) {
            cin >> maze[i][j];
        }
    }
    int begin = 0, end = 1;
    path[begin] = node(1, 1, 0);
    while (begin != end) {
        int x = path[begin].xx, y = path[begin].yy;
        if (x == 5 && y == 5) break;
        if (visited[x][y]) {
            begin++;
            continue;
        }
        visited[x][y] = 1;
        if (x != 1 && maze[x-1][y] != 1) path[end++] = node(x-1, y, begin);
        if (x != 5 && maze[x+1][y] != 1) path[end++] = node(x+1, y, begin);
        if (y != 1 && maze[x][y-1] != 1) path[end++] = node(x, y-1, begin);
        if (y != 5 && maze[x][y+1] != 1) path[end++] = node(x, y+1, begin);
        ++begin;
    }
    vector<pair<int, int> > ans;
    while (begin) {
        ans.push_back(make_pair(path[begin].xx, path[begin].yy));
        begin = path[begin].pre;
    }
    printf("(0, 0)\n");
    for (auto i = ans.rbegin(); i != ans.rend(); ++i) {
        printf("(%d, %d)\n", i -> first - 1, i -> second - 1);
    }
}