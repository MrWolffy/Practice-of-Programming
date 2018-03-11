//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/8.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
class Complex {
private:
    double r,i;
public:
    void Print() {
        cout << r << "+" << i << "i" << endl;
    }
    Complex & operator= (const string &c) {
        this -> r = (double)(c[0] - '0');
        this -> i = (double)(c[2] - '0');
        return *this;
    }
};
int main() {
    Complex a;
    a = "3+4i"; a.Print();
    a = "5+6i"; a.Print();
    return 0;
}
