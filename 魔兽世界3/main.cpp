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


class Weapon;
class Sword;
class Bomb;
class Arrow;
class Warrior;
class Dragon;
class Ninja;
class Iceman;
class Lion;
class Wolf;
class HeadQuarter;
class City;
inline void PrintWarName(int t);
inline void PrintHqName(int t);
inline bool PrintWp(Weapon *wp);
Weapon * MakeWeapon(int t, int force, int r);



class City {
public:
    int id;
    int element;
    Warrior *red, *blue;
    City() {}
    City(int n): id(n) {
        red = blue = NULL;
        element = 0;
    }
};
City cities[20];

class Weapon {
public:
    int type;
    Weapon(int t): type(t) {}
};

class Sword: public Weapon {
public:
    int force;
    Sword(int t, int force_): Weapon(t) {
        force = (int)(force_ * 0.2);
    }
};

class Bomb: public Weapon {
public:
    Bomb(int t): Weapon(t) {}
};

class Arrow: public Weapon {
public:
    int use;
    int force;
    Arrow(int t, int r): Weapon(t) {
        use = 3;
        force = r;
    }
};

class Warrior {
public:
    int type;
    int strength;
    int id;
    int force;
    HeadQuarter *hq;
    int city;
    Warrior(int t, int s, int n, HeadQuarter *hq_, int force);
    void March(int t, int n);
    void ReportWp(int time);
};

class Dragon: public Warrior {
public:
    Weapon *wp;
    double morale;
    Dragon(int t, int s, int e, int n, HeadQuarter * hq, int force, int r):Warrior(t, s, n, hq, force) {
        morale = ((e - s) * 1.0) / (s * 1.0);
        wp = MakeWeapon((n + 1) % 3, force, r);
    }
    void PrintInfo() {
        printf("Its morale is %.2f\n", morale);
    }
    void ReportWp() {
        if (!PrintWp(wp)) printf("no weapon");
    }
};

class Ninja: public Warrior {
public:
    Weapon *wp1, *wp2;
    Ninja(int t, int s, int e, int n, HeadQuarter * hq, int force, int r):Warrior(t, s, n, hq, force) {
        wp1 = MakeWeapon((n + 1) % 3, force, r);
        wp2 = MakeWeapon((n + 2) % 3, force, r);
    }
    void ReportWp() {
        if (wp1 == NULL && wp2 == NULL) printf("no weapon");
        if (wp1 != NULL && wp1 -> type == 2) PrintWp(wp1);
        else if (wp2 != NULL && wp2 -> type == 2) PrintWp(wp2);
        if (wp1 != NULL && wp1 -> type == 1) PrintWp(wp1);
        else if (wp2 != NULL && wp2 -> type == 1) PrintWp(wp2);
        if (wp1 != NULL && wp1 -> type == 0) PrintWp(wp1);
        else if (wp2 != NULL && wp2 -> type == 0) PrintWp(wp2);
    }
};

class Iceman: public Warrior {
public:
    Weapon *wp;
    Iceman(int t, int s, int e, int n, HeadQuarter * hq, int force, int r):Warrior(t, s, n, hq, force) {
        wp = MakeWeapon((n + 1) % 3, force, r);
    }
    void ReportWp() {
        if (!PrintWp(wp)) printf("no weapon");
    }
};

class Lion: public Warrior {
public:
    int loyalty;
    Lion(int t, int s, int e, int n, HeadQuarter * hq, int force, int r):Warrior(t, s, n, hq, force) {
        loyalty = e - s;
    }
    void PrintInfo() {
        printf("Its loyalty is %d\n", loyalty);
    }
    void ReportWp() {
        printf("no weapon");
    }
};

class Wolf: public Warrior {
public:
    Weapon *wp1, *wp2, *wp3;
    Wolf(int t, int s, int e, int n, HeadQuarter * hq, int force, int r):Warrior(t, s, n, hq, force) {
        wp1 = wp2 = wp3 = NULL;
    }
    void ReportWp() {
        if (!PrintWp(wp3) && !PrintWp(wp2) && !PrintWp(wp1)) printf("no weapon");
    }
};

