//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/3.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
using namespace std;

int bitManipulation2(int n, int i) {
    return n - ((n >> i & 1) << i) + ((~(n >> i) & 1) << i);
}

int main() {
    int t, n, i;
    cin >> t;
    while (t--) {
        cin >> n >> i;
        cout << bitManipulation2(n, i) << endl;
    }
    return 0;
}
