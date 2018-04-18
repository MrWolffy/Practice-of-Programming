#include <iostream>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

int main() {
    int n, id, force, upper, lower;
    map<int, int> mymap;
    map<int, int>::iterator it, left, right;
    mymap.insert(make_pair(1000000000, 1));
    scanf("%d", &n);
    while (n--) {
        scanf("%d%d", &id, &force);
        it = mymap.insert(make_pair(force, id)).first;
        if (it == mymap.begin()) printf("%d %d\n", id, (++it) -> second);
        else if (it == --mymap.end()) printf("%d %d\n", id, (--it) -> second);
        else {
            right = ++it; --it; left = --it; ++it;
            upper = right -> first - force;
            lower = force - left -> first;
            if (upper >= lower) printf("%d %d\n", id, left -> second);
            else printf("%d %d\n", id, right -> second);
        }
    }
    return 0;
}