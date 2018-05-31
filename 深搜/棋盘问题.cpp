#include <iostream>
#include <cstring>
using namespace std;

struct node {
    int x;
    int y;
} ori[100];
int totalnode = 0;
int visited[100];
int cnt;
int depth;
int n, k;

void dfs(int m) {
    if (depth == k - 1) {
        ++cnt;
        return;
    }
    ++depth;
    visited[m] = 1;
    for (int i = m + 1; i < totalnode; ++i) {
        bool flag = true;
        for (int j = 0; j <= m; ++j) {
            if (visited[j] && (ori[i].x == ori[j].x || ori[i].y == ori[j].y)) {
                flag = false;
                break;
            }
        }
        if (flag) dfs(i);
    }
    visited[m] = 0;
    --depth;
}

int main() {
    char c;
    while (1) {
        cin >> n >> k;
        if (n == -1) return 0;
        depth = cnt = totalnode = 0;
        memset(visited, 0, sizeof(visited));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> c;
                if (c == '#') {
                    ori[totalnode].x = i;
                    ori[totalnode].y = j;
                    ++totalnode;
                }
            }
        }
        for (int i = 0; i < totalnode; ++i) dfs(i);
        cout << cnt << endl;
    }
}