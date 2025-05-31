/// basic_data.h

#ifndef BASIC_DATA_H_IN_MY_PROJECT
#define BASIC_DATA_H_IN_MY_PROJECT

#include <ostream>
#include <tuple>
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

	template<typename... Args>
	class basic_data
	{
	public:
		explicit basic_data(Args&& ...args) : _args{ std::make_tuple(std::forward<Args>(args) ...) } {}
		template<typename Tag>
		void send_to(std::ostream& os, Tag func) const
		{
			const auto&& [beg, end] = func();
			os << beg;
			send_to(os, std::index_sequence_for<Args...>{});
			os << end;
		}
	private:
		template<std::size_t... I>
		void send_to(std::ostream& os, std::index_sequence<I...>) const
		{
			((os << std::get<I>(_args)), ...);
		}
		std::tuple<Args...> _args{};
	}; /// class basic_data
} /// namespace solid

#endif // !BASIC_DATA_H_IN_MY_PROJECT
