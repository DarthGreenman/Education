// homework_04_08_03.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается
// выполнение программы.

#include "lib/my_memory.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

struct A {
	int X{}, Y{};
};

int main()
{
	using std::cout;
	
	seq::My_unique_ptr<int> up{new int{10}};
	up.reset(new int{ 100 });
	auto u{ *up };
	u = 100;
	
	cout << "String:";
	seq::My_unique_ptr<std::string_view> u_str{
		new std::string_view{"includes/my_memory.h"}
	};
	auto str = u_str.get();
	cout << "\nPath = " << *str;
	u_str.reset(new std::string_view{ "includes/my_allocator.h" });
	str = u_str.get();
	cout << "\nPath = " << *str;

	cout << "\n\nStruct:";
	seq::My_unique_ptr<A> a{ new A{2, 25} };
	auto&& [x, y] = *a;
	cout << "\nx = " << x << "\ny = " << y;

	cout << "\n\nVisitor:";
	using Person = typename std::pair<std::string_view, int>;
	seq::My_unique_ptr<Person> person{ new Person{"Alex", 25} };
	cout << "\nName:\t" << person->first << "\nAge:\t" << person->second;

	cout << "\n\nEmployee:";
	person.reset(new Person{ "Mike", 48 });
	cout << "\nName:\t" << person->first << "\nAge:\t" << person->second;

	cout << "\n\n";
	std::system("pause");
	return 0;
}