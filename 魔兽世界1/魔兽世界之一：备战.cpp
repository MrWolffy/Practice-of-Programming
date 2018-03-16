//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/12.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

void PrintWarName(int t) {
    switch (t) {
        case 0: cout << "dragon "; break;
        case 1: cout << "ninja "; break;
        case 2: cout << "iceman "; break;
        case 3: cout << "lion "; break;
        case 4: cout << "wolf "; break;
        default: break;
    }
}

class Warrior {
public:
    int type;
    int strength;
    Warrior(int t, int s):type(t), strength(s) {}
};

class HeadQuarter {
public:
    int type;
    int element;
    int order[5];
    int count[5];
    int WarNum;
    Warrior *warriors[1000];
    int now;
    bool stop;
    HeadQuarter(int e, int *a) {
        element = e;
        for (int i = 0; i < 5; i++) {
            order[i] = a[i];
            count[i] = 0;
        }
        stop = false;
        now = 0;
        WarNum = 0;
    }
    bool MakeWarrior(int time, int *elements) {
        if (stop) return false;
        int i, tmp;
        printf("%03d ", time);
        if (type) cout << "blue ";
        else cout << "red ";
        for (i = 0; i < 5; i++) {
            tmp = order[(now+i)%5];
            if (elements[tmp] <= element) {
                warriors[WarNum] = new Warrior(tmp, elements[tmp]);
                element -= elements[tmp];
                count[tmp]++;
                PrintWarName(tmp);
                cout << ++WarNum << ' ';
                cout << "born with strength ";
                cout << warriors[WarNum-1] -> strength << ',';
                cout << count[tmp] << ' ';
                PrintWarName(tmp);
                cout << "in ";
                if (type) cout << "blue ";
                else cout << "red ";
                cout << "headquarter" << endl;
                now = (now + i + 1) % 5;
                return true;
            }
        }
        if (i == 5) {
            stop = true;
            cout << "headquarter stops making warriors" << endl;
        }
        return false;
    }
};

int main() {
    int cases, m, elements[5], time;
    int order_red[5] = {2, 3, 4, 1, 0}, order_blue[5] = {3, 0, 1, 2, 4};
    cin >> cases;
    for (int j = 1; j <= cases; j++) {
        cout << "Case:" << j << endl;
        time = 0;
        cin >> m;
        for (int i = 0; i < 5; i++) {
            cin >> elements[i];
        }
        HeadQuarter red(m, order_red), blue(m, order_blue);
        red.type = 0; blue.type = 1;
        while (!red.stop || !blue.stop) {
            red.MakeWarrior(time, elements);
            blue.MakeWarrior(time, elements);
            time++;
        }
    }
    return 0;
}

