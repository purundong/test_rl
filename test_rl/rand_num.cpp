#include "rand_num.h"
#include <cstdlib> // 包含rand()函数
#include <ctime> 
rand_num::rand_num()
{
    srand(time(0));
}

int rand_num::get_rand(int num)
{
    return rand() % num;
}

