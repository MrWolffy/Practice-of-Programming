#include <iostream>
#include <istream>
using namespace std;
#define V true;
#define F false;

bool exp();
bool item();

bool item() {
    while (cin.peek() == ' ') cin.get();
    char c = cin.get();
    switch (c) {
        case '!': return !item();
        case '(': {
            bool tmp = exp();
            while (cin.peek() != ')') cin.get();
            cin.get();
            return tmp;
        }
        case 'V': return V;
        case 'F': return F;
    }
}
bool exp() {
    bool val;
    while (cin.peek() == ' ') cin.get();
    val = item();
    while (cin.peek() == ' ') cin.get();
    while (1) {
        while (cin.peek() == ' ') cin.get();
        if (cin.peek() == '|') {
            cin.get();
            val = val | item();
        }
        else if (cin.peek() == '&') {
            cin.get();
            val = val & item();
        }
        else break;
    }
    return val;
}

int main() {
    int i = 0; char c;
    //freopen("text.txt", "r", stdin);
    while (1) {
        i++;
        cout << "Expression " << i << ": ";
        if (exp()) cout << "V" << endl;
        else cout << "F" << endl;
        if (scanf("%c", &c) == EOF) break;
        cin.putback(c);
        while (cin.peek() != '\n') cin.get();
        cin.get();
    }
    return 0;
}