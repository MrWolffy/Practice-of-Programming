#include <iostream>
#include <cmath>
using namespace std;

char a[1024][2048];

void draw(int x, int y, int n) {
    if (n == 1) {
        a[x][y] = a[x-1][y+1] = '/';
        a[x][y+1] = a[x][y+2] = '_';
        a[x-1][y+2] = a[x][y+3] = '\\';
    }
    else {
        draw(x, y, n - 1);
        draw(x, y + int(pow(2, n)), n - 1);
        draw(x - int(pow(2, n - 1)), y + int(pow(2, n - 1)), n - 1);
    }
}

void print(int n) {
    int power = int(pow(2, n));
    for (int i = 0; i < power; ++i) {
        for (int j = 0; j < power + 1 + i; ++j) {
            printf("%c", a[i][j]);
        }
        printf("\n");
    }
}

void clear(int n) {
    int power = int(pow(2, n));
    int power2 = power * 2;
    for (int i = 0; i < power; ++i) {
        for (int j = 0; j < power2; ++j) {
            a[i][j] = ' ';
        }
    }
}

int main() {
    int n;
    while (1) {
        scanf("%d", &n);
        if (n == 0) break;
        else {
            clear(n);
            draw(int(pow(2, n)) - 1, 0, n);
            print(n);
            printf("\n");
        }
    }
    return 0;
}