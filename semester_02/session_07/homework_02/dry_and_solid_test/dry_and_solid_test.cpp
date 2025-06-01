// dry_and_solid_test.cpp
#include "include/basic_data.h"
#include "include/tags.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

namespace my_literals
{
	static std::string operator""_s(const char* str, std::size_t) 
	{ 
		return std::string{ str }; 
	}

	template<typename T, typename... Args>
	void send_to(std::ofstream& of, T tag, Args&&... args)
	{
		const solid::basic_data raw{ std::forward<Args>(args)... };
		raw.send_to(of, tag);
	}
}

int main()
{
	if (std::ofstream log("test.txt"); log.is_open())
	{
		using namespace solid;
		using namespace my_literals;

		const basic_data text{ 'A','.',' ',
			"Strings are objects that represent sequences of characters"_s, '.' };
		text.send_to(log, text_tag{});

		send_to(log, html_tag{},
			"<meta "_s,
			"property = \"og:image:alt\" "_s, "content = \"Image description\""_s,
			'>');

		send_to(log, json_tag{},
			"\"street\": \"Moskovskoe sh., 10, fl. 101\", "_s,
			"\"city\": \"Leningrad\", "_s,
			"\"postalCode\": "_s, 101101);
	}
	else { std::cout << "Could not open the file"; }

	std::cout << "\n\n";
	std::system("pause");
	return 0;
}
