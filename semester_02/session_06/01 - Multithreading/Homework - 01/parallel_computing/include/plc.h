// plc.h

#ifndef PARALLEL_COMPUTING
#define PARALLEL_COMPUTING

#include "frame_iterator.h"
#include "frame.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <random>
#include <thread>
#include <type_traits>
#include <vector>


namespace plc
{
	enum class count_threads : unsigned
	{
		one = 1u,
		two = 2u,
		four = 4u,
		eight = 8u,
		sixteen = 16u,
		thirty_two = 32u
	};

	static auto make_vector(std::size_t count)
	{
		using namespace std;
		mt19937 engine;
		vector<size_t> vec{};
		generate_n(back_inserter(vec), count, ref(engine));
		return vec;
	}
	template<typename T, 
		typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	static auto make_vector(std::size_t count, T value)
	{
		return std::vector(count, value);
	}

	namespace helper
	{
		template<typename A, typename B, typename C, 
			typename = std::enable_if_t<std::is_arithmetic_v<A>&& std::is_arithmetic_v<B>>>
		static auto add(typename std::vector<A>::iterator a, typename std::vector<B>::iterator b,
			plc::frame_iterator<C>& fc)
		{
			std::for_each(std::get<0>(*fc), std::get<1>(*fc), [&a, &b](decltype(A{} + B{})& c)
				{ c = *a + *b; ++a; ++b; });
		}

		template<typename A, typename B,
			typename = std::enable_if_t<std::is_arithmetic_v<A>&& std::is_arithmetic_v<B>>>
		static auto adder(std::vector<A>&& va, std::vector<B>&& vb)
		{
			using namespace std;
			assert((size(va) == size(vb)) && "In the curriculum, vectors must be the same length.");

			using C = decltype(A{} + B{});
			vector<C> vc(size(va));
			auto a = cbegin(va), b = cbegin(vb);
			for_each(begin(vc), end(vc), [&a, &b](C& c) { c = *a + *b; ++a; ++b; });
			
			return vc;
		}
	} // namespace helper


	template<typename A, typename B,
		typename = std::enable_if_t<std::is_arithmetic_v<A>&& std::is_arithmetic_v<B>>>
	static auto adder(count_threads count, std::vector<A>&& va, std::vector<B>&& vb)
	{
		using namespace std;
		if (count == count_threads::one)
			return helper::adder(move(va), move(vb));
		assert((size(va) == size(vb)) && "In the curriculum, vectors must be the same length.");

		using C = decltype(A{} + B{});
		plc::frame fa{ static_cast<size_t>(count), va }, fb{ static_cast<size_t>(count), vb },
			fc{ static_cast<size_t>(count), vector<C>(size(va)) };

		vector<thread> tasks{};
		for (auto c = begin(fc), a = begin(fa), b = begin(fb); c != end(fc); ++c, ++a, ++b)
		{
			tasks.emplace_back(thread(helper::add<A, B, C>, /*  */
				get<0>(*a), /* ~ a.get().first_c - first_c */
				get<0>(*b), /* ~ b.get().first_c - first_c */
				ref(c)));
		}
		// Каждый поток выполняет одну и ту же задачу, но с разными данными - распараллеливание по данным
		for_each(begin(tasks), end(tasks),
			[](thread& task) { if (task.joinable()) task.join(); });

		return fc.get();
	}
} // namespace plc

#endif // !PARALLEL_COMPUTING_H