//
//  test2.hpp
//  HelloWorld
//
//  Created by 陈羽如 on 2019/5/27.
//  Copyright © 2019 陈羽如. All rights reserved.
//

#ifndef test2_hpp
#define test2_hpp

#include <stdio.h>
typedef struct{
    int pNum;   //分区号
    int size;   //分区大小
    int beginAdd;   //起始地址
}FPT;   //空闲分区表定义

void test2();
#endif /* test2_hpp */
