/* functors.h */

#ifndef FUNCTORS_H
#define FUNCTORS_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <concepts>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <vector>

namespace seq
{
	/* Концепт формирующий список возможных типов в шаблонах */
	template<typename T>
	concept Appropriate_type = std::same_as<T, int> || std::same_as<T, double>;

	template<typename T> 
		requires Appropriate_type<T>
	class adder_element {
	public:
		adder_element() = delete;
		template<size_t N>
		adder_element(const T(&Array)[N])
		{
			sequence_.reserve(N);
			for (size_t i{}; i < N; ++i) {
				sequence_.push_back(Array[i]);
			}
		}
		adder_element(std::initializer_list<T>& List) :
			adder_element(List.begin(), List.end())
		{}
		adder_element(const std::vector<T>& Vector) :
			adder_element(Vector.cbegin(), Vector.cend())
		{}
		adder_element(std::vector<T>&& Vector) :
			sequence_{ std::move(Vector) }
		{}
		template<typename InputIt>
		adder_element(InputIt First, InputIt Last)
		{
			sequence_.reserve(Last - First);
			while (First != Last) {
				sequence_.push_back(*First++);
			}
		}
		
		T operator()() const {
			const auto init = T();
			return std::accumulate(sequence_.cbegin(), sequence_.cend(), init);
		}
	private:
		std::vector<T> sequence_{};
	};

	class multiple_elements {
	public:
		multiple_elements() = delete;
		template<size_t N>
		multiple_elements(const int(&Array)[N])
		{
			sequence_.reserve(N);
			for (size_t i{}; i < N; ++i) {
				sequence_.push_back(Array[i]);
			}
		}
		multiple_elements(std::initializer_list<int>& List) :
			multiple_elements(List.begin(), List.end())
		{}
		multiple_elements(const std::vector<int>& Vector) :
			multiple_elements(Vector.cbegin(), Vector.cend())
		{}
		multiple_elements(std::vector<int>&& Vector) :
			sequence_{ std::move(Vector) }
		{}
		template<typename InputIt>
		multiple_elements(InputIt First, InputIt Last)
		{
			sequence_.reserve(Last - First);
			while (First != Last) {
				sequence_.push_back(*(First++));
			}
		}

		size_t operator()(unsigned Divisor) const
		{
			assert((void("Division by zero"), Divisor != 0));
			size_t count{};
			for (const auto& item : sequence_) {
				if (item != 0 && std::abs(item) % Divisor == 0) {
					++count;
				}
			}
			return count;
		}

	private:
		std::vector<int> sequence_{};
	};

}
#endif /* FUNCTORS_H */
