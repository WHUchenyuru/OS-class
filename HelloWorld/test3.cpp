//
//  test3.cpp
//  HelloWorld
//
//  Created by 陈羽如 on 2019/5/28.
//  Copyright © 2019 陈羽如. All rights reserved.
//

#include "test3.hpp"
#include <iostream>
using namespace std;
const int M = 20;       //空闲块总数
const int N = 3;        //每组空闲块数
const int n = 5;        //空闲块长度
int A[M][n];            //二维数组
int MA[n];              //专用块

//数组复制
void copy(int a[], int b[])
{
    for(int i = 0; i < n; i++)
    {
        a[i] = b[i];
    }
}

//初始化
void init(int A[][n])
{
    for(int i = 0; i < M; i++)
    {
        if(i==0)    //专用块
        {
            A[i][0] = N;
            for(int j = 1; j <= N; j++)
            {
                A[i][j] = i + j;
            }
        }else if((i%N == 1 && M-i > 2*N))
        {
            A[i][0] = N;
            for(int j = 1; j <= N; j++)
            {
                A[i][j] = i+N + j-1;
            }
        }else if(i%N == 1 && M-i < N)//最后一组
        {
            A[i][0] = 0;
            for(int j = 1; j <= N; j++)
            {
                A[i][j] = i;
            }
        }else{
            A[i][0] = M%N - 1;
            for(int j = 1; j <= M%N - 1;j++)
            {
                A[i][j] = i +N+j -1;
            }
        }
    }
    //将专用块内容复制到MA中
    copy(MA, A[0]);
}

//打印
void print(int A[][n])
{
    int k = 0;
    for(int i = 0; i < M-N; i++)
    {
        if(i%N == 1 || i==0)
        {
            cout<<"第"<<++k<<"组:";
            for(int j = 0; j <= N; j++)
            {
                cout<<A[i][j]<<"  ";
            }
            cout<<endl;
        }
    }
}


//分配一块
void operate(){
    MA[0]--;
    for(int i = 2; i <= N; i++)
    {
        MA[i] = MA[i+1];
    }
    int k = 0;
    if(MA[1]-3 > 0)
        k = MA[1]-3;
    copy(A[k], MA);
}
void distribute(int A[][n])
{
    if (MA[0] >1)
    {
        operate();
    }else if(MA[0] == 1)//如果MA所指的组中只剩第一块空闲块，将MA指向下一组
    {
        int k = MA[1];
        copy(MA, A[k]);
        operate();
    }
}

//归还一块
void recycle(int A[][n],int rId)
{
    if (MA[0] < N)
    {
        MA[0]++;
        MA[MA[0]] = rId;
        int k = 0;
        if(MA[1]-3 > 0)
            k = MA[1]-3;
        copy(A[k],MA);
        
    }else if(MA[0] == N)//若当前组已满
    {
        int k = 0;
        if(MA[1]-6 > 0)
            k = MA[1] - 6;
        copy(MA, A[k]);
        MA[0]++;
        MA[MA[0]] = rId;
        copy(A[k],MA);
    }
}

void test3()
{
    cout<<"初始状态："<<endl;
    init(A);
    print(A);
    while(1){
        cout<<"选择分配(D)还是回收(R):";
        char ans;
        cin>>ans;
        if(ans=='D')
        {
            cout<<"请输入分配块数：";
            int k, j = 1;
            cin>>k;
            while(k>0)
            {
                cout<<"分配"<<j++<<"块："<<endl;
                distribute(A);
                print(A);
                k--;
            }
        }else if (ans=='R')
        {
            cout<<"请输入回收块ID：";
            int k;
            cin>>k;
            recycle(A, k);
            print(A);
        }else{
            break;
        }
    }
}
