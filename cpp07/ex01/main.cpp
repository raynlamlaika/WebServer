

#include <iostream>

#include  "iter.hpp"
#include <vector>




void func(int i)
{
    std::cout << i << std::endl;
}

template <class T>
void ft_ft(T t, T t2)
{
    class T::iterator iut =  t2;
}

int main()
{
    // const std::vector<int> my; // Declares an empty vector of integers.
    // const std::vector<int> vec(5); // Declares a vector of 5 integers, initialized to 0.
    const int tors[5] = {10, 20, 30, 40, 50}; // Declares and initializes.


    iter<int>(tors, 5, func);
    return 0;
}
