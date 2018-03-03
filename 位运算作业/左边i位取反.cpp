//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/3.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
using namespace std;

int bitManipulation3(int n, int i) {
    return (-1 >> (32 - i) << (32 - i)) ^ n;
}

int main() {
    int t, n, i;
    cin >> t;
    while (t--) {
        cin >> n >> i;
        cout << bitManipulation3(n, i) << endl;
    }
    return 0;
}
