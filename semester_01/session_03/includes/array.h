/* array.h */

#ifndef MY_LIBRARY_ARRAY_H
#define MY_LIBRARY_ARRAY_H

#include "array_smart.h"
#include "array_iterator.h"
#include "utilities.h"

#include <cstddef>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace seq
{
	template<typename T, 
		typename S = seq::Size_control<seq::Method::netology>>
	class Array
	{
	public:
		using value_type		= T;
		using size_control		= S;
		using size_type			= typename std::size_t;
		using difference_type	= typename std::ptrdiff_t;
		using reference			= value_type&;
		using const_reference	= const value_type&;
		using iterator			= Array_iterator<value_type>;
		using const_iterator	= Array_iterator<const value_type>;
	
	public:
		Array() = default;
		explicit Array(size_type Count, const_reference& Value = T())
			: sequence_{ Count }
		{
			std::fill(begin(), end(), Value);
		}
		Array(size_type Count, size_t Reserve)
			: sequence_{ Count, Reserve }
		{}
		Array(const_iterator First, const_iterator Last)
			: sequence_{ static_cast<size_t>(Last - First) }
		{
			std::copy(First, Last, begin());
		}
		/*****************************************************************************************/
		Array(const Array& Copied) 
			: sequence_{ Copied.size() }
		{
			std::copy(Copied.begin(), Copied.end(), begin());
		}
		Array& operator=(const Array& Copied)
		{
			if (this != &Copied) {
				Array<value_type> temp{ Copied };
				sequence_.swaps_contents(temp.sequence_);
			}
			return *this;
		}
		/*****************************************************************************************/
		Array(Array&& Movable) noexcept
			: sequence_{ std::move(Movable.sequence_) }
		{
		}
		Array& operator=(Array&& Movable) noexcept
		{
			sequence_ = std::move(Movable.sequence_);
			return *this;
		}
		/*****************************************************************************************/
		Array(const std::initializer_list<value_type>& Value_list)
			: sequence_{ Value_list.size() }
		{
			std::copy(Value_list.begin(), Value_list.end(), begin());
		}
		~Array() = default;
		/*****************************************************************************************/
		void push_back(const value_type& New)
		{
			if (sequence_.last_ == sequence_.end_) 
			{
				Array<value_type> temp(*this);
				sequence_.resize(size() + 1);
				std::copy(temp.cbegin(), temp.cend(), sequence_.first_);
				--sequence_.last_;
			}
			*(sequence_.last_) = New;
			++sequence_.last_;
		}
		void erase(size_type Index)
		{
			if (!(Index >= 0 && Index < size())) {
				throw std::out_of_range("Index of an element outside the array.");
			}
			shift_left(begin() + Index);
		}
		void erase(iterator Position)
		{
			shift_left(Position);
		}
		void erase(iterator First, iterator Last)
		{
			shift_left(First, Last - First);
		}
		/*****************************************************************************************/
		reference operator[](size_type Index) 
		{
			if (!(Index >= 0 && Index < size())) {
				throw std::out_of_range("Index of an element outside the array.");
			}
			const auto it = begin();
			return it[Index];
		}
		/*****************************************************************************************/
		iterator begin() 
		{
			return iterator(sequence_.first_);
		}
		iterator end() 
		{
			return iterator(sequence_.last_);
		}
		/* Итераторы, выполняющие перебор постоянных элементов */
		const_iterator begin() const 
		{
			return const_iterator(sequence_.first_);
		}
		const_iterator end() const 
		{
			return const_iterator(sequence_.last_);
		}
		const_iterator cbegin() const 
		{
			return const_iterator(sequence_.first_);
		}
		const_iterator cend() const 
		{
			return const_iterator(sequence_.last_);
		}
		/*****************************************************************************************/
		size_type size() const 
		{
			return cend() - cbegin();
		}
		size_type capacity() const 
		{
			return sequence_.end_ - sequence_.first_;
		}
	private:
		void shift_left(iterator Position, size_type Shift = 1)
		{
			auto first = Position;
			for (const auto last = end() - Shift; first != last; ++first) {
				std::iter_swap(first, first + Shift);
			}
			*first = value_type();
			sequence_.last_ -= Shift;
		}
	private:
		Array_smart<value_type, size_control> sequence_;
	};
}
#endif /* MY_LIBRARY_ARRAY_H */