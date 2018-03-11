//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/8.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;
class Base {
public:
    int k;
    Base(int n):k(n) { }
};
class Big
{
public:
    int v;
    Base b;
    Big(int n):v(n), b(n) {};
};
int main()
{
    int n;
    while(cin >>n) {
        Big a1(n);
        Big a2 = a1;
        cout << a1.v << "," << a1.b.k << endl;
        cout << a2.v << "," << a2.b.k << endl;
    }
}
