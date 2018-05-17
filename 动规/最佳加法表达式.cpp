#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;
#define inf "000000000000000000000000000000000000000000000000000001"

string str;
string ans[70][70][70];

string add(string a, string b) {
    int l1 = a.length();
    int l2 = b.length();
    string tmp;
    int maxl = 55,c = 0;
    for (int i = 0; i < maxl; i++) {
        int t;
        if (i < l1 && i < l2) t = a[i] + b[i] - 2 * '0' + c;
        else if (i < l1 && i >= l2) t = a[i] - '0' + c;
        else if (i >= l1 && i < l2) t = b[i] - '0' + c;
        else break;
        tmp.append(1, t % 10 + '0');
        c = t / 10;
    }
    while (c) {
        tmp.append(1, c % 10 + '0');
        c /= 10;
    }
    return tmp;
}

bool myless(string a, string b) {
    if (a.size() != b.size()) return a.size() < b.size();
    for (int i = a.size() - 1; i >= 0; --i) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return false;
}

string findmin(int beg, int end, int n) {
    if (ans[beg][end][n] != "") return ans[beg][end][n];
    if (n == 0) return str.substr(beg, end - beg + 1);
    if (end - beg < n) return inf;
    string min = inf;
    for (int i = beg; i < end - 1; ++i) {
        string a = str.substr(beg, i - beg + 1);
        string b = findmin(i + 1, end, n - 1);
        string tmp = add(a, b);
        if (myless(tmp, min)) min = tmp;
    }
    ans[beg][end][n] = min;
    return min;
}

int main() {
    int n;
    while (cin >> n >> str) {
        reverse(str.begin(), str.end());
        for (int i = 0; i < 70; ++i) {
            for (int j = i; j < 70; ++j) {
                for (int k = 0; k < 70; ++k) ans[i][j][k].clear();
            }
        }
        string min = findmin(0, str.size(), n);
        reverse(min.begin(), min.end());
        cout << min << endl;
    }
    return 0;
}