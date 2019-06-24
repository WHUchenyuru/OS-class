//
//  test2.cpp
//  HelloWorld
//
//  Created by 陈羽如 on 2019/5/27.
//  Copyright © 2019 陈羽如. All rights reserved.
//

#include "test2.hpp"
#include<iostream>
#include<stdio.h>
using namespace std;
#define MaxSize 50

FPT table[MaxSize];
int initialSize = 128;
int number = 0; //实际元素个数

//初始化
void init(FPT table[])
{
    table[0].pNum = ++number;
    table[0].size = 8;
    table[0].beginAdd = 24;
    table[1].pNum = ++number;
    table[1].size = 12;
    table[1].beginAdd = 128;
    table[2].pNum = ++number;
    table[2].size = 8;
    table[2].beginAdd = 248;
    table[3].pNum = ++number;
    table[3].size = 64;
    table[3].beginAdd = 276;
    table[4].pNum = ++number;
    table[4].size = 36;
    table[4].beginAdd = 400;
    
}

//显示内存布局情况
void print(FPT table[])
{
    cout<<"分区号"<<'\t'<<"大小"<<'\t'<<'\t'<<"起始地址"<<endl;
    for(int i = 0; i < number; i++)
    {
        cout<<table[i].pNum<<'\t'<<'\t'<<table[i].size<<"KB"<<'\t'<<'\t'<<table[i].beginAdd<<"KB"<<endl;
    }
}

//分配空间
bool distribute(FPT table[], int newSize)
{
    for(int i = 0; i < number; i++)
    {
        if(table[i].size > newSize)
        {
            table[i].size -= newSize;
            table[i].beginAdd += newSize;
            return true;
        }
        if(table[i].size == newSize)
        {
            for(int j = i; j < number-1; j++)
            {
                table[j] = table[j+1];
                table[j].pNum--;
            }
            number--;
            return true;
        }
    }
    return false;
}

//回收空间
void recover(FPT table[], int rAdd, int rSize)
{
    for(int i = 0; i < number; i++)
    {
        if((table[i].beginAdd < rAdd && rAdd < table[i+1].beginAdd))
        {
            //若回收区同时与插入点的前、后两个分区邻接，将三个分区合并
            if(table[i].beginAdd+table[i].size == rAdd && rAdd+rSize == table[i+1].beginAdd)
            {
                table[i].size = table[i].size + rSize + table[i+1].size;
                //将i+1以后的元素前移一个单位
                for(int j = i+1; j<number-1; j++)
                {
                    table[j] = table[j+1];
                    table[j].pNum--;
                }
                number--;
            }
            //若回收区与插入点的前一个空闲分区F1相邻接，此时应将回收区与插入点的前一分区合并
            else if(table[i].beginAdd+table[i].size == rAdd)
            {
                table[i].size += rSize;
            }
            //若回收分区与插入点的后一空闲分区F2相邻接
            else if(rAdd+rSize == table[i+1].beginAdd)
            {
                table[i+1].beginAdd = rAdd;
                table[i+1].size += rSize;
            }
            //回收区既不与F1邻接，又不与F2邻接。此时应为回收区单独建立一个新表项
            else
            {
                FPT newFPT;
                newFPT.beginAdd = rAdd;
                newFPT.pNum = i + 2;
                newFPT.size = rSize;
                for(int j = number; j > i+1; j--)
                {
                    table[j] = table[j-1];
                    table[j].pNum++;
                }
                table[i+1] = newFPT;
                number++;
            }
        }
    }
}




void test2()
{
    init(table);
    cout<<"初始分区表"<<endl;
    print(table);
    while(1){
        cout<<"分配空间(D)或者回收空间(R)?";
        char ans;
        cin>>ans;
        if(ans == 'D')
        {
            int newSize;
            cout<<"请输入分配的空间大小：";
            cin>>newSize;
            if(distribute(table, newSize))
            {
                cout<<"分配之后的空闲分区表:"<<endl;
                print(table);
            }else{
                cout<<"没有满足条件的空闲区，分配失败！";
            }
        }else if(ans == 'R')
        {
            int rAdd, rSize;
            cout<<"请输入回收分区的首地址和大小："<<endl;
            cin>>rAdd>>rSize;
            recover(table, rAdd, rSize);
            cout<<"回收后的空闲分区表："<<endl;
            print(table);
        }else
            break;
    }
}
