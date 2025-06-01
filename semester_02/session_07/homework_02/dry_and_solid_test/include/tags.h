/// tags.h

#ifndef TAGS_H_IN_MY_PROJECT
#define TAGS_H_IN_MY_PROJECT

#include <utility>

namespace solid
{
	struct text_tag
	{
		decltype(auto) operator()() const
		{
			return std::make_pair("\nbeginning of formatted text\n",
				"\nend of formatted text\n");
		}
	}; /// struct text_tag

	struct html_tag
	{
		decltype(auto) operator()() const
		{
			return std::make_pair("\n<html>\t<head>\n\t\t", "\n\t</head>\n</html>\n");
		}
	}; /// struct html_tag

	struct json_tag
	{
		decltype(auto) operator()() const
		{
			return std::make_pair("\n\"address\" : {", "}\n");
		}
	}; /// struct json_tag
} /// namespace solid

#endif // !TAGS_H_IN_MY_PROJECT
