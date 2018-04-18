#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <set>
using namespace std;

map<int, int> mymap;

int main() {
    int n, x;
    string cmd;
    cin >> n;
    while (n--) {
        cin >> cmd >> x;
        auto i = mymap.find(x);
        if (cmd == "add") {
            if (i == mymap.end()) {
                mymap.insert(make_pair(x, 1));
                cout << 1 << endl;
            }
            else {
                cout << ++(i -> second) << endl;
            }
        }
        else if (cmd == "del") {
            cout << i -> second << endl;
            i -> second = 0;
        }
        else if (cmd == "ask") {
            cout << (i != mymap.end()) << ' ';
            cout << i -> second << endl;
        }
    }
    return 0;
}