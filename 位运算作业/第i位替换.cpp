//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/3.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
using namespace std;

int bitManipulation1(int n, int m, int i) {
    return n - ((n >> i & 1) << i) + (m & (1 << i));
}

int main() {
    int n, m, i, t;
    cin >> t;
    while (t--) {
        cin >> n >> m >> i;
        cout << bitManipulation1(n, m, i) << endl;
    }
    return 0;
}
