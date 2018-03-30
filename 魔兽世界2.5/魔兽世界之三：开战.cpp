//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/28.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
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
inline void PrintWpName(int t);
inline void PrintWarInfo(Warrior *war);
void BothAlive(Warrior *war1, Warrior *war2, int time);
void BothDead(Warrior *war1, Warrior *war2, int time);



class City {
public:
    int id;
    int element;
    int flag;
    Warrior *red, *blue, *red2, *blue2;
    int now;
    int count;
    City() {}
    City(int n): id(n) {
        red = blue = red2 = blue2 = NULL;
        element = 0;
        flag = -1;
        now = -1;
        count = 0;
    }
    void WarGetElement(int time);
    void RaiseFlag(int time);
};
City cities[30];
class Weapon {
public:
    int type;
    int force;
    Weapon(int t, int f): type(t), force(f) {}
};
class Sword: public Weapon {
public:
    Sword(int t, int f): Weapon(t, f * 2 / 10) {}
};
class Bomb: public Weapon {
public:
    Bomb(int t, int f): Weapon(t, f * 4 / 10) {}
};
class Arrow: public Weapon {
public:
    int use;
    Arrow(int t, int f): Weapon(t, f * 3 / 10) {
        use = 2;
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
    Weapon *weapon[3][10];
    int nWeapon[3];
    Warrior(int time, int s, HeadQuarter *hq_, int force);
    void March(int time, int n, int k);
    void Attack(Warrior *enemy, int time);
    void ReportWp(int time);
    void SortWp();
    void MakeWeapon(int t, int force);
    bool FindNextWp(int *typenow, int *idnow);
    void UseWeapon(Warrior *enemy, int typenow, int idnow);
    void WarWin(Warrior *enemy, int time);

};
class Dragon: public Warrior {
public:
    Dragon(int t, int s, HeadQuarter *hq, int force);
    void Yell(int time) {
        printf("%03d:40 ", time);
        PrintWarInfo(this);
        printf("yelled in city %d\n", city);
    }
};
class Ninja: public Warrior {
public:
    Ninja(int t, int s, HeadQuarter *hq, int force);
};
class Iceman: public Warrior {
public:
    Iceman(int t, int s, HeadQuarter *hq, int force);
};
class Lion: public Warrior {
public:
    int loyalty;
    Lion(int t, int s, HeadQuarter *hq, int force);
    void PrintInfo() {
        printf("Its loyalty is %d\n", loyalty);
    }
    void RunAway(int time);
};
class Wolf: public Warrior {
public:
    Wolf(int t, int s, HeadQuarter *hq, int force);
    void TakeWp(Warrior *enemy, int time);
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
    void MakeWarrior(int time, int *elements, int *force);
    inline void PrintWarInfo(int time, int tmp, int *elements) {
        printf("%03d:00 ", time);
        PrintHqName(type);
        PrintWarName(tmp);
        cout << ++WarNum << ' ';
        cout << "born" << endl;
    }
    void TakeHq(HeadQuarter *src, HeadQuarter *dst, int time);
    void ReportElement(int time);
};





Dragon::Dragon(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {
    MakeWeapon(id % 3, force);
}
Ninja::Ninja(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {
    MakeWeapon(id % 3, force);
    MakeWeapon((id + 1) % 3, force);
}
Iceman::Iceman(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {
    MakeWeapon(id % 3, force);
}
Lion::Lion(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {
    loyalty = hq -> element - s;
    MakeWeapon(id % 3, force);
}
Wolf::Wolf(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {}

Warrior::Warrior(int t, int s, HeadQuarter *hq_, int force_) {
    type = t;
    strength = s;
    id = hq_ -> WarNum + 1;
    force = force_;
    hq = hq_;
    city = hq_ -> location;
    if (hq -> type == 0) cities[0].red = this;
    else cities[hq -> location].blue = this;
    for (int i = 0; i < 10; i++) {
        weapon[0][i] = weapon[1][i] = weapon[2][i] = NULL;
    }
    nWeapon[0] = nWeapon[1] = nWeapon[2] = 0;
}
void Warrior::March(int time, int n, int k) {
    if (strength <= 0) return;
    if (hq -> type == 0) {
        if (city == n + 1) return;
        cities[city++].red = NULL;
        cities[city].red2 = this;
        if (type == 2) strength -= strength / 10;
        if (type == 3) ((Lion *)this) -> loyalty -= k;
    }
    if (hq -> type == 1) {
        if (city == 0) return;
        cities[city--].blue = NULL;
        cities[city].blue2 = this;
        if (type == 2) strength -= strength / 10;
        if (type == 3) ((Lion *)this) -> loyalty -= k;
    }
    printf("%03d:10 ", time);
    PrintWarInfo(this);
    if (city != n + 1 && city != 0) printf("marched to city %d ", city);
    else {
        printf("reached ");
        PrintHqName(!(hq -> type));
        printf("headquarter ");
    }
    printf("with %d elements and force %d\n", strength, force);
}
void Warrior::SortWp() {
    int i, j;
    Weapon *tmp;
    for (i = 0; i < nWeapon[2] - 1; i++) {
        for (j = 0; j < nWeapon[2] - i - 1; j++) {
            if (((Arrow *)weapon[2][j]) -> use < ((Arrow *)weapon[2][j+1]) -> use) {
                tmp = weapon[2][j];
                weapon[2][j] = weapon[2][j+1];
                weapon[2][j+1] = tmp;
            }
        }
    }
}
bool Warrior::FindNextWp(int *typenow, int *idnow) {
    int type_ = *typenow, id_ = *idnow + 1, i = 10;
    while (i--) {
        if (id_ >= nWeapon[type_]) {
            type_ = (type_ + 1) % 3;
            id_ = 0;
        }
        if (weapon[type_][id_]) {
            *typenow = type_;
            *idnow = id_;
            return true;
        }
        id_++;
    }
    return false;
}
void Warrior::UseWeapon(Warrior *enemy, int typenow, int idnow) {
    enemy -> strength -= weapon[typenow][idnow] -> force;
    if (typenow == 1) {
        if (type != 1) strength -= weapon[1][idnow] -> force / 2;
        for (int i = idnow + 1; i < nWeapon[1]; i++) {
            weapon[1][i-1] = weapon[1][i];
        }
        weapon[1][nWeapon[1]-1] = NULL;
        nWeapon[1]--;
    }
    if (typenow == 2) {
        ((Arrow *)weapon[2][idnow]) -> use --;
        if (((Arrow *)weapon[2][idnow]) -> use <= 0) {
            for (int i = idnow + 1; i < nWeapon[2]; i++) {
                weapon[2][i-1] = weapon[2][i];
            }
            weapon[2][nWeapon[2]-1] = NULL;
            nWeapon[2]--;
        }
    }
}
void Warrior::WarWin(Warrior *enemy, int time) {
    printf("%03d:40 ", time);
    PrintWarInfo(this);
    printf("killed ");
    PrintWarInfo(enemy);
    printf("in city %d remaining %d elements\n", city, strength);
    int i = 0;
    while ((nWeapon[0] + nWeapon[1] + nWeapon[2]) < 10 && i < enemy -> nWeapon[0]) {
        weapon[0][nWeapon[0]++] = enemy -> weapon[0][i];
        i++;
    }
    i = 0;
    while ((nWeapon[0] + nWeapon[1] + nWeapon[2]) < 10 && i < enemy -> nWeapon[1]) {
        weapon[1][nWeapon[1]++] = enemy -> weapon[1][i];
        i++;
    }
    i = enemy -> nWeapon[2] - 1;
    while ((nWeapon[0] + nWeapon[1] + nWeapon[2]) < 10 && i >= 0) {
        weapon[2][nWeapon[2]++] = enemy -> weapon[2][i];
        i--;
    }
}
void Warrior::Attack(Warrior *enemy, int time) {
    int typenow[2] = {2, 2}, idnow[2] = {10, 10}, hpnow[2], count = 0;
    while (1) {
        hpnow[0] = strength; hpnow[1] = enemy -> strength;
        if (FindNextWp(&typenow[0], &idnow[0])) {
            UseWeapon(enemy, typenow[0], idnow[0]);
            if (enemy -> strength <= 0) {
                if (strength <= 0) {
                    BothDead(this, enemy, time);
                    break;
                }
                else {
                    WarWin(enemy, time);
                    break;
                }
            }
        }
        if (enemy -> FindNextWp(&typenow[1], &idnow[1])) {
            enemy -> UseWeapon(this, typenow[1], idnow[1]);
            if (strength <= 0) {
                if (enemy -> strength <= 0) {
                    BothDead(this, enemy, time);
                    break;
                }
                else {
                    enemy -> WarWin(this, time);
                    break;
                }
            }
        }
        if (strength == hpnow[0] && enemy -> strength == hpnow[1]) count++;
        else count = 0;
        if (count >= 10) {
            BothAlive(this, enemy, time);
            break;
        }
    }
}
void Warrior::ReportWp(int time) {
    printf("%03d:55 ", time);
    PrintWarInfo(this);
    printf("has %d sword %d bomb %d arrow and %d elements\n", nWeapon[0], nWeapon[1], nWeapon[2], strength);
}
void Warrior::MakeWeapon(int t, int force) {
    switch (t) {
        case 0: {
            weapon[0][nWeapon[0]++] = new Sword(0, force);
            return;
        }
        case 1: {
            weapon[1][nWeapon[1]++] = new Bomb(0, force);
            return;
        }
        case 2: {
            weapon[2][nWeapon[2]++] = new Arrow(0, force);
            return;
        }
    }
}

void HeadQuarter::MakeWarrior(int time, int *elements, int *force) {
    int tmp = order[now];
    if (elements[tmp] <= element) {
        switch (tmp) {
            case 0: {
                warriors[WarNum] = new Dragon(tmp, elements[tmp], this, force[tmp]);
                PrintWarInfo(time, tmp, elements);
                break;
            }
            case 1: {
                warriors[WarNum] = new Ninja(tmp, elements[tmp], this, force[tmp]);
                PrintWarInfo(time, tmp, elements);
                break;
            }
            case 2: {
                warriors[WarNum] = new Iceman(tmp, elements[tmp], this, force[tmp]);
                PrintWarInfo(time, tmp, elements);
                break;
            }
            case 3: {
                warriors[WarNum] = new Lion(tmp, elements[tmp], this, force[tmp]);
                PrintWarInfo(time, tmp, elements);
                ((Lion *)warriors[WarNum-1]) -> PrintInfo();
                break;
            }
            case 4: {
                warriors[WarNum] = new Wolf(tmp, elements[tmp], this, force[tmp]);
                PrintWarInfo(time, tmp, elements);
                break;
            }
            default: break;
        }
        element -= elements[tmp];
        now = (now + 1) % 5;
    }
}
void HeadQuarter::TakeHq(HeadQuarter *src, HeadQuarter *dst, int time) {
    int count = 0;
    for (int i = 0; i < src -> WarNum; i++) {
        if (src -> warriors[i] -> city == dst -> location) count++;
    }
    if (count >= 1) {
        dst -> taken = true;
        printf("%03d:10 ", time);
        PrintHqName(dst -> type);
        printf("headquarter was taken\n");
    }
}
void HeadQuarter::ReportElement(int time) {
    printf("%03d:50 %d elements in ", time, element);
    PrintHqName(type);
    printf("headquarter\n");
}

void Lion::RunAway(int time) {
    printf("%03d:05 ", time);
    PrintWarInfo(this);
    printf("ran away\n");
    if (hq -> type == 0) cities[city].red = NULL;
    else if (hq -> type == 1) cities[city].blue = NULL;
}

void Wolf::TakeWp(Warrior *enemy, int time) {
    int i, gettype;
    if (enemy -> nWeapon[0] != 0) {
        for (i = 0; i < enemy -> nWeapon[0] && nWeapon[0] < 10; i++) {
            weapon[0][nWeapon[0]++] = enemy -> weapon[0][i];
            enemy -> weapon[0][i] = NULL;
        }
        enemy -> nWeapon[0] = 0;
        gettype = 0;
    }
    else if (enemy -> nWeapon[1] != 0) {
        for (i = 0; i < enemy -> nWeapon[1] && nWeapon[1] < 10; i++) {
            weapon[1][nWeapon[1]++] = enemy -> weapon[1][i];
            enemy -> weapon[1][i] = NULL;
        }
        enemy -> nWeapon[1] = 0;
        gettype = 1;
    }
    else if (enemy -> nWeapon[2] != 0) {
        for (i = 0; i < enemy -> nWeapon[2] && nWeapon[2] < 10; i++) {
            weapon[2][nWeapon[2]++] = enemy -> weapon[2][i];
            enemy -> weapon[2][i] = NULL;
        }
        SortWp();
        enemy -> nWeapon[2] = 0;
        gettype = 2;
    }
    else return;
    printf("%03d:35 ", time);
    PrintWarInfo(this);
    printf("took %d ", i);
    PrintWpName(gettype);
    printf("from ");
    PrintWarInfo(enemy);
    printf("in city %d\n", city);
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
}
inline void PrintHqName(int t) {
    if (t) cout << "blue ";
    else cout << "red ";
}
inline void PrintWpName(int t) {
    switch (t) {
        case 0: cout << "sword "; return;
        case 1: cout << "bomb "; return;
        case 2: cout << "arrow "; return;
    }
}
inline void PrintWarInfo(Warrior *war) {
    PrintHqName(war -> hq -> type);
    PrintWarName(war -> type);
    printf("%d ", war -> id);
}
void BothAlive(Warrior *war1, Warrior *war2, int time) {
    printf("%03d:40 both ", time);
    if (war1 -> hq -> type == 0) {
        PrintWarInfo(war1);
        printf("and ");
        PrintWarInfo(war2);
    }
    else {
        PrintWarInfo(war2);
        printf("and ");
        PrintWarInfo(war1);
    }
    printf("were alive in city %d\n", war1 -> city);
}
void BothDead(Warrior *war1, Warrior *war2, int time) {
    printf("%03d:40 both ", time);
    if (war1 -> type == 0) {
        PrintWarInfo(war1);
        printf("and ");
        PrintWarInfo(war2);
    }
    else {
        PrintWarInfo(war2);
        printf("and ");
        PrintWarInfo(war1);
    }
    printf("died in city %d\n", war1 -> city);
}




int main() {
    //freopen("datapub.in.txt", "r", stdin);
    //freopen("out.txt", "w", stdout);
    int cases, elements[5], force[5], time;
    int m, n, k, t;
    int order_red[5] = {2, 3, 4, 1, 0}, order_blue[5] = {3, 0, 1, 2, 4};
    scanf("%d", &cases);
    for (int j = 1; j <= cases; j++) {
        printf("Case %d:\n", j);
        time = 0;
        scanf("%d%d%d%d", &m, &n, &k, &t);
        for (int i = 0; i < 5; i++) scanf("%d", &elements[i]);
        for (int i = 0; i < 5; i++) scanf("%d", &force[i]);
        for (int i = 0; i <= n + 1; i++) cities[i] = City(i);
        HeadQuarter red(m, order_red), blue(m, order_blue);
        red.type = 0; blue.type = 1;
        red.location = 0; blue.location = n + 1;
        while (1) {
            // time: 00 （1）武士降生
            if (time * 60 > t) break;
            red.MakeWarrior(time, elements, force);
            blue.MakeWarrior(time, elements, force);

            // time: 05 （2）lion逃跑
            if (time * 60 + 5 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (i != n + 1 && cities[i].red && cities[i].red -> type == 3) {
                    if (((Lion *)(cities[i].red)) -> loyalty <= 0)
                        ((Lion *)(cities[i].red)) -> RunAway(time);
                }
                if (i != 0 && cities[i].blue && cities[i].blue -> type == 3) {
                    if (((Lion *)(cities[i].blue)) -> loyalty <= 0)
                        ((Lion *)(cities[i].blue)) -> RunAway(time);
                }
            }

            // time: 10 （3）武士前进 （7）武士抵达敌军司令部 （8）司令部被占领
            if (time * 60 + 10 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (i >= 1 && cities[i-1].red) cities[i-1].red -> March(time, n, k);
                if (i == n + 1 && !blue.taken) red.TakeHq(&red, &blue, time);
                if (i <= n && cities[i+1].blue) cities[i+1].blue -> March(time, n, k);
                if (i == 0 && !red.taken) blue.TakeHq(&blue, &red, time);
            }
            if (red.taken || blue.taken) break;
            cities[0].red = cities[blue.location].blue = NULL;
            if (cities[0].blue2) {
                cities[0].blue = cities[0].blue2;
                cities[0].blue2 = NULL;
            }
            if (cities[blue.location].red2) {
                cities[blue.location].red = cities[blue.location].red2;
                cities[blue.location].red2 = NULL;
            }
            for (int i = 1; i <= n; i++) {
                cities[i].red = cities[i].red2;
                cities[i].blue = cities[i].blue2;
                cities[i].red2 = cities[i].blue2 = NULL;
            }

            // time: 35 （4）wolf抢武器
            if (time * 60 + 35 > t) break;
            for (int i = 1; i <= n; i++) {
                if (cities[i].red) cities[i].red -> SortWp();
                if (cities[i].blue) cities[i].blue -> SortWp();
            }
            for (int i = 1; i <= n; i++) {
                 if (cities[i].red && cities[i].blue) {
                     if (cities[i].red -> type == 4 && cities[i].blue -> type != 4)
                         ((Wolf *)cities[i].red) -> TakeWp(cities[i].blue, time);
                     else if (cities[i].red -> type != 4 && cities[i].blue -> type == 4)
                         ((Wolf *)cities[i].blue) -> TakeWp(cities[i].red, time);
                 }
            }

            // time: 40 （5）报告战斗情况 （6）武士欢呼
            if (time * 60 + 40 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].red && cities[i].blue)  {
                    if (i % 2 == 1) {
                        cities[i].red -> Attack(cities[i].blue, time);
                    }
                    else {
                        cities[i].blue -> Attack(cities[i].red, time);
                    }
                    if (cities[i].red -> type == 0 && cities[i].red -> strength > 0)
                        ((Dragon *)cities[i].red) -> Yell(time);
                    if (cities[i].blue -> type == 0 && cities[i].blue -> strength > 0)
                        ((Dragon *)cities[i].blue) -> Yell(time);
                }
            }
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].red && cities[i].red -> strength <= 0) {
                    cities[i].red = NULL;
                }
                if (cities[i].blue && cities[i].blue -> strength <= 0) {
                    cities[i].blue = NULL;
                }
            }

            // time: 50 （9）司令部报告生命元数量
            if (time * 60 + 50 > t) break;
            red.ReportElement(time);
            blue.ReportElement(time);

            // time: 55 （10）武士报告情况
            if (time * 60 + 55 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].red) cities[i].red -> ReportWp(time);
                if (cities[i].blue) cities[i].blue -> ReportWp(time);
            }
            time++;
        }
    }
    return 0;
}