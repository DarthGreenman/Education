// selection_sort.h

#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <algorithm>
#include <future>
#include <iterator>
#include <type_traits>

namespace my
{
	template<typename BidirectionalIt,
		typename Compare = std::greater<typename std::iterator_traits<BidirectionalIt>::value_type>,
		typename = std::enable_if_t<std::is_same_v<Compare, std::less<typename std::iterator_traits<BidirectionalIt>::value_type>> ||
		std::is_same_v<Compare, std::greater<typename std::iterator_traits<BidirectionalIt>::value_type>>>
	>
	auto sort(BidirectionalIt first, BidirectionalIt last, Compare compare = Compare{})
	{
		if (first == std::prev(last))
			return;

		auto find_extremum = [last, compare](BidirectionalIt comparison_sample, BidirectionalIt next)
			{
				for (; next != last; ++next)
					if (compare(*comparison_sample, *next))
						comparison_sample = next;
				return comparison_sample;
			};

		std::future<BidirectionalIt> future_value = std::async(find_extremum, first, std::next(first));
		if (auto exchange_value = future_value.get(); exchange_value != last)
			std::iter_swap(first, exchange_value);

		my::sort(++first, last, compare);
	}

	template<typename T, std::size_t N, typename C = std::greater<T>,
		typename = std::enable_if_t<std::is_same_v<C, std::less<T>> || std::is_same_v<C, std::greater<T>>>
	>
	auto sort(T(&array)[N], C compare = C{})
	{
		my::sort(&array[0], &array[0] + N, compare);
	}

} // namespace my

#endif // !SELECTION_SORT_H