class HeadQuarter {
public:
    int type;
    int element;
    int order[5];
    int WarNum;
    Warrior *warriors[10000];
    int now;
    bool taken;
    int location;
    HeadQuarter(int e, int *a) {
        element = e;
        for (int i = 0; i < 5; i++) {
            order[i] = a[i];
        }
        now = 0;
        WarNum = 0;
        taken = false;
    }
    void MakeWarrior(int time, int *elements, int *force, int r);
    inline void PrintWarInfo(int time, int tmp, int *elements) {
        printf("%03d:00 ", time);
        PrintHqName(type);
        PrintWarName(tmp);
        cout << ++WarNum << ' ';
        cout << "born" << endl;
    }
    void LionRunAway(int i, int time);
    void TakeHq(HeadQuarter *src, HeadQuarter *dst, int time);
    void ReportElement(int time);
};





Warrior::Warrior(int t, int s, int n, HeadQuarter *hq_, int force_) {
    type = t;
    strength = s;
    id = n + 1;
    force = force_;
    hq = hq_;
    city = hq_ -> location;
}
void Warrior::March(int time, int n) {
    if (strength <= 0) return;
    if (hq -> type == 0) {
        if (city == n + 1) return;
        cities[city].red = NULL;
        city = city + 1;
        cities[city].red = this;
    }
    if (hq -> type == 1) {
        if (city == 0) return;
        cities[city].blue = NULL;
        city = city - 1;
        cities[city].blue = this;
    }
    printf("%03d:00 ", time);
    PrintHqName(hq -> type);
    PrintWarName(type);
    printf("%d ", id);
    if (city != n + 1 && city != 0) printf("marched to city %d ", city);
    else {
        printf("reached ");
        PrintHqName(!(hq -> type));
        printf("headquarter ");
    }
    printf("with %d elements and force %d\n", strength, force);
}
void Warrior::ReportWp(int time) {
    printf("%03d:55 ", time);
    PrintHqName(hq -> type);
    PrintWarName(type);
    printf("%d has ", id);
    switch (type) {
        case 0: ((Dragon *)this) -> ReportWp(); break;
        case 1: ((Ninja *)this) -> ReportWp(); break;
        case 2: ((Iceman *)this) -> ReportWp(); break;
        case 3: ((Lion *)this) -> ReportWp(); break;
        case 4: ((Wolf *)this) -> ReportWp(); break;
        default: break;
    }
    printf("\n");
}

void HeadQuarter::MakeWarrior(int time, int *elements, int *force, int r) {
    int tmp = order[now];
    if (elements[tmp] <= element) {
        switch (tmp) {
            case 0: {
                warriors[WarNum] = new Dragon(tmp, elements[tmp], element, WarNum, this, force[tmp], r);
                PrintWarInfo(time, tmp, elements);
                ((Dragon *)warriors[WarNum-1]) -> PrintInfo();
                break;
            }
            case 1: {
                warriors[WarNum] = new Ninja(tmp, elements[tmp], element, WarNum, this, force[tmp], r);
                PrintWarInfo(time, tmp, elements);
                break;
            }
            case 2: {
                warriors[WarNum] = new Iceman(tmp, elements[tmp], element, WarNum, this, force[tmp], r);
                PrintWarInfo(time, tmp, elements);
                break;
            }
            case 3: {
                warriors[WarNum] = new Lion(tmp, elements[tmp], element, WarNum, this, force[tmp], r);
                PrintWarInfo(time, tmp, elements);
                ((Lion *)warriors[WarNum-1]) -> PrintInfo();
                break;
            }
            case 4: {
                warriors[WarNum] = new Wolf(tmp, elements[tmp], element, WarNum, this, force[tmp], r);
                PrintWarInfo(time, tmp, elements);
                break;
            }
        }
        element -= elements[tmp];
        now = (now + 1) % 5;
    }
    return;
}
void HeadQuarter::LionRunAway(int i, int time) {
    if (warriors[i] -> type == 3) {
        if (((Lion *)warriors[i]) -> loyalty < 0) {
            warriors[i] -> strength = -1;
            printf("%03d:05 ", time);
            PrintHqName(type);
            printf("lion %d run away\n", warriors[i] -> id);
        }
    }
}
void HeadQuarter::TakeHq(HeadQuarter *src, HeadQuarter *dst, int time) {
    int count = 0;
    for (int i = 0; i < src -> WarNum; i++) {
        if (src -> warriors[i] -> city == dst -> location) count++;
    }
    if (count >= 2) {
        dst -> taken = true;
        printf("%03d:10 ", time);
        PrintHqName(dst -> type);
        printf("headquarter was taken\n");
    }
}
void HeadQuarter::ReportElement(int time) {
    printf("%d elements in ", element);
    PrintHqName(type);
    printf("headquarter\n");
}






