// homework_01_05_01.cpp

#include <iostream>
#include <ctime>

int main()
{
    const size_t kSize{ 10 };
    int array[kSize] = { 0 };

    srand(static_cast<unsigned int>(time(nullptr)));
    for (auto& x : array) {
        x = rand();
    }

    for (size_t i{ 0 }; i < kSize; ++i) {
        std::cout << array[i] << (i != kSize - 1 ? ", " : ".");
    }
    return 0;
}