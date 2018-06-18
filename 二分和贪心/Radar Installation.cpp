#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

struct node {
    double s, e;
    bool operator< (node n) const { return s < n.s; }
} no[1010];
int n, i;
double d, xx, yy;

int main() {
    int cases = 0;
    while (cin >> n >> d) {
        if (n == 0) return 0;
        cout << "Case " << ++cases << ": ";
        bool valid = true;
        for (i = 0; i < n; ++i) {
            cin >> xx >> yy;
            if (fabs(yy) > d) valid = false;
            no[i].s = xx - sqrt(d * d - yy * yy);
            no[i].e = xx + sqrt(d * d - yy * yy);
        }
        if (!valid) {
            cout << -1 << endl;
            continue;
        }
        sort(no, no + n);
        double pre = no[0].e; int cnt = 1;
        for (i = 1; i < n; ++i) {
            if (no[i].e < pre) pre = no[i].e;
            if (no[i].s > pre) {
                ++cnt;
                pre = no[i].e;
            }
        }
        cout << cnt << endl;
    }
}