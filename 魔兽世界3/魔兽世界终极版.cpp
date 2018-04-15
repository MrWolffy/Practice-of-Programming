//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/17.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdio>
#include <cmath>

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
inline void PrintWarInfo(Warrior *war);
Weapon * MakeWeapon(int t, int force);
bool PriorToAttack(int i, int type);
double digit(double r);



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
    Weapon(int t): type(t) {}
};
class Sword: public Weapon {
public:
    int force;
    Sword(int t, int force_): Weapon(t) {
        force = force_ * 2 / 10;
    }
};
class Bomb: public Weapon {
public:
    Bomb(int t): Weapon(t) {}
};
class Arrow: public Weapon {
public:
    int use;
    Arrow(int t): Weapon(t) {
        use = 3;
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
    bool kill;
    Weapon *wp[3];
    Warrior(int t, int s, HeadQuarter *hq_, int force_);
    void March(int time, int n);
    void Shot(int time, int r);
    void WarUseArrow();
    bool WillUseBomb(Warrior *enemy);
    void WarUseBomb(Warrior *enemy, int time);
    void Attack(Warrior *enemy, int time);
    void WarWin(Warrior *enemy, int time);
    int WarHasSword();
    void WarUseSword();
    void FightBack(Warrior *enemy, int time);
    void EarnElement(int time);
    void ReportWp(int time);
};
class Dragon: public Warrior {
public:
    double morale;
    Dragon(int t, int s, HeadQuarter *hq, int force);
    void PrintInfo() {
        printf("Its morale is %d.%02d\n", (int)morale, (int)round(digit(morale)*100));
    }
    void Yell(int time) {
        if (kill) morale += 0.2;
        else morale -= 0.2;
        if (morale > 0.8) {
            printf("%03d:40 ", time);
            PrintWarInfo(this);
            printf("yelled in city %d\n", city);
        }
    }
};
class Ninja: public Warrior {
public:
    Ninja(int t, int s, HeadQuarter *hq, int force);
};
class Iceman: public Warrior {
public:
    int step;
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
    void GetWeapon(Warrior *enemy);
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
    inline void PrintMakeWar(int time, int tmp, int *elements) {
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
    morale = ((hq -> element - s) * 1.0) / (s * 1.0);
    wp[(hq -> WarNum + 1) % 3] = MakeWeapon((hq -> WarNum + 1) % 3, force);
}
Ninja::Ninja(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {
    wp[(hq -> WarNum + 1) % 3] = MakeWeapon((hq -> WarNum + 1) % 3, force);
    wp[(hq -> WarNum + 2) % 3] = MakeWeapon((hq -> WarNum + 2) % 3, force);
}
Iceman::Iceman(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {
    wp[(hq -> WarNum + 1) % 3] = MakeWeapon((hq -> WarNum + 1) % 3, force);
    step = 0;
}
Lion::Lion(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {
    loyalty = hq -> element - s;
}
Wolf::Wolf(int t, int s, HeadQuarter *hq, int force):Warrior(t, s, hq, force) {}

Warrior::Warrior(int t, int s, HeadQuarter *hq_, int force_) {
    type = t;
    strength = s;
    id = hq_ -> WarNum + 1;
    force = force_;
    hq = hq_;
    city = hq_ -> location;
    kill = false;
    if (hq -> type == 0) cities[0].red = this;
    else cities[hq -> location].blue = this;
    wp[0] = wp[1] = wp[2] = NULL;
}
void Warrior::March(int time, int n) {
    if (strength <= 0) return;
    if (hq -> type == 0) {
        if (city == n + 1) return;
        cities[city++].red = NULL;
        cities[city].red2 = this;
        if (type == 2) {
            ((Iceman *)this) -> step ++;
            if (((Iceman *)this) -> step != 0 && ((Iceman *)this) -> step % 2 == 0) {
                if (strength <= 9) {
                    strength = 1;
                    force += 20;
                }
                else {
                    strength -= 9;
                    force += 20;
                }
            }
        }
    }
    if (hq -> type == 1) {
        if (city == 0) return;
        cities[city--].blue = NULL;
        cities[city].blue2 = this;
        if (type == 2) {
            ((Iceman *)this) -> step ++;
            if (((Iceman *)this) -> step != 0 && ((Iceman *)this) -> step % 2 == 0) {
                if (strength <= 9) {
                    strength = 1;
                    force += 20;
                }
                else {
                    strength -= 9;
                    force += 20;
                }
            }
        }
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
void Warrior::WarUseArrow() {
    ((Arrow *)wp[2]) -> use --;
    if (((Arrow *)wp[2]) -> use <= 0) wp[2] = NULL;
}
void Warrior::Shot(int time, int r) {
    if (hq -> type == 0) {
        cities[city+1].blue -> strength -= r;
        printf("%03d:35 ", time);
        PrintWarInfo(this);
        printf("shot");
        if (cities[city+1].blue -> strength <= 0) {
            printf(" and killed ");
            PrintWarInfo(cities[city+1].blue);
        }
        printf("\n");
        WarUseArrow();
    }
    if (hq -> type == 1) {
        cities[city-1].red -> strength -= r;
        printf("%03d:35 ", time);
        PrintWarInfo(this);
        printf("shot");
        if (cities[city-1].red -> strength <= 0) {
            printf(" and killed ");
            PrintWarInfo(cities[city-1].red);
        }
        printf("\n");
        WarUseArrow();
    }
}
bool Warrior::WillUseBomb(Warrior *enemy) {
    if (strength <= 0 || enemy -> strength <= 0) return false;
    if (PriorToAttack(city, enemy -> hq -> type)) {
        return strength <= (enemy -> force + enemy -> WarHasSword());
    }
    else {
        return (enemy -> strength > (force + WarHasSword()) &&
                enemy -> type != 1 &&
                strength <= (enemy -> force / 2 + enemy -> WarHasSword()));
    }
}
void Warrior::WarUseBomb(Warrior *enemy, int time) {
    printf("%03d:38 ", time);
    PrintWarInfo(this);
    printf("used a bomb and killed ");
    PrintWarInfo(enemy);
    printf("\n");
    cities[city].red -> strength = cities[city].blue -> strength = 0;
    cities[city].red = cities[city].blue = NULL;
}
int Warrior::WarHasSword() {
    int tmp = 0;
    if (wp[0]) tmp = ((Sword *)wp[0]) -> force;
    return tmp;
}
void Warrior::WarUseSword() {
    if (wp[0]) {
        ((Sword *)wp[0]) -> force = (int)(((Sword *)wp[0]) -> force * 0.8);
        if (((Sword *)wp[0]) -> force <= 0) wp[0] = NULL;
    }
}
void Warrior::WarWin(Warrior *enemy, int time) {
    if (type == 4) ((Wolf *)this) -> GetWeapon(enemy);
    EarnElement(time);
}
void Warrior::Attack(Warrior *enemy, int time) {
    printf("%03d:40 ", time);
    PrintWarInfo(this);
    printf("attacked ");
    PrintWarInfo(enemy);
    printf("in city %d with %d elements and force %d\n", city, strength, force);
    int aggforce = force + WarHasSword();
    if (enemy -> type == 3 && enemy -> strength <= aggforce) strength += enemy -> strength;
    enemy -> strength -= aggforce;
    WarUseSword();
    if (enemy -> strength <= 0) {
        printf("%03d:40 ", time);
        PrintWarInfo(enemy);
        printf("was killed in city %d\n", city);
        kill = true;
    }
}
void Warrior::FightBack(Warrior *enemy, int time) {
    if (strength <= 0 || type == 1) return;
    if (enemy -> strength <= 0) return;
    printf("%03d:40 ", time);
    PrintWarInfo(this);
    printf("fought back against ");
    PrintWarInfo(enemy);
    printf("in city %d\n", city);
    int aggforce = force / 2 + WarHasSword();
    if (enemy -> type == 3 && enemy -> strength <= aggforce) strength += enemy -> strength;
    enemy -> strength -= aggforce;
    WarUseSword();
    if (enemy -> strength <= 0) {
        printf("%03d:40 ", time);
        PrintWarInfo(enemy);
        printf("was killed in city %d\n", city);
        kill = true;
    }
}
void Warrior::EarnElement(int time) {
    printf("%03d:40 ", time);
    PrintWarInfo(this);
    printf("earned %d elements for his headquarter\n", cities[city].element);
    hq -> element += cities[city].element;
    cities[city].element = 0;
}
void Warrior::ReportWp(int time) {
    printf("%03d:55 ", time);
    PrintWarInfo(this);
    printf("has ");
    int sum = (wp[0] != NULL) + (wp[1] != NULL) + (wp[2] != NULL);
    switch (sum) {
        case 3: {
            PrintWp(wp[2]);
            printf(",");
            PrintWp(wp[1]);
            printf(",");
            PrintWp(wp[0]);
            break;
        }
        case 2: {
            if (PrintWp(wp[2])) {
                printf(",");
                PrintWp(wp[1]);
                PrintWp(wp[0]);
            }
            else {
                PrintWp(wp[1]);
                printf(",");
                PrintWp(wp[0]);
            }
            break;
        }
        case 1: {
            PrintWp(wp[2]);
            PrintWp(wp[1]);
            PrintWp(wp[0]);
            break;
        }
        case 0: {
            printf("no weapon");
            break;
        }
    }
    printf("\n");
}

void HeadQuarter::MakeWarrior(int time, int *elements, int *force) {
    int tmp = order[now];
    if (elements[tmp] <= element) {
        switch (tmp) {
            case 0: {
                warriors[WarNum] = new Dragon(tmp, elements[tmp], this, force[tmp]);
                PrintMakeWar(time, tmp, elements);
                ((Dragon *)warriors[WarNum-1]) -> PrintInfo();
                break;
            }
            case 1: {
                warriors[WarNum] = new Ninja(tmp, elements[tmp], this, force[tmp]);
                PrintMakeWar(time, tmp, elements);
                break;
            }
            case 2: {
                warriors[WarNum] = new Iceman(tmp, elements[tmp], this, force[tmp]);
                PrintMakeWar(time, tmp, elements);
                break;
            }
            case 3: {
                warriors[WarNum] = new Lion(tmp, elements[tmp], this, force[tmp]);
                PrintMakeWar(time, tmp, elements);
                ((Lion *)warriors[WarNum-1]) -> PrintInfo();
                break;
            }
            case 4: {
                warriors[WarNum] = new Wolf(tmp, elements[tmp], this, force[tmp]);
                PrintMakeWar(time, tmp, elements);
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
    if (count >= 2) {
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

void City::WarGetElement(int time) {
    if (red && blue == NULL) {
        red -> hq -> element += element;
        printf("%03d:30 ", time);
        PrintWarInfo(red);
        printf("earned %d elements for his headquarter\n", element);
        element = 0;
        return;
    }
    if (red == NULL && blue) {
        blue -> hq -> element += element;
        printf("%03d:30 ", time);
        PrintWarInfo(blue);
        printf("earned %d elements for his headquarter\n", element);
        element = 0;
        return;
    }
}
void City::RaiseFlag(int time) {
    if (blue -> strength > 0 && red -> strength > 0 ||
        blue -> strength <= 0 && red -> strength <= 0) {
        now = -1;
        count = 0;
        return;
    }
    if (blue -> strength <= 0) {
        if (now != 0) {
            now = 0;
            count = 1;
        }
        if (flag != 0) {
            if (count++ >= 2) {
                flag = 0;
                printf("%03d:40 red flag raised in city %d\n", time, id);
            }
        }
    }
    if (red -> strength <= 0) {
        if (now != 1) {
            now = 1;
            count = 1;
        }
        if (flag != 1) {
            if (count++ >= 2) {
                flag = 1;
                printf("%03d:40 blue flag raised in city %d\n", time, id);
            }
        }
    }
}

void Lion::RunAway(int time) {
    printf("%03d:05 ", time);
    PrintWarInfo(this);
    printf("ran away\n");
    if (hq -> type == 0) cities[city].red = NULL;
    else if (hq -> type == 1) cities[city].blue = NULL;
}

void Wolf::GetWeapon(Warrior *enemy) {
    if (!wp[0] && enemy -> wp[0]) wp[0] = enemy -> wp[0];
    if (!wp[1] && enemy -> wp[1]) wp[1] = enemy -> wp[1];
    if (!wp[2] && enemy -> wp[2]) wp[2] = enemy -> wp[2];
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
inline void PrintWarInfo(Warrior *war) {
    PrintHqName(war -> hq -> type);
    PrintWarName(war -> type);
    printf("%d ", war -> id);
}
Weapon * MakeWeapon(int t, int force) {
    switch (t) {
        case 0: {
            Sword *tmp = new Sword(0, force);
            if (tmp -> force <= 0) tmp = NULL;
            return (Weapon *)tmp;
        }
        case 1: return new Bomb(1);
        case 2: return new Arrow(2);
    }
    return NULL;
}
bool PriorToAttack(int i, int type) {
    if (type == 0) return (cities[i].flag == 0 || (cities[i].flag == -1 && i % 2 == 1));
    if (type == 1) return (cities[i].flag == 1 || (cities[i].flag == -1 && i % 2 == 0));
    return false;
}
double digit(double r) {
    double r1 = round(r);
    if (r > r1) return (r - r1);
    else if (r == r1) return 0;
    else return (r - r1 + 1);
}

int main() {
    //freopen("Warcraft.in.txt", "r", stdin);
    //freopen("out.txt", "w", stdout);
    int cases, elements[5], force[5], time;
    int m, n, r, k, t;
    int order_red[5] = {2, 3, 4, 1, 0}, order_blue[5] = {3, 0, 1, 2, 4};
    scanf("%d", &cases);
    for (int j = 1; j <= cases; j++) {
        printf("Case %d:\n", j);
        time = 0;
        scanf("%d%d%d%d%d", &m, &n, &r, &k, &t);
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

            // time: 10 （3）武士前进 （12）武士抵达敌军司令部 （13）司令部被占领
            if (time * 60 + 10 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (i >= 1 && cities[i-1].red) cities[i-1].red -> March(time, n);
                if (i == n + 1 && !blue.taken) red.TakeHq(&red, &blue, time);
                if (i <= n && cities[i+1].blue) cities[i+1].blue -> March(time, n);
                if (i == 0 && !red.taken) blue.TakeHq(&blue, &red, time);
            }
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
            if (red.taken || blue.taken) break;

            // time: 20 城市生产生命值
            if (time * 60 + 20 > t) break;
            for (int i = 1; i <= n; i++) cities[i].element += 10;

            // time: 30 武士从城市获取生命值
            if (time * 60 + 30 > t) break;
            for (int i = 1; i <= n; i++) cities[i].WarGetElement(time);

            // time: 35 （4）武士放箭
            if (time * 60 + 35 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (i != n + 1 && cities[i].red && cities[i].red -> wp[2] && cities[i+1].blue)
                    cities[i].red -> Shot(time, r);
                if (i != 0 && cities[i].blue && cities[i].blue -> wp[2] && cities[i-1].red)
                    cities[i].blue -> Shot(time, r);
            }

            // time: 38 （5）武士使用bomb
            if (time * 60 + 38 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].red && cities[i].blue) {
                    if (cities[i].red -> wp[1] && cities[i].red -> WillUseBomb(cities[i].blue)) {
                        cities[i].red -> WarUseBomb(cities[i].blue, time);
                    }
                    else if (cities[i].blue -> wp[1] && cities[i].blue -> WillUseBomb(cities[i].red)) {
                        cities[i].blue -> WarUseBomb(cities[i].red, time);
                    }
                }
            }

            // time: 40 （6）武士主动进攻 （7）武士反击 （8）武士战死 （9）武士欢呼 （10）武士获取生命元
            if (time * 60 + 40 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].red && cities[i].blue)  {
                    if (cities[i].red -> strength <= 0 && cities[i].blue -> strength <= 0) continue;
                    if (cities[i].red -> strength > 0 && cities[i].blue -> strength <= 0) {
                        cities[i].red -> kill = true;
                        if (cities[i].red -> type == 4)
                            ((Wolf *)cities[i].red) -> GetWeapon(cities[i].blue);
                        if (cities[i].red -> type == 0 && cities[i].red -> strength > 0 && PriorToAttack(i, 0))
                            ((Dragon *)cities[i].red) -> Yell(time);
                        cities[i].red -> EarnElement(time);
                    }
                    else if (cities[i].blue -> strength > 0 && cities[i].red -> strength <= 0){
                        cities[i].blue -> kill = true;
                        if (cities[i].blue -> type == 4)
                            ((Wolf *)cities[i].blue) -> GetWeapon(cities[i].red);
                        if (cities[i].blue -> type == 0 && cities[i].blue -> strength > 0 && PriorToAttack(i, 1))
                            ((Dragon *)cities[i].blue) -> Yell(time);
                        cities[i].blue -> EarnElement(time);
                    }
                    else if (PriorToAttack(i, 0)) {
                        cities[i].red -> Attack(cities[i].blue, time);
                        cities[i].blue -> FightBack(cities[i].red, time);
                        if (cities[i].red -> type == 0 && cities[i].red -> strength > 0)
                            ((Dragon *)cities[i].red) -> Yell(time);
                        if (cities[i].red -> strength > 0 && cities[i].blue -> strength <= 0)
                            cities[i].red -> WarWin(cities[i].blue, time);
                        if (cities[i].blue -> strength > 0 && cities[i].red -> strength <= 0)
                            cities[i].blue -> WarWin(cities[i].red, time);
                    }
                    else {
                        cities[i].blue -> Attack(cities[i].red, time);
                        cities[i].red -> FightBack(cities[i].blue, time);
                        if (cities[i].blue -> type == 0 && cities[i].blue -> strength > 0)
                            ((Dragon *)cities[i].blue) -> Yell(time);
                        if (cities[i].blue -> strength > 0 && cities[i].red -> strength <= 0)
                            cities[i].blue -> WarWin(cities[i].red, time);
                        if (cities[i].red -> strength > 0 && cities[i].blue -> strength <= 0)
                            cities[i].red -> WarWin(cities[i].blue, time);
                    }
                    // time: 40 （11）旗帜升起
                    cities[i].RaiseFlag(time);
                    // Lion忠诚度降低
                    if (!cities[i].red -> kill && !cities[i].blue -> kill) {
                        if (cities[i].red -> type == 3)
                            ((Lion *)cities[i].red) -> loyalty -= k;
                        if (cities[i].blue -> type == 3)
                            ((Lion *)cities[i].blue) -> loyalty -= k;
                    }
                }
            }
            // 为死掉的武士收尸
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].red && cities[i].red -> strength <= 0) {
                    cities[i].red = NULL;
                }
                if (cities[i].blue && cities[i].blue -> strength <= 0) {
                    cities[i].blue = NULL;
                }
            }
            // time: 40 司令部奖励武士生命值
            for (int i = n + 1; i >= 0; i--) {
                if (cities[i].red && cities[i].red -> kill) {
                    if (red.element >= 8) {
                        cities[i].red -> strength += 8;
                        red.element -= 8;
                    }
                    cities[i].red -> kill = false;
                }
            }
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].blue && cities[i].blue -> kill) {
                    if (blue.element >= 8) {
                        cities[i].blue -> strength += 8;
                        blue.element -= 8;
                    }
                    cities[i].blue -> kill = false;
                }
            }

            // time: 50 （14）司令部报告生命元数量
            if (time * 60 + 50 > t) break;
            red.ReportElement(time);
            blue.ReportElement(time);

            // time: 55 （15）武士报告武器情况
            if (time * 60 + 55 > t) break;
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].red) cities[i].red -> ReportWp(time);
            }
            for (int i = 0; i <= n + 1; i++) {
                if (cities[i].blue) cities[i].blue -> ReportWp(time);
            }
            time++;
        }
    }
    return 0;
}