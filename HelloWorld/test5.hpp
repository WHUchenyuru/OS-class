//
//  test5.hpp
//  HelloWorld
//
//  Created by 陈羽如 on 2019/5/31.
//  Copyright © 2019 陈羽如. All rights reserved.
//

#ifndef test5_hpp
#define test5_hpp

#include <stdio.h>
typedef struct Pcb{
    char name[10];      //进程名
    char state[10];    //运行状态
    char reason[10];    //若阻塞，其原因
    int breakp;         //断点保护
    struct Pcb *next;   //阻塞时的顺序
}Pcb,*link;

void init();       //初始化
void p(int s);     //P操作
void v(int s);     //V操作
void block(int s);//阻塞函数
void wakeup(int s);//唤醒函数
void control();   //处理机调度
void processor();//处理机执行
void test5();
#endif /* test5_hpp */
