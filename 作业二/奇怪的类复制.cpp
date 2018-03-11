//
//  main.cpp
//  Cpp Program
//
//  Created by 尹晨桥 on 2018/3/8.
//  Copyright © 2018年 尹晨桥. All rights reserved.
//

#include <iostream>
using namespace std;
class Sample {
public:
    int v;
    Sample(int v_) {v = v_ ;}
    Sample() {};
    Sample(const Sample &a) {
        v = a.v + 2;
    }
};
void PrintAndDouble(Sample o)
{
    cout << o.v;
    cout << endl;
}
int main()
{
    Sample a(5);
    Sample b = a;
    PrintAndDouble(b);
    Sample c = 20;
    PrintAndDouble(c);
    Sample d;
    d = a;
    cout << d.v;
    return 0;
}
