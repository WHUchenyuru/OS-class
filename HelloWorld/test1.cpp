//
//  test1.cpp
//  HelloWorld
//
//  Created by 陈羽如 on 2019/5/13.
//  Copyright © 2019 陈羽如. All rights reserved.
//

#include "test1.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;

//插入排序法生成链表
void Insert(PCB *& ready, PCB * p)
{
    PCB * pre = ready;
    while(pre->next != NULL && pre->next->priority > p->priority)
        pre = pre->next;
    p->next = pre->next;
    pre->next = p;
}

void prepare(PCB *& ready , int n){
    string names[5] = {"P1" , "P2", "P3", "P4", "P5"};
    PCB * L1;
    int pri; double time;
    
    //初始化五个进程块
    for(int i = 0; i < n; i++)
    {
        L1 = (PCB*)malloc(sizeof(PCB));
        cout<<"请输入"<<names[i]<<"的优先数：";
        cin>>pri;
        cout<<"请输入"<<names[i]<<"的要求运行时间：";
        cin>>time;
        L1->name = names[i];
        L1->priority = pri;
        L1->time = time;
        L1->state = 'R';
        Insert(ready, L1);
    }
    
}

void print(PCB *& ready, PCB *& run1)
{
    cout<<"运行进程："<<endl;
    cout<<run1->name<<'\t'<<run1->priority<<'\t'<<run1->time<<'\t'<<run1->state<<endl;
    
    if(ready->next != NULL)
    {
        PCB *t = ready->next;
        cout<<"就绪进程："<<endl;
        while(t != NULL)
        {
            cout<<t->name<<'\t'<<t->priority<<'\t'<<t->time<<'\t'<<t->state<<endl;
            t = t->next;
        }
    }
}

void operate(PCB *& ready, PCB *& run){
    run->next = ready->next;
    run->next->priority--;
    run->next->time--;
    ready->next = ready->next->next;
    
    if(run->next->time == 0)
        run->next->state = 'E';
    print(ready, run->next);
    
    if(run->next->time > 0)
        Insert(ready, run->next);
}

void test1() {
    PCB * ready = (PCB*)malloc(sizeof(PCB));    //就绪队列
    PCB *run = (PCB*)malloc(sizeof(PCB));       //运行队列
    ready->next = NULL;
    prepare(ready, 5);
    //输出每次运行结果
    int i = 1;      //标示运行次数
    while(ready->next != NULL)
    {
        cout<<"第"<<i<<"次运行："<<endl;
        operate(ready, run);
        i++;
        cout<<"--------------------------------"<<endl;
    }
}
