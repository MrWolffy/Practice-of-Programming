#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

int map[20][20];
int dp[1 << 16][16][16];

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < (1 << n); ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                dp[i][j][k] = 0xffffff;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> map[i][j];
    for (int i = 0; i < n; ++i) dp[1 << i][i][i] = 0;
    for (int i = 1; i < (1 << n); ++i) {
        for (int j = 0; j < n; ++j) {
            if ((i >> j) & 1 && i != (1 << j)) {
                for (int k = 0; k < n; ++k) {
                    if ((i >> k) & 1 && k != j) {
                        for (int m = 0; m < n; ++m) {
                            if ((i >> m) & 1 && m != k) {
                                dp[i][j][k] = min(dp[i - (1 << k)][j][m] + map[m][k], dp[i][j][k]);
                            }
                        }
                    }
                }
            }
        }
    }
    cout << dp[(1 << n) - 1][0][n-1] << endl;
    return 0;
}