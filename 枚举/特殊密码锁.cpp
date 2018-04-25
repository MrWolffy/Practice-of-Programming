#include <iostream>
#include <bitset>
#include <cmath>
using namespace std;

bitset<40> from, to, trybit, movebit;

void setori(int n) {
    for (int i = 0; i < 40; ++i) {
        movebit[i] = from[i];
        trybit[i] = 0;
    }
    trybit[1] = n;
}

bool canmove(int n) {
    if (trybit[1]) {
        movebit[1] = !movebit[1];
        movebit[2] = !movebit[2];
    }
    for (int i = 2; i <= n; ++i) {
        if (to[i-1] != movebit[i-1]) {
            trybit[i] = 1;
            movebit[i-1] = !movebit[i-1];
            movebit[i] = !movebit[i];
            movebit[i+1] = !movebit[i+1];
        }
    }
    for (int i = 1; i <= n; ++i) {
        if (movebit[i] != to[i]) return false;
    }
    return true;
}

int main() {
    string s;
    cin >> s;
    int len = s.length(), i;
    for (int i = 0; i < len; ++i) {
        from[i+1] = (s[i] == '1');
    }
    cin >> s;
    for (int i = 0; i < len; ++i) {
        to[i+1] = (s[i] == '1');
    }
    for (i = 0; i < 2; ++i) {
        setori(i);
        if (canmove(len)) break;
    }
    if (i == 2) cout << "impossible";
    else {
        int count = 0;
        for (i = 1; i <= len; ++i) {
            if (trybit[i]) count++;
        }
        cout << count;
    }
    return 0;
}