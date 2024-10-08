// Rabin-Karp algorithm.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Упрощённый алгоритм Рабина-Карпа

#include "my_hash.h"
#include "my_localisation.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace my
{
	auto find_substring(const std::string& text, const std::string& substring,
		std::pair<std::size_t, std::size_t> key)
	{
		using namespace std;
		const auto offset = substring.length();
		if (!offset)
			return vector<size_t>{};

		const auto hash_sub = my::hash::generate(begin(substring), end(substring), key);
		vector<size_t> entries{};
		for (auto first = begin(text); 
			static_cast<decltype(offset)>(distance(first, end(text))) >= offset; ++first)
			if (my::hash::generate(first, next(first, offset), key) == hash_sub) // сравнить символы?
				entries.push_back(distance(begin(text), first));
		return entries;
	}

	enum class answer : char { no = 'N', yes = 'Y' };
	auto get_correct_answer(char ch, std::string message = "Повторите ввод, нажмите Y или N:")
	{
		using namespace std;
		while (toupper(ch) != static_cast<char>(answer::yes) &&
			toupper(ch) != static_cast<char>(answer::no)) {
			cout << message;
		}
		return ch;
	}
}

int main()
{
	const my::Console_localisation<my::Code_pages::rus> local{};
	const std::string text{ 
		"English texts for beginners to practice reading and comprehension online and for free.\n"
		"Practicing your comprehension of written English will both improve your vocabulary and\n"
		"understanding of grammar and word order.\n"
		"The texts below are designed to help you develop while giving you an instant evaluation\n"
		"of your progress." };

	for (char go{ static_cast<char>(my::answer::yes) };
		toupper(go) == static_cast<char>(my::answer::yes); )
	{
		using std::cout;
		using std::cin;

		cout << '\n' << text << '\n';
		std::string substring{};
		cout << "\nВведите строку из текста: "; cin >> substring;

		if (const auto entries = my::find_substring(text, substring, std::make_pair(29, 1000)); 
			!entries.empty())
		{
			cout << "Строка " << substring << " встречается в тексте: " << entries.size() << " раз(а), позиция(и): ";
			std::for_each(std::begin(entries), std::end(entries),
				[](typename std::iterator_traits<decltype(std::begin(entries))>::value_type member)
				{
					std::cout << member << ' ';
				});
		} else 
			cout << "Строка " << substring << " не найдена.";
		cout << "\n\nДля продолжения исследования текста нажмите Y,\nдля выхода - N: "; cin >> go;
		go = my::get_correct_answer(go);
	}

	std::system("pause");
	return 0;
}