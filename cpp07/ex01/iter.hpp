#ifndef ITER_HPP
# define ITER_HPP

#include <iostream>
#include <unistd.h>

template <typename T,typename Func_helper>
void iter(T *arr, const int len, Func_helper func)
{
    if (!arr || len <= 0 || !func)
    {
        std::cout << "checkout the parameters that are passed\n";
        _exit(1);
    }
    for (int i =0; i <= len ; i++)
    {
        func(arr[i]);
    }
}


template <typename T,typename Func_helper>

void iter(const T *arr, const int len, Func_helper func)
{
    if (!arr || len <= 0 || !func)
    {
        std::cout << "checkout the parameters that are passed\n";
        _exit(1);
    }
    for (int i =0; i < len ; i++)
    {
        func(arr[i]);
    }
}

#endif