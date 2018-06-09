#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <bitset>
using namespace std;

struct node {
    int x;
    int y;
    int kill;
    int key;
    int time;
    node() {}
    node(int xx, int yy, int ki, int ke, int t) {
        x = xx;
        y = yy;
        kill = ki;
        key = ke;
        time = t;
    }
    bool operator< (node n) const {
        return time > n.time;
    }
};

int visited[100][100][(1 << 5) + 10][10];

int main() {
    int n, m;
    while (cin >> n >> m && n) {
        char maze[n][n];
        int beginx, beginy, endx, endy;
        vector<pair<int, int> > snake;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> maze[i][j];
                if (maze[i][j] == 'K') {
                    beginx = i;
                    beginy = j;
                }
                else if (maze[i][j] == 'S') {
                    snake.push_back(make_pair(i, j));
                }
            }
        }
        memset(visited, 0, sizeof(visited));
        priority_queue<node> path;
        path.push(node(beginx, beginy, 0, 0, 0));
        while (!path.empty()) {
            node tmp = path.top();
            if (maze[tmp.x][tmp.y] == 'T' && tmp.key == m) break;
            if (visited[tmp.x][tmp.y][tmp.kill][tmp.key] || maze[tmp.x][tmp.y] == '#') {
                path.pop();
                continue;
            }
            path.pop();
            visited[tmp.x][tmp.y][tmp.kill][tmp.key] = true;
            if (maze[tmp.x][tmp.y] == 'S') {
                int i;
                for (i = 0; i < snake.size(); ++i) {
                    if (tmp.x == snake[i].first && tmp.y == snake[i].second) break;
                }
                if (!(tmp.kill >> i & 1)) {
                    if (tmp.x != 0)
                        path.push(node(tmp.x-1, tmp.y, tmp.kill + (1 << i), tmp.key, tmp.time+2));
                    if (tmp.x != n - 1)
                        path.push(node(tmp.x+1, tmp.y, tmp.kill + (1 << i), tmp.key, tmp.time+2));
                    if (tmp.y != 0)
                        path.push(node(tmp.x, tmp.y-1, tmp.kill + (1 << i), tmp.key, tmp.time+2));
                    if (tmp.y != n - 1)
                        path.push(node(tmp.x, tmp.y+1, tmp.kill + (1 << i), tmp.key, tmp.time+2));
                    continue;
                }
            }
            if (maze[tmp.x][tmp.y] >= '0' && maze[tmp.x][tmp.y] <= '9') {
                int i = maze[tmp.x][tmp.y] - '0';
                if (tmp.key == i - 1) {
                    if (tmp.x != 0)
                        path.push(node(tmp.x-1, tmp.y, tmp.kill, i, tmp.time+1));
                    if (tmp.x != n - 1)
                        path.push(node(tmp.x+1, tmp.y, tmp.kill, i, tmp.time+1));
                    if (tmp.y != 0)
                        path.push(node(tmp.x, tmp.y-1, tmp.kill, i, tmp.time+1));
                    if (tmp.y != n - 1)
                        path.push(node(tmp.x, tmp.y+1, tmp.kill, i, tmp.time+1));
                    continue;
                }
            }
            if (tmp.x != 0)
                path.push(node(tmp.x-1, tmp.y, tmp.kill, tmp.key, tmp.time+1));
            if (tmp.x != n - 1)
                path.push(node(tmp.x+1, tmp.y, tmp.kill, tmp.key, tmp.time+1));
            if (tmp.y != 0)
                path.push(node(tmp.x, tmp.y-1, tmp.kill, tmp.key, tmp.time+1));
            if (tmp.y != n - 1)
                path.push(node(tmp.x, tmp.y+1, tmp.kill, tmp.key, tmp.time+1));
        }
        if (path.empty()) cout << "impossible" << endl;
        else cout << path.top().time << endl;
    }
}