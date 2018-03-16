#include <iostream>
#include <cstring>
using namespace std;

class Array2 {
    int array[3][4];
public:
    Array2() {}
    Array2(int a, int b) {}
    Array2 & operator= (Array2 a) {
        memcpy(array, a.array, sizeof(int)*12);
        return *this;
    }
    int* operator[] (int i) {
        return array[i];
    }
    int operator() (int i, int j) {
        return array[i][j];
    }
};

int main() {
    Array2 a(3,4);
    int i,j;
    for(  i = 0;i < 3; ++i )
        for(  j = 0; j < 4; j ++ )
            a[i][j] = i * 4 + j;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << a(i,j) << ",";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Array2 b;     b = a;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}