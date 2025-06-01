/// basic_data.h

#ifndef BASIC_DATA_H_IN_MY_PROJECT
#define BASIC_DATA_H_IN_MY_PROJECT

#include <fstream>
#include <tuple>
#include <utility>

namespace solid
{
	template<typename... Args>
	class basic_data
	{
	public:
		explicit basic_data(Args&& ...args) : _args{ std::make_tuple(std::forward<Args>(args) ...) } {}
		template<typename T>
		void send_to(std::ofstream& of, T tag) const
		{
			const auto&& [beg, end] = tag();
			of << beg;
			send_to(of, std::index_sequence_for<Args...>{});
			of << end;
		}
	private:
		template<std::size_t... I>
		void send_to(std::ofstream& of, std::index_sequence<I...>) const
		{
			((of << std::get<I>(_args)), ...);
		}
		std::tuple<Args...> _args{};
	}; /// class basic_data
} /// namespace solid

#endif // !BASIC_DATA_H_IN_MY_PROJECT
