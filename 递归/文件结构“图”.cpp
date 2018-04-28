#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class filenode {
public:
    string name;
    vector<filenode> dir;
    filenode(const string &s): name(s) {}
};

struct myless {
    bool operator() (const filenode &a, const filenode &b) {
        if (!(a.name[0] == 'd' && b.name[0] == 'd')) return a.name < b.name;
        return false;
    }
};

void read(vector<filenode> &d) {
    string s;
    while (1) {
        cin >> s;
        if (s[0] == 'f') d.push_back(filenode(s));
        else if (s[0] == 'd') {
            d.push_back(filenode(s));
            read((--d.end()) -> dir);
        }
        else if (s[0] == ']') break;
        else if (s[0] == '*') break;
    }
}

void print(vector<filenode> &d, int a) {
    vector<filenode>::iterator i;
    for (i = d.begin(); i < d.end() && i -> name[0] == 'd'; ++i) ;
    stable_sort(i, d.end(), myless());
    for (i = d.begin(); i < d.end(); ++i) {
        if (i -> name[0] == 'd') {
            for (int j = 0; j < a; ++j) cout << "|     ";
            cout << i -> name << endl;
            print(i -> dir, a + 1);
        }
        else {
            for (int j = 0; j < a - 1; ++j) cout << "|     ";
            cout << i -> name << endl;
        }
    }
}

int main() {
    string s; int i = 1;
    vector<filenode> dirmap;
    while (1) {
        read(dirmap);
        cout << "DATA SET " << i++ << ":" << endl;
        cout << "ROOT" << endl;
        print(dirmap, 1);
        dirmap.clear();
        getchar();
        if (cin.peek() == '#') break;
        cout << endl;
    }
    return 0;
}