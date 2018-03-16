#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;
const int MAX = 110;
class CHugeInt {
    int array[210];
    int len;
public:
    CHugeInt(char *s){
        for (int i = 0; i < 210; ++i) {
            array[i] = 0;
        }
        len = strlen(s);
        for (int i = 0, j = len - 1; i < len; i++, j--) {
            array[i] = s[j] - '0';
        }
    }
    CHugeInt(int n = 0){
        for (int i = 0; i < 210; ++i) {
            array[i] = 0;
        }
        int i = 0;
        if(n == 0) array[i++] = 0;
        else {
            while(n != 0) {
                array[i++] = n % 10;
                n = n / 10;
            }
        }
        len = i;
    }
    CHugeInt(CHugeInt & a) {
        len = a.len;
        memcpy(array, a.array, sizeof(int)*210);
    }
    CHugeInt operator+ (CHugeInt & a) {
        int i = 0, j = 0, sum = 0;
        CHugeInt tmp(*this);
        int flag = 0;
        while(j < a.len && i < tmp.len) {
            sum = a.array[j] + tmp.array[i];
            if (sum >= 10) {
                tmp.array[i] = sum % 10;
                tmp.array[i+1] = sum / 10 + tmp.array[i+1];
                flag = 1;
            }
            else {
                flag = 0;
                tmp.array[i] = sum;
            }
            i++, j++;
        }
        while(j < a.len) {
            if(flag == 1) {
                tmp.array[i+1] = a.array[j];
                i++, j++;
            }
            else {
                tmp.array[i] = a.array[j];
                i++, j++;
            }
        }
        if(i >= len) {
            if(flag == 1) tmp.len = i + 1;
            else tmp.len = i;
        }
        return tmp;
    }
    CHugeInt & operator+= (int n) {
        CHugeInt tmp(n);
        *this = *this + tmp;
        return *this;
    }
    friend ostream & operator<< (ostream & os, const CHugeInt & s) {
        for(int j = s.len - 1; j >= 0; j--)
            os << s.array[j];
        return os;
    }
    friend CHugeInt operator+ (int n, CHugeInt s) {
        CHugeInt tmp(n);
        s = s + tmp;
        return s;
    }
    friend CHugeInt operator+ (CHugeInt s, int n) {
        CHugeInt tmp(n);
        s = s + tmp;
        return s;
    }
    CHugeInt & operator++() {
        *this = *this + 1;
        return *this;
    }
    CHugeInt operator++ (int n) {
        CHugeInt tmp(*this);
        *this = *this + 1;
        return tmp;
    }
};
int  main()
{
    char s[210];
    int n;

    while (cin >> s >> n) {
        CHugeInt a(s);
        CHugeInt b(n);

        cout << a + b << endl;
        cout << n + a << endl;
        cout << a + n << endl;
        b += n;
        cout  << ++ b << endl;
        cout << b++ << endl;
        cout << b << endl;
    }
    return 0;
}