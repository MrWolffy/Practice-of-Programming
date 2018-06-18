#include <iostream>
#include <algorithm>
using namespace std;

int l, n, m, d[50010];
int count(int mid) {
    int last = 0, cnt = 0;
    for (int i = 0; i <= n; ++i) {
        if (d[i] - last <= mid) ++cnt;
        else last = d[i];
    }
    return cnt;
}

int main() {
    cin >> l >> n >> m;
    for (int i = 0; i < n; ++i) cin >> d[i];
    d[n] = l;
    stable_sort(d, d + n + 1);
    int left = 0, right = l, mid;
    while (left <= right) {
        mid = left + (right - left) / 2;
        int p = count(mid);
        if (p > m) right = mid - 1;
        else left = mid + 1;
    }
    cout << left << endl;
}