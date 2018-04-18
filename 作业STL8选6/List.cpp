#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <map>
#include <set>
using namespace std;


int main() {
    int count, id, num, id2;
    char cmd[10];
    list<int> * mylist[10000];
    scanf("%d", &count);
    while (count--) {
        scanf("%s", cmd);
        if (!strcmp(cmd, "new")) {
            scanf("%d", &id);
            mylist[id] = new list<int>;
        }
        else if (!strcmp(cmd, "add")) {
            scanf("%d%d", &id, &num);
            mylist[id] -> push_back(num);
        }
        else if (!strcmp(cmd, "merge")) {
            scanf("%d%d", &id, &id2);
            mylist[id] -> merge(*mylist[id2]);
        }
        else if (!strcmp(cmd, "unique")) {
            scanf("%d", &id);
            mylist[id] -> sort();
            mylist[id] -> unique();
        }
        else if (!strcmp(cmd, "out")) {
            scanf("%d", &id);
            mylist[id] -> sort();
            for (list<int>::iterator i = mylist[id] -> begin(); i != mylist[id] -> end(); i++) {
                printf("%d ", *i);
            }
            printf("\n");
        }
    }
    return 0;
}