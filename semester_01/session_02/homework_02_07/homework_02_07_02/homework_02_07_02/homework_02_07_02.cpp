// homework_02_07_02.cpp 

#include <iostream>

#define SUB(x, y) ((x) - (y))

int main()
{
	int a = 6;
	int b = 5;
	int c = 2;
	std::cout << SUB(a, b) << std::endl;
	std::cout << SUB(a, b) * c << std::endl;
	std::cout << SUB(a, b + c) * c << std::endl;
	
	return 0;
}