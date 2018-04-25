#include <iostream>
#include <string>
#include <cstring>
#include <bitset>
#include <cmath>
using namespace std;

bitset<20> oriwall[20];
bitset<20> trywall[20];
bitset<20> paintpoint[20];

void Setori(int a, int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            paintpoint[i][j] = 0;
        }
    }
    for (int j = 1; j <= n; ++j) paintpoint[1][j] = (a >> (j-1)) & 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            trywall[i][j] = oriwall[i][j];
        }
    }
}

bool paint(int n) {
    for (int j = 1; j <= n; ++j) {
        if (paintpoint[1][j]) {
            trywall[1][j] = !trywall[1][j];
            trywall[1][j-1] = !trywall[1][j-1];
            trywall[1][j+1] = !trywall[1][j+1];
            trywall[2][j] = !trywall[2][j];
        }
    }
    for (int i = 2; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (trywall[i-1][j] == 0) paintpoint[i][j] = 1;
        }
        for (int j = 1; j <= n; ++j) {
            if (paintpoint[i][j]) {
                trywall[i][j] = !trywall[i][j];
                trywall[i][j-1] = !trywall[i][j-1];
                trywall[i][j+1] = !trywall[i][j+1];
                trywall[i+1][j] = !trywall[i+1][j];
                trywall[i-1][j] = !trywall[i-1][j];
            }
        }
    }
    for (int j = 1; j <= n; ++j) {
        if (trywall[n][j] == 0) return false;
    }

    return true;
}

int countpoint(int n) {
    int count = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (paintpoint[i][j]) count++;
        }
    }
    return count;
}

int main() {
    int n, i, j, count, min = 1 << 16;
    char c;
    cin >> n;
    for (i = 1; i <= n; ++i) {
        for (j = 1; j <= n; ++j) {
            cin >> c;
            if (c == 'w') oriwall[i][j] = 0;
            else oriwall[i][j] = 1;
        }
    }
    int power = int(pow(2, n));
    for (i = 0; i < power; ++i) {
        Setori(i, n);
        if (paint(n)) {
            count = countpoint(n);
            if (count < min) min = count;
        }
    }
    if (min != 1 << 16) cout << min << endl;
    else cout << "inf" << endl;
    return 0;
}