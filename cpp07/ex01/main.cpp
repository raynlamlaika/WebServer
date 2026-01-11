

#include <iostream>

#include  "iter.hpp"
#include <vector>




void func(int i)
{
    std::cout << i << std::endl;
}

int main()
{
    const int tors[5] = {10, 20, 30, 40, 50};


    iter<int>(tors, 5, func);
    return 0;
}
