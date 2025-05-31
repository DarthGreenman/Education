// dry_and_solid_test.cpp
#include "include/basic_data.h"
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

std::string operator""_s(const char* str, std::size_t) { return std::string{ str }; }

int main()
{
	using namespace std;

	if (ofstream log("test.txt"); log.is_open())
	{
		using namespace solid;

		basic_data text{ 'A','.',' ',
			"Strings are objects that represent sequences of characters"_s, '.' };

		basic_data html{ "<meta "_s, "property = \"og:image:alt\" "_s,
			"content = \"Image description\""_s,	">"_s };

		basic_data json{ "\"street\": \"Moskovskoe sh., 10, fl. 101\", "_s,
			"\"city\": \"Leningrad\", "_s, "\"postalCode\": "_s, 101101 };

		void(decltype(text):: * send_to_text)(std::ostream&, text_tag) const = &decltype(text)::send_to;
		std::invoke(send_to_text, &text, log, text_tag{});

		void(decltype(html):: * send_to_html)(std::ostream&, html_tag) const = &decltype(html)::send_to;
		std::invoke(send_to_html, &html, log, html_tag{});

		void(decltype(json):: * send_to_json)(std::ostream&, json_tag) const = &decltype(json)::send_to;
		std::invoke(send_to_json, &json, log, json_tag{});
	}
	else { cout << "Could not open the file"; }

	cout << "\n\n";
	system("pause");
	return 0;
}
