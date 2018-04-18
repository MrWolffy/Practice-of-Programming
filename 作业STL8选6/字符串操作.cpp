#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <set>
using namespace std;


string str[30];

bool isnum(string str) {
    int size = str.size();
    for (int i = 0; i < size; ++i) {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}
int findN();
string findS();
string mycopy() {
    int n = findN();
    int x = findN();
    int l = findN();
    return str[n].substr(x, l);
}
string myadd() {
    string s1 = findS();
    string s2 = findS();
    if (isnum(s1) && isnum(s2)) {
        int a1 = atoi(s1.c_str()), a2 = atoi(s2.c_str());
        if (a1 >= 0 && a1 <= 99999 && a2 >= 0 && a2 <= 99999) return to_string(a1 + a2);
    }
    return (s1 + s2);
}
int myfind() {
    string s = findS();
    int n = findN();
    return str[n].find(s);
}
int myrfind() {
    string s = findS();
    int n = findN();
    return str[n].rfind(s);
}
void myinsert() {
    string s = findS();
    int n = findN();
    int x = findN();
    str[n].insert(x, s);
}
void myreset() {
    string s = findS();
    int n = findN();
    str[n] = s;
}
void myprint() {
    int n = findN();
    printf("%s\n", str[n].c_str());
}
void myprintall(int n) {
    for (int i = 1; i <= n; ++i) {
        printf("%s\n", str[i].c_str());
    }
}
int findN() {
    string str;
    cin >> str;
    if (str == "find") return myfind();
    if (str == "rfind") return myrfind();
    return atoi(str.c_str());
}
string findS() {
    string str;
    cin >> str;
    if (str == "copy") return mycopy();
    if (str == "add") return myadd();
    return str;
}

int main() {
    string cmd;
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        cin >> str[i];
    }
    while (1) {
        cin >> cmd;
        if (cmd == "copy") mycopy();
        else if (cmd == "add") myadd();
        else if (cmd == "find") myfind();
        else if (cmd == "rfind") myrfind();
        else if (cmd == "insert") myinsert();
        else if (cmd == "reset") myreset();
        else if (cmd == "print") myprint();
        else if (cmd == "printall") myprintall(n);
        else if (cmd == "over") break;
    }
    return 0;
}