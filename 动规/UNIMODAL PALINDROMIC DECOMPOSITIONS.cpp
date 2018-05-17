#include <iostream>
using namespace std;

int main() {
    long long int count[300] = {0}, tmp[300][300] = {0};
    for (int i = 1; i < 300; ++i) tmp[0][i] = tmp[1][i] = tmp[i][1] = 1;
    for (int i = 2; i < 250; ++i) {
        for (int j = 2; j < 250; ++j) {
            if (j <= i) {
                for (int k = 1; k <= j; ++k) {
                    tmp[i][j] += tmp[i-k][k];
                }
            }
            else tmp[i][j] = tmp[i][i];
        }
    }
    for (int i = 1; i < 250; ++i) {
        for (int j = 1; j <= i; ++j) {
            if ((i - j) % 2 == 0) count[i] += tmp[(i-j)/2][j];
        }
        if (i % 2 == 0) count[i] += tmp[i/2][i/2];
    }
    int n;
    while (cin >> n) {
        if (n == 0) break;
        cout << n << " " << count[n] << endl;
    }
    return 0;
}