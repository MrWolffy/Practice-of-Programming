#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

bool isprime(int a) {
    int sq = int(sqrt(a));
    bool flag = true;
    if (a % 2 == 0) return false;
    for (int i = 3; i < sq; i = i + 2) {
        if (a % i == 0) {
            flag = false;
            break;
        }
    }
    return flag;
}

int countprime(int a) {
    int count = 0, b = a;
    if (b % 2 == 0) count++;
    while (b % 2 == 0) b = b / 2;
    for (int i = 3; i < a; i = i + 2) {
        if (b % i == 0 && isprime(i)) {
            count++;
            while(b % i == 0) b = b / i;
            if (b == 1) break;
        }
    }
    return count;
}

struct myless {
    bool operator() (pair<int, int> a, pair<int, int> b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first > b.first;
    }
};

int main() {
    vector<pair<int, int> > pq;
    int num, n;
    scanf("%d", &num);
    while (num--) {
        for (int i = 0; i < 10; ++i) {
            scanf("%d", &n);
            pq.push_back(make_pair(n, countprime(n)));
        }
        stable_sort(pq.begin(), pq.end(), myless());
        printf("%d %d\n", pq.begin() -> first, (--pq.end()) -> first);
        pq.erase(pq.begin()); pq.erase(--pq.end());
    }
    return 0;
}