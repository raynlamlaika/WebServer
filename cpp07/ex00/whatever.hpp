#ifndef WHATEVER_HPP
# define WHATEVER_HPP

#include <iostream>


template <typename T> void swap(T &var1, T &var2)
{
    T temp;

    temp = const_cast<T&>(var1);
    const_cast<T&>(var1) = const_cast<T&>(var2);
    const_cast<T&>(var2) = temp;
}


template <typename T> T min(T var1, T var2)
{
    return (var1 > var2 ? var2 : var1);
}

template <typename T> T max(T var1, T var2)
{
    return (var1 < var2 ? var2 : var1);
}

#endif