inline void PrintWarName(int t) {
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
inline void PrintHqName(int t) {
    if (t) cout << "blue ";
    else cout << "red ";
    return;
}
inline bool PrintWp(Weapon *wp) {
    if (wp == NULL) return false;
    switch (wp -> type) {
        case 0: cout << "sword"; break;
        case 1: cout << "bomb"; break;
        case 2: cout << "arrow"; break;
    }
    switch (wp -> type) {
        case 0: printf("(%d)", ((Sword *)wp) -> force); break;
        case 1: break;
        case 2: printf("(%d)", ((Arrow *)wp) -> use); break;
    }
    return true;
}
Weapon * MakeWeapon(int t, int force, int r) {
    switch (t) {
        case 0: {
            Sword *tmp = new Sword(0, force);
            if (tmp -> force == 0) tmp = NULL;
            return (Weapon *)tmp;
        }
        case 1: return new Bomb(1);
        case 2: return new Arrow(2, r);
    }
    return NULL;
}

int main() {
    //int cases, elements[5], force[5], time;
    int cases, time;
    int m, n, r, k, t;
    int order_red[5] = {2, 3, 4, 1, 0}, order_blue[5] = {3, 0, 1, 2, 4};
    //cin >> cases;
    cases = 1;
    for (int j = 1; j <= cases; j++) {
        cout << "Case:" << j << endl;
        time = 0;
        //cin >> m >> n >> r >> k >> t;
        m = 20; n = 1; r = 10; k = 10; t = 1000;
        t = 5;
        //for (int i = 0; i < 5; i++) cin >> elements[i];
        int elements[5] = {10, 20, 30, 10, 20};
        //for (int i = 0; i < 5; i++) cin >> force[i];
        int force[5] = {5, 5, 5, 5, 5};
        for (int i = 0; i < n; i++) cities[i] = City(i);
        HeadQuarter red(m, order_red), blue(m, order_blue);
        red.type = 0; blue.type = 1;
        red.location = 0; blue.location = n + 1;
        while (time < t) {
            // time: 00 （1）武士降生
            red.MakeWarrior(time, elements, force, r);
            blue.MakeWarrior(time, elements, force, r);

            // time: 05 （2）lion逃跑
            /*for (int i = 0; i < red.WarNum && i < blue.WarNum; i++) {
                red.LionRunAway(i, time);
                blue.LionRunAway(i, time);
            }*/

            // time: 10 （3）武士前进 （12）武士抵达敌军司令部
            for (int i = 0; i < red.WarNum || i < blue.WarNum; i++) {
                if (i < red.WarNum) red.warriors[i] -> March(time, n);
                if (i < blue.WarNum) blue.warriors[i] -> March(time, n);
            }
            // time: 10 （13）司令部被占领
            red.TakeHq(&red, &blue, time);
            blue.TakeHq(&blue, &red, time);
            if (red.taken || blue.taken) break;

            // time: 20
            for (int i = 1; i <= n; i++) cities[i].element += 10;

            // time: 50 （14）司令部报告生命元数量
            red.ReportElement(time);
            blue.ReportElement(time);

            // time: 55 （15）武士报告武器情况
            for (int i = 0; i < red.WarNum; i++) {
                red.warriors[i] -> ReportWp(time);
            }
            for (int i = 0; i < blue.WarNum; i++) {
                blue.warriors[i] -> ReportWp(time);
            }
            time++;
        }
        // 清理cities
    }
    return 0;
}

