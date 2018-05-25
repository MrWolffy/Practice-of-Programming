#include <iostream>
#include <algorithm>
using namespace std;

int f[20000] = {0};

int main() {
    int n, m, w[4000] = {0}, d[4000] = {0};
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) cin >> w[i] >> d[i];
    for (int i = 1; i <= m; ++i) {
        if (w[1] <= i) f[i] = d[1];
        else f[i] = 0;
    }
    for (int i = 2; i <= n; ++i) {
        for (int j = m; j >= 1; --j) {
            if (j - w[i] >= 0) f[j] = max(f[j], f[j-w[i]] + d[i]);
        }
    }
    cout << f[m] << endl;
}