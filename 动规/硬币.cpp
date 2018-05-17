#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cstring>
using namespace std;

vector<int> coin;
vector<int> mustuse;

int main() {
    int n, x, t;
    cin >> n >> x;
    int tmp[20000], count[20000];
    memset(tmp, 0, sizeof(tmp));
    tmp[0] = 1;
    for (int i = 0; i < n; ++i) {
        cin >> t;
        coin.push_back(t);
        for (int k = 2 * x; k >= 0; --k) {
            tmp[k+t] = tmp[k+t] + tmp[k];
        }
    }
    for (int i = 0; i < n; ++i) {
        memset(count, 0, sizeof(count));
        count[0] = 1;
        for (int j = 1; j <= x; ++j) {
            if (j >= coin[i]) count[j] = tmp[j] - count[j-coin[i]];
            else count[j] = tmp[j];
        }
        if (count[x] == 0) mustuse.push_back(coin[i]);
    }
    sort(mustuse.begin(), mustuse.end());
    cout << mustuse.size() << endl;
    for (auto i = mustuse.begin(); i != mustuse.end(); ++i) cout << *i << " ";
}