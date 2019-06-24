//
//  test5.cpp
//  HelloWorld
//
//  Created by 陈羽如 on 2019/5/31.
//  Copyright © 2019 陈羽如. All rights reserved.
//

#include "test5.hpp"
#include <stdio.h>
#include <iostream>
#include <time.h>
#define BUF 3  //缓存的大小
#define MAX 20  //最大可以输入的字符

int s1,s2; //信号量
link p1;//生产者进程
link c1;//消费者进程
char str[MAX]; //输入的字符串
char buffer[BUF]; //缓冲池
int len; //输入长度
int sp=0; //string的指针
int in=0; //生产者指针
int out=0; //消费者指针
char temp; //供打印的临时产品
char rec_p[MAX];//生产记录
int rp1=0;//生产记录指针
char rec_c[MAX];//消费记录
int rp2=0;//消费记录指针
link ready; //就绪队列
link b_s1; //s1阻塞队列
link b_s2; //s2阻塞队列
int pc; //程序计数器
int count; //字符计数器
int con_cnt; //消费计数器

void init(){ //初始化
    s1=BUF;
    s2=0;
    p1=(link)malloc(sizeof(Pcb));//建立新的结点,并初始化为生产者1
    strcpy(p1->name,"Producer");
    strcpy(p1->state,"Ready");
    strcpy(p1->reason,"Null");
    p1->breakp=0;
    p1->next=NULL;
    
    c1=(link)malloc(sizeof(Pcb));//建立新的结点,并初始化为消费者1
    strcpy(c1->name,"Consumer");
    strcpy(c1->state,"Ready");
    strcpy(c1->reason,"Null");
    c1->breakp=0;
    c1->next=NULL;

    ready=p1;
    ready->next=c1;//初始化为生产进程在前，消费进程在后
    b_s1=NULL;   b_s2=NULL;//阻塞进程为NULL
    pc=0;
    con_cnt=0; //消费计数器
}

void p(int s)//P操作
{
    if(s==1){ //p(s1)
        s1--;
        if(s1<0)
            block(1); //阻塞当前生产进程
        else{
            printf("\t *s1信号申请成功!");
            ready->breakp=pc; //保存断点
        }
    }else{ //p(s2)
        s2--;
        if(s2<0)
            block(2);//阻塞当前消费进程
        else{
            printf("\t *s2信号申请成功!");
            ready->breakp=pc; //保存断点
        }
    }
}

void v(int s){
    if(s==1){ //v(s1)
        s1++;
        if(s1<=0)
            wakeup(1); //唤醒生产进程
        ready->breakp=pc; //保存断点
    }else{ //v(s2)
        s2++;
        if(s2<=0)
            wakeup(2);//唤醒消费进程
        ready->breakp=pc; //保存断点
    }
}

void block(int s){//阻塞函数的定义
    link p;
    if(s==1){//生产进程
        strcpy(p1->state,"Block");//改变状态
        strcpy(p1->reason,"S1");//说明原因
        p=b_s1;
        while(p){
            p=p->next;//p的值为NULL,表示队尾
        }
        if(!b_s1)
            b_s1=p1;
        else
            p=p1;
        p1->next=NULL;
        printf("\t *p1生产进程阻塞了!");
        ready->breakp=pc; //保存断点
        ready=ready->next;//在就绪队列中去掉,指向下一个
    }else{//消费进程
        strcpy(c1->state,"Block");
        strcpy(c1->reason,"S2");
        p=b_s2;
        while(p){
            p=p->next;//p的值为NULL,表示队尾
        }
        if(!b_s2)
            b_s2=c1;
        else
            p=c1;
        ready->breakp=pc; //保存断点
        ready=ready->next;//在就绪队列中去掉,指向下一个
        c1->next=NULL;
        printf("\t *c1消费进程阻塞了!,原因为%s", c1->reason);
    }
}

