#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;

struct node {
    int minscore;
    int finishday;
    vector<string> ans;
};
struct project {
    string s;
    int d;
    int c;
};

int main() {
    int t;
    cin >> t;
    while (t--) {
        project project1[20];
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i) {
            cin >> project1[i].s >> project1[i].d >> project1[i].c;
        }
        node dp[(1 << 16) + 10];
        for (int i = 0; i < (1 << n) + 10; ++i) dp[i].minscore = 0xfffffff;
        dp[0].minscore = 0; dp[0].finishday = 0;
        int MAX = 1 << n;
        for (int i = 1; i < MAX; ++i) {
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) {
                    int newminscore = project1[j].c + dp[i - (1 << j)].finishday - project1[j].d;
                    newminscore = (newminscore > 0) ? newminscore : 0;
                    newminscore += dp[i - (1 << j)].minscore;
                    if (newminscore < dp[i].minscore) {
                        dp[i].minscore = newminscore;
                        dp[i].finishday = project1[j].c + dp[i - (1 << j)].finishday;
                        dp[i].ans = dp[i - (1 << j)].ans;
                        dp[i].ans.push_back(project1[j].s);
                    }
                    else if (newminscore == dp[i].minscore) {
                        vector<string> tmp;
                        tmp = dp[i - (1 << j)].ans;
                        tmp.push_back(project1[j].s);
                        int flag;
                        for (int m = 0; m < tmp.size(); ++m) {
                            flag = strcmp(dp[i].ans[m].c_str(), tmp[m].c_str());
                            if (flag) break;
                        }
                        if (flag > 0) {
                            dp[i].finishday = project1[j].c + dp[i - (1 << j)].finishday;
                            dp[i].ans = tmp;
                        }
                    }
                }
            }
        }
        cout << dp[(1 << n) - 1].minscore << endl;
        int i = (1 << n) - 1;
        for (auto j = dp[i].ans.begin(); j != dp[i].ans.end(); ++j) cout << *j << endl;
    }
    return 0;
}