//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/17.
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
    return;
}

void PrintHqName(int t) {
    if (t) cout << "blue ";
    else cout << "red ";
    return;
}

void PrintWpName(int t) {
    switch (t) {
        case 0: cout << "a sword"; break;
        case 1: cout << "a bomb"; break;
        case 2: cout << "a arrow"; break;
    }
    return;
}

class Weapon {
public:
    int type;
    Weapon(int t): type(t) {}
};

class Sword: public Weapon {
public:
    Sword(int t): Weapon(t) {}
};

class Bomb: public Weapon {
public:
    Bomb(int t): Weapon(t) {}

};

class Arrow: public Weapon {
public:
    Arrow(int t): Weapon(t) {}
};

Weapon * MakeWeapon(int t) {
    switch (t) {
        case 0: return new Sword(0);
        case 1: return new Bomb(1);
        case 2: return new Arrow(2);
    }
}

class Warrior {
public:
    int type;
    int strength;
    static int count[2][5];
    Warrior(int t, int s):type(t), strength(s) {}
};

class Dragon: public Warrior {
public:
    Weapon *wp;
    double morale;
    Dragon(int t, int s, int e, int n):Warrior(t, s) {
        morale = ((e - s) * 1.0) / (s * 1.0);
        wp = MakeWeapon((n + 1) % 3);
    }
    void PrintInfo() {
        printf("It has ");
        PrintWpName(wp -> type);
        printf(",and it's morale is %.2f\n", morale);
    }
};

class Ninja: public Warrior {
public:
    Weapon *wp1, *wp2;
    Ninja(int t, int s, int e, int n):Warrior(t, s) {
        wp1 = MakeWeapon((n + 1) % 3);
        wp2 = MakeWeapon((n + 2) % 3);
    }
    void PrintInfo() {
        printf("It has ");
        PrintWpName(wp1 -> type);
        printf(" and ");
        PrintWpName(wp2 -> type);
        printf("\n");
    }
};

class Iceman: public Warrior {
public:
    Weapon *wp;
    Iceman(int t, int s, int e, int n):Warrior(t, s) {
        wp = MakeWeapon((n + 1) % 3);
    }
    void PrintInfo() {
        printf("It has ");
        PrintWpName(wp -> type);
        printf("\n");
    }
};

class Lion: public Warrior {
public:
    int loyalty;
    Lion(int t, int s, int e, int n):Warrior(t, s) {
        loyalty = e - s;
    }
    void PrintInfo() {
        printf("It's loyalty is %d\n", loyalty);
    }
};

class Wolf: public Warrior {
public:
    Wolf(int t, int s, int e, int n):Warrior(t, s) {}
};

class HeadQuarter {
public:
    int type;
    int element;
    int order[5];
    int WarNum;
    Warrior *warriors[10000];
    int now;
    bool stop;
    HeadQuarter(int e, int *a) {
        element = e;
        for (int i = 0; i < 5; i++) {
            order[i] = a[i];
        }
        stop = false;
        now = 0;
        WarNum = 0;
    }
    bool MakeWarrior(int time, int *elements) {
        if (stop) return false;
        int i, tmp;
        for (i = 0; i < 5; i++) {
            tmp = order[(now+i)%5];
            if (elements[tmp] <= element) {
                Warrior::count[type][tmp]++;
                switch (tmp) {
                    case 0: {
                        warriors[WarNum] = new Dragon(tmp, elements[tmp], element, WarNum);
                        PrintWarInfo(time, tmp, elements);
                        ((Dragon *)warriors[WarNum-1]) -> PrintInfo();
                        break;
                    }
                    case 1: {
                        warriors[WarNum] = new Ninja(tmp, elements[tmp], element, WarNum);
                        PrintWarInfo(time, tmp, elements);
                        ((Ninja *)warriors[WarNum-1]) -> PrintInfo();
                        break;
                    }
                    case 2: {
                        warriors[WarNum] = new Iceman(tmp, elements[tmp], element, WarNum);
                        PrintWarInfo(time, tmp, elements);
                        ((Iceman *)warriors[WarNum-1]) -> PrintInfo();
                        break;
                    }
                    case 3: {
                        warriors[WarNum] = new Lion(tmp, elements[tmp], element, WarNum);
                        PrintWarInfo(time, tmp, elements);
                        ((Lion *)warriors[WarNum-1]) -> PrintInfo();
                        break;
                    }
                    case 4: {
                        warriors[WarNum] = new Wolf(tmp, elements[tmp], element, WarNum);
                        PrintWarInfo(time, tmp, elements);
                        break;
                    }
                }
                element -= elements[tmp];
                now = (now + i + 1) % 5;
                return true;
            }
        }
        if (i == 5) {
            stop = true;
            printf("%03d ", time);
            PrintHqName(type);
            cout << "headquarter stops making warriors" << endl;
        }
        return false;
    }
    void PrintWarInfo(int time, int tmp, int *elements) {
        printf("%03d ", time);
        PrintHqName(type);
        PrintWarName(tmp);
        cout << ++WarNum << ' ';
        cout << "born with strength ";
        cout << warriors[WarNum-1] -> strength << ',';
        cout << Warrior::count[type][tmp] << ' ';
        PrintWarName(tmp);
        cout << "in ";
        PrintHqName(type);
        cout << "headquarter" << endl;
    }
};

int Warrior::count[2][5] = {0};

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
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 5; j++) {
                Warrior::count[i][j] = 0;
            }
        }
    }
    return 0;
}