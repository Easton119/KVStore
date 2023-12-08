#include<iostream>
#include "skip_list.h"
using namespace std;
int main(){
    SkipList<int,int> skipList(10);
    skipList.insertElement(5,8);
    skipList.insertElement(5,10);
    skipList.insertElement(3,9);
    skipList.displayList();
}