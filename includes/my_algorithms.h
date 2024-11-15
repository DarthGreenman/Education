// my_algorithms.h

#ifndef MY_LIBRARY_ALGORITHMS_H
#define MY_LIBRARY_ALGORITHMS_H

#include "my_types.h"
#include "my_utilities.h"
#include "my_vector.h"

#include <algorithm>
#include <assert.h>
#include <concepts>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace seq
{
	template<typename Iter>
	using Value_type = typename std::iterator_traits<Iter>::value_type;

	template<typename Iter> requires Input_iterator<Iter>
	Iter find(Iter first, Iter last, const Value_type<Iter>& key)
	{
		assert(std::is_sorted(first, last) && "Array not sorted.");

		for (; first != last; ++first)
			if (*first == key)
				return first;
		return first;
	}

	template<typename Iter> requires Input_iterator<Iter>
	std::pair<Iter, Iter> find_range(Iter first, Iter last, const Value_type<Iter>& key)
	{
		assert(std::is_sorted(first, last) && "Array not sorted.");

		const auto begin = seq::find(first, last, key);
		auto current = begin;
		while (*current == key) 
			++current;

		return std::make_pair(begin, current);
	}

    template<typename Iter>
    using My_unique_vector =
        My_vector<typename std::iterator_traits<Iter>::value_type>;

	template<typename Iter>
	My_unique_vector<Iter> make_unique_vector(Iter first, Iter last)
	{
		assert(std::is_sorted(first, last) && "Array not sorted.");

		using Value_type = typename std::iterator_traits<Iter>::value_type;
		My_vector<Value_type> ret_value;
		ret_value.reserve(last - first);

		for (; first != last; ++first)
			if (const auto current = seq::find(std::next(first), last, *first);	current == last)
				ret_value.push_back(*first);
		return ret_value;
	}

    struct sort
    {
        // СОРТИРОВКА СЛИЯНИЕМ
		template<typename Iter, typename Compare = std::less<>> 
			requires Input_iterator<Iter>
		static void merge(Iter first, Iter last, Compare comp = Compare{})
		{
			if ((std::distance(first, last) <= 1) || std::is_sorted(first, last, comp))
				return;

			auto mid = std::next(first, std::distance(first, last) / 2);
			sort::merge(first, mid, comp);
			sort::merge(mid, last, comp);
			sort::merge_parts(first, mid, last, comp);
		}

        // БЫСТРАЯ СОРТИРОВКА
		template<typename Iter, typename Compare = std::less<>> 
			requires Input_iterator<Iter>
		static void quick(Iter first, Iter last, Compare comp = Compare{})
		{			
			if ((std::distance(first, last) <= 1) || std::is_sorted(first, last, comp))
				return;

			auto&& [left, right] = sort::quick_split(first, last, comp);
			sort::quick(first, left, comp);
			sort::quick(right, last, comp);
		}

        // СОРТИРОВКА ПОДСЧЕТОМ - релизациванная только сортировка "по возрастанию" 
		template<typename Iter> 
			requires Iterator_integer_types<Iter>
		static void count(Iter first, Iter last)
		{
			if ((std::distance(first, last) <= 1) || std::is_sorted(first, last, std::less<>{}))
				return;

			const auto range = std::minmax_element(first, last);
			const auto key = *range.first;
			using Value_type = typename std::iterator_traits<Iter>::value_type;
			std::vector<Value_type> counters(
				sort::count_size(*range.first, *range.second), 0
			);

			for (auto v = first; v != last; ++v)
				// Решение по приведению ключа к типу подсказанное.
				// Объясните пожалуйста: для чего необходимо выполнить такой "финт",
				// если тип элементов вектора - int.
				++counters[
					*v - static_cast<std::vector<Value_type,
						std::allocator<Value_type>>::size_type>(key)
				];

			for (Value_type n{}, v{}; v < counters.size(); ++v)
				for (std::size_t i{}; i < counters[v]; ++i)
					first[n++] = v + key;
		}

    private: // Закрытый интерфейс реализации
		template<typename Iter , typename Compare> 
			requires Input_iterator<Iter>
		static void merge_parts(Iter first, Iter sep, Iter last, Compare comp)
		{
			auto p_first = first, p_sep = sep;
			using Value_type = typename std::iterator_traits<Iter>::value_type;
			std::vector<Value_type> ret_value;
			ret_value.reserve(std::distance(first, last));

			while (p_first != sep && p_sep != last)
				comp(*p_first, *p_sep) ? ret_value.emplace_back(*p_first++) :
					ret_value.emplace_back(*p_sep++);

			for (; p_first != sep; ++p_first)
				ret_value.emplace_back(*p_first);

			for (; p_sep != last; ++p_sep)
				ret_value.emplace_back(*p_sep);

			std::move(std::begin(ret_value), std::end(ret_value), first);
		}

        // Быстрая сортировка по алгоритму Хоара
		template<typename Iter, typename Compare>
			requires Input_iterator<Iter>
		static std::pair<Iter, Iter> quick_split(Iter first, Iter last, Compare comp)
		{
			auto p_base = first, p_left = first, p_right = std::prev(last);

			// Сравниваем правое значение интервала с крайним левым (базовым):
			// если базовое значение меньше, или равно правому, то перемещаем
			// влево указатель на правый элемент.
			// если больше, то меняем значения местами и восстанавливаем указатели,
			// т. е.: теперь базовое значение - крайний правый элемент, и сравниваем
			// значение расположенные справа от базового.
			while (p_left != p_right)
			{
				if (comp(*p_base, *p_right))
					--p_right;
				else {
					std::iter_swap(p_base, p_right);
					p_base = p_right;
					++p_left;

					while (p_left != p_right && comp(*p_left, *p_base))
						++p_left;
					std::iter_swap(p_left, p_base);
					p_base = p_left;
				}
			}
			return std::make_pair(p_left, ++p_right);
		}

        // Рассчитывает размер массива счетчиков для интервала целых чисел,
		// для алгоритма сортировки ПОДСЧЕТОМ
        static std::size_t count_size(int left, int right)
		{
			const auto is_same_sign{
				left >= 0 && right >= 0 || left < 0 && right < 0
			};

			return 
				is_same_sign ? 
				1ull + (std::max)(std::abs(left), std::abs(right)) - 
				(std::min)(std::abs(left), std::abs(right)) : 
				1ull + (std::max)(std::abs(left), std::abs(right)) +
				(std::min)(std::abs(left), std::abs(right));
		}
    };
}

#endif // MY_LIBRARY_ALGORITHMS_H