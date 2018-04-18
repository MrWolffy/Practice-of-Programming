#include <iostream>
#include <string>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
using namespace std;

int main() {
    int n, x, y, a, b;
    map<int ,int> mymap;
    map<int ,int>::iterator k;
    mymap.insert(make_pair(1000000000, 1));
    scanf("%d", &n);
    while(n--) {
        scanf("%d%d", &x, &y);
        k = mymap.upper_bound(y);
        if(k == mymap.end()) k--;
        a = k -> first - y;
        b = k -> second;
        if(k != mymap.begin()) {
            k--;
            if(a > y - k -> first || (a == y - k -> first && b > k -> second))
                b = k -> second;
        }
        printf("%d %d\n",x,b);
        k = mymap.find(y);
        if(k == mymap.end() || k -> second > x) mymap[y] = x;
    }
    return 0;
}