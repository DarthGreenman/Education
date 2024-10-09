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
	Iter find(Iter First, Iter Last, const Value_type<Iter>& Value)
	{
		assert(std::is_sorted(First, Last) && "Array not sorted.");

		for (; First != Last; ++First) {
			if (*First == Value) {
				return First;
			}
		}
		return First;
	}

	template<typename Iter> requires Input_iterator<Iter>
	std::pair<Iter, Iter> find_range(Iter First, Iter Last,
		const Value_type<Iter>& Value)
	{
		assert(std::is_sorted(First, Last) && "Array not sorted.");

		const auto begin = seq::find(First, Last, Value);
		auto current = begin;
		while (*current == Value) {
			++current;
		}

		return std::make_pair(begin, current);
	}

    template<typename Iter>
    using My_unique_vector =
        My_vector<typename std::iterator_traits<Iter>::value_type>;

	template<typename Iter>
	My_unique_vector<Iter> make_unique_vector(Iter First, Iter Last)
	{
		assert(std::is_sorted(First, Last) && "Array not sorted.");

		using Value_type = typename std::iterator_traits<Iter>::value_type;
		My_vector<Value_type> vec;
		vec.reserve(Last - First);

		for (; First != Last; ++First) {
			if (const auto current = seq::find(std::next(First), Last, *First);
				current == Last) {
				vec.push_back(*First);
			}
		}
		return vec;
	}

    struct sort
    {
        // СОРТИРОВКА СЛИЯНИЕМ
		template<typename Iter, typename Compare = std::less<>> 
			requires Input_iterator<Iter>
		static void merge(Iter First, Iter Last, Compare Comp = Compare{})
		{
			if ((std::distance(First, Last) <= 1)
				|| std::is_sorted(First, Last, Comp)) {
				return;
			}

			auto mid = std::next(First, std::distance(First, Last) / 2);
			sort::merge(First, mid, Comp);
			sort::merge(mid, Last, Comp);
			sort::merge_parts(First, mid, Last, Comp);
		}

        // БЫСТРАЯ СОРТИРОВКА
		template<typename Iter, typename Compare = std::less<>> 
			requires Input_iterator<Iter>
		static void quick(Iter First, Iter Last, Compare Comp = Compare{})
		{			
			if ((std::distance(First, Last) <= 1) 
				|| std::is_sorted(First, Last, Comp)) {
				return;
			}

			auto&& [left, right] =
				sort::quick_split(First, Last, Comp);
			sort::quick(First, left, Comp);
			sort::quick(right, Last, Comp);
		}

        // СОРТИРОВКА ПОДСЧЕТОМ - релизациванная только сортировка "по возрастанию" 
		template<typename Iter> 
			requires Iterator_integer_types<Iter>
		static void count(Iter First, Iter Last)
		{
			if ((std::distance(First, Last) <= 1)
				|| std::is_sorted(First, Last, std::less<>{})) {
				return;
			}

			const auto range{ std::minmax_element(First, Last) };
			const auto key{ *range.first };
			using Value_type = typename std::iterator_traits<Iter>::value_type;
			std::vector<Value_type> counters(
				sort::count_size(*range.first, *range.second), 0
			);

			for (auto v = First; v != Last; ++v) {
				// Решение по приведению ключа к типу подсказанное.
				// Объясните пожалуйста: для чего необходимо выполнить такой "финт",
				// если тип элементов вектора - int.
				++counters[
					*v - static_cast<std::vector<Value_type,
						std::allocator<Value_type>>::size_type>(key)
				];
			}

			for (Value_type n{}, v{}; v < counters.size(); ++v) {
				for (std::size_t i{}; i < counters[v]; ++i) {
					First[n++] = v + key;
				}
			}
		}

    private: // Закрытый интерфейс реализации
		template<typename Iter , typename Compare> 
			requires Input_iterator<Iter>
		static void merge_parts(Iter First, Iter Sep, Iter Last, Compare Comp)
		{
			auto p_first = First, p_sep = Sep;
			using Value_type = typename std::iterator_traits<Iter>::value_type;
			std::vector<Value_type> vec;
			vec.reserve(std::distance(First, Last));

			while (p_first != Sep && p_sep != Last) {
				Comp(*p_first, *p_sep) ? vec.emplace_back(*p_first++) :
					vec.emplace_back(*p_sep++);
			}

			for (; p_first != Sep; ++p_first) {
				vec.emplace_back(*p_first);
			}

			for (; p_sep != Last; ++p_sep) {
				vec.emplace_back(*p_sep);
			}

			std::move(std::begin(vec), std::end(vec), First);
		}

        // Быстрая сортировка по алгоритму Хоара
		template<typename Iter, typename Compare>
			requires Input_iterator<Iter>
		static std::pair<Iter, Iter> quick_split(Iter First, Iter Last, Compare Comp)
		{
			auto p_base = First, p_left = First, p_right = std::prev(Last);

			// Сравниваем правое значение интервала с крайним левым (базовым):
			// если базовое значение меньше, или равно правому, то перемещаем
			// влево указатель на правый элемент.
			// если больше, то меняем значения местами и восстанавливаем указатели,
			// т. е.: теперь базовое значение - крайний правый элемент, и сравниваем
			// значение расположенные справа от базового.
			while (p_left != p_right)
			{
				if (Comp(*p_base, *p_right)) {
					--p_right;
				}
				else {
					std::iter_swap(p_base, p_right);
					p_base = p_right;
					++p_left;

					while (p_left != p_right && Comp(*p_left, *p_base)) {
						++p_left;
					}
					std::iter_swap(p_left, p_base);
					p_base = p_left;
				}
			}
			return std::make_pair(p_left, ++p_right);
		}

        // Рассчитывает размер массива счетчиков для интервала целых чисел,
		// для алгоритма сортировки ПОДСЧЕТОМ
        static std::size_t count_size(int Left, int Right)
		{
			const bool is_same_sign{
				Left >= 0 && Right >= 0 || Left < 0 && Right < 0
			};

			return 
				is_same_sign ? 
				1ull + (std::max)(std::abs(Left), std::abs(Right)) - 
				(std::min)(std::abs(Left), std::abs(Right)) : 
				1ull + (std::max)(std::abs(Left), std::abs(Right)) +
				(std::min)(std::abs(Left), std::abs(Right));
		}
    };
}

#endif // MY_LIBRARY_ALGORITHMS_H