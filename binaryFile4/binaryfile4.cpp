#include <array>
#include <iostream>

void print(int array[], size_t size) 
{
    for (size_t i = 0; i < size; i++) 
    {
        std::cout << array[i] << ' ';
    }
    
    std::cout << '\n';
}

int main() 
{
    std::array<int, 5> values{1, 2, 3, 4, 5};
    print(values.data(), values.size());

    return 0;
}
