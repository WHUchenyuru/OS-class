//
//  test1.hpp
//  HelloWorld
//
//  Created by 陈羽如 on 2019/5/13.
//  Copyright © 2019 陈羽如. All rights reserved.
//

#ifndef test1_hpp
#define test1_hpp

#include <iostream>
#include <stdio.h>
using namespace std;

typedef struct node{
    string name;
    node * next;
    double time;        //假设进程需要运行的单位时间数
    int priority;       //赋予进程的优先数，调度时总是选取优先数大的进程先执行
    char state;
}PCB;

void test1();

#endif /* test1_hpp */
