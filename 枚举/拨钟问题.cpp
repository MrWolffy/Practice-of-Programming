#include <iostream>
#include <cmath>
using namespace std;

int oriclock[9], tryclock[9], moveclock[9];

void Setori(int a) {
    for (int i = 0; i < 9; ++i) {
        moveclock[i] = a % 4;
        a = a / 4;
    }
    for (int i = 0; i < 9; ++i) tryclock[i] = oriclock[i];
}

void moveone(int n) {
    tryclock[n] = (tryclock[n] + 1) % 4;
}

bool canmove() {
    for (int i = 0; i < moveclock[0]; ++i) {moveone(0); moveone(1); moveone(3); moveone(4);};
    for (int i = 0; i < moveclock[1]; ++i) {moveone(0); moveone(1); moveone(2);};
    for (int i = 0; i < moveclock[2]; ++i) {moveone(1); moveone(2); moveone(4); moveone(5);};
    for (int i = 0; i < moveclock[3]; ++i) {moveone(0); moveone(3); moveone(6);};
    for (int i = 0; i < moveclock[4]; ++i) {moveone(1); moveone(3); moveone(4); moveone(5); moveone(7);};
    for (int i = 0; i < moveclock[5]; ++i) {moveone(2); moveone(5); moveone(8);};
    for (int i = 0; i < moveclock[6]; ++i) {moveone(3); moveone(4); moveone(6); moveone(7);};
    for (int i = 0; i < moveclock[7]; ++i) {moveone(6); moveone(7); moveone(8);};
    for (int i = 0; i < moveclock[8]; ++i) {moveone(4); moveone(5); moveone(7); moveone(8);};
    for (int i = 0; i < 9; ++i) {
        if (tryclock[i] != 0) return false;
    }
    return true;
}

int main() {
    for (int i = 0; i < 9; ++i) cin >> oriclock[i];
    int max = int(pow(4, 9));
    for (int i = 0; i < max; ++i) {
        Setori(i);
        if (canmove()) break;
    }
    for (int i = 0; i < 9; ++i) {
        if (moveclock[i]) {
            for (int j = 0; j < moveclock[i]; ++j) cout << i + 1 << ' ';
        }
    }
    return 0;
}