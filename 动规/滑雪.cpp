#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <set>
using namespace std;

int a[110][110];
int b[110][110];
int dest[2][4] = {{0, 1, 0, -1}, {1, 0, -1, 0}};
int r, c;

int mymax (int x, int y) {
    if (b[x][y] >= 0) return b[x][y];
    int mmax = 0, ans = 0;
    for (int i = 0; i < 4; ++i) {
        int xx = x + dest[0][i];
        int yy = y + dest[1][i];
        if (xx < 1 || xx > r || yy < 1 || yy > c) continue;
        if (a[x][y] > a[xx][yy]) {
            ans = mymax(xx, yy);
            mmax = max(mmax, ans);
        }
    }
    b[x][y] = mmax + 1;
    return b[x][y];
}

int main() {
    for (int i = 0; i < 110; ++i) {
        for (int j = 0; j < 110; ++j) {
            a[i][j] = 0;
            b[i][j] = -1;
        }
    }
    cin >> r >> c;
    for (int i = 1; i <= r; ++i) {
        for (int j = 1; j <= c; ++j) {
            cin >> a[i][j];
        }
    }
    int mmax = -1;
    for (int i = 1; i <= r; ++i) {
        for (int j = 1; j <= c; ++j) {
            b[i][j] = mymax(i, j);
            mmax = max(mmax, b[i][j]);
        }
    }
    cout << mmax << endl;
}