void wakeup(int s){//唤醒函数的定义
    link p;  link q=ready;
    if(s==1){ //唤醒b_s1队首进程,生产进程队列
        p=b_s1;
        b_s1=b_s1->next;//阻塞指针指向下一个阻塞进程
        strcpy(p->state,"Ready");
        strcpy(p->reason,"Null");
        while(q)//插入就绪队列
            q=q->next;
        q=p;
        p->next=NULL;
        printf("\t *p1生产进程唤醒了!");
    }else{ //唤醒b_s2队首进程,消费进程队列
        p=b_s2;
        b_s2=b_s2->next;//阻塞指针指向下一个阻塞进程
        strcpy(p->state,"Ready");
        strcpy(p->reason,"Null");
        while(q->next)//插入就绪队列
            q=q->next;
        q->next=p;
        p->next=NULL;
        printf("\t *c1消费进程唤醒了!");
    }
}

void control() //处理器调度程序
{  int rd;  int num=0;     link p=ready;
    if(ready==NULL) //若无就绪进程,结束
        return;
    while(p) //统计就绪进程个数
    {
        num++;
        p=p->next;//最终p变为NULL
    }
     printf("*就绪进程个数为:%d",num);
    time_t t;
    srand((unsigned) time(&t));
    rd=rand()%num;//随机函数产生随机数
    if(rd==1){
        p=ready;
        ready=ready->next;
        ready->next=p;
        p->next=NULL;
        strcpy(ready->state,"Run");
        strcpy(ready->next->state,"Ready");
    }else
        strcpy(ready->state,"Run");
    pc=ready->breakp;
}

void processor(){ //模拟处理器指令执行
    if(strcmp(ready->name,"Producer")==0){ //当前进程为生产者
        printf("\t *当前进程为生产者");
        switch(pc){
            case 0://produce
                printf("\t *生产者生产了字符%c",str[sp]);
                rec_p[rp1]=str[sp];//添加到生产记录
                sp=(sp+1)%len;
                pc++;
                ready->breakp=pc; //保存断点
                break;
            case 1: //p(s1)
                pc++;
                p(1);
                break;
            case 2: //put
                buffer[in]=rec_p[rp1]; //放到缓冲区
                printf("\t *%c字符成功入驻空缓存!",buffer[in]);
                rp1++;
                in=(in+1)%BUF;
                pc++;
                ready->breakp=pc; //保存断点
                break;
            case 3: //v(s2)
                pc++;
                printf("\t *释放一个s2信号");
                v(2);
                break;
            case 4://goto01
                printf("\t *生产进程goto 0 操作");
                pc=0;
                count--; //剩余字符个数减1
                printf("\t *剩余字符count=%d个",count);
                ready->breakp=pc; //保存断点
                if(count<=0){ //生产结束
                    printf("\t *生产者结束生产!");
                    strcpy(p1->state,"Stop");
                    strcpy(p1->reason,"Null");
                    ready->breakp=-1;
                    ready=ready->next;//在就绪队列中去掉
                }
        }
    }else{  //当前进程为消费者
        printf("\t *当前进程为消费者");
            switch(pc)   {
                case 0: //p(s2)
                    pc++;
                    p(2);
                    break;
                case 1: //get
                    printf("\t *消费者取字符!");
                    temp=buffer[out];
                    out=(out+1)%BUF;
                    pc++;
                    ready->breakp=pc; //保存断点
                    break;
                case 2: //v(s1)
                    pc++;
                    printf("\t *释放一个s1");
                    v(1);
                    break;
                case 3: //consume
                    printf("\t *消费了字符%c",temp);
                    rec_c[rp2]=temp;//添加到消费记录
                    rp2++;
                    con_cnt++;
                    if(con_cnt>=len){
                        strcpy(c1->state,"Stop");//完成态
                        c1->breakp=-1;
                        return;
                    }
                    pc++;
                    ready->breakp=pc; //保存断点
                    break;
                case 4: //goto0
                    printf("\t *消费进程goto 0 操作");
                    pc=0;
                    ready->breakp=pc; //保存断点
            }
    }
}

void print(){
    int i;
    printf("(1.继续 0.退出)");
    scanf("%d",&i);
    if(i==0){   exit(0);   }
}


void test5(){
    printf("*请输入字符串:");
    scanf("%s",str);    //string数组存放将要产生的字符
    len=strlen(str);
    count=len;     //输入字符的个数
    init();        //初始化
    while(con_cnt<len) //消费完所有的字符为结束
    {
        control();      //处理器调度程序
        processor();    //模拟处理器指令执行
        print();        //输出显示各个信息
    }
    printf("\n程序结束!\n");
}
