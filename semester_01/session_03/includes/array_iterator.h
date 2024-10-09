/* array_iterator.h */

#ifndef MY_LIBRARY_ARRAY_ITERATOR_H
#define MY_LIBRARY_ARRAY_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace seq
{
	template<typename T, typename S>
	class Array;

	template<typename T>
	class Array_iterator
	{
	public:
		template<typename T, typename S>
		friend class Array;
		using iterator_category	= typename std::random_access_iterator_tag;
		using value_type		= T;
		using difference_type	= typename std::ptrdiff_t;
		using pointer			= value_type*;
		using reference			= value_type&;
	
	private:
		Array_iterator(const pointer It)
			: ptr_{ It } {}
	
	public:
		/*****************************************************************************************/
		Array_iterator(const Array_iterator& Copied) = default;
		Array_iterator& operator=(const Array_iterator& Copied) = default;
		/*****************************************************************************************/
		Array_iterator(Array_iterator&& Movable) = default;
		Array_iterator& operator=(Array_iterator&& Movable) = default;
		/*****************************************************************************************/
		~Array_iterator() = default;
				
		reference operator*() const
		{
			return *ptr_;
		}
		pointer operator->() const
		{
			return ptr_;
		}
		/*****************************************************************************************/
		Array_iterator& operator++()
		{
			++ptr_;
			return *this;
		}
		Array_iterator operator++(int)
		{
			auto it = *this;
			++(*this);
			return it;
		}
		Array_iterator& operator--()
		{
			--ptr_;
			return *this;
		}
		Array_iterator operator--(int)
		{
			auto it = *this;
			--(*this);
			return it;
		}
		/*****************************************************************************************/
		Array_iterator& operator+=(const difference_type Offset)
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			ptr_ += Offset;
			return *this;
		}
		Array_iterator operator+(const difference_type Offset) const
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			auto it = *this;
			it += Offset;
			return it;
		}
		Array_iterator& operator-=(const difference_type Offset) 
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			return *this += -Offset;
		}
		Array_iterator operator-(const difference_type Offset) const
		{
			/* Значение n должно находиться в диапазоне представимых
			 * значений типа difference_type */
			auto it = *this;
			it -= Offset;
			return it;
		}
		/*****************************************************************************************/
		reference operator[](const difference_type Offset) const
		{
			return *(*this + Offset);
		}

	private:
		pointer ptr_;
	};

	template<typename T>
	Array_iterator<T> operator+(const typename Array_iterator<T>::difference_type Offset,
		const Array_iterator<T>& It)
	{
		/* Значение n должно находиться в диапазоне представимых
		 * значений типа difference_type */
		return It + Offset;
	}

	template<typename T>
	typename Array_iterator<T>::difference_type	operator-(const Array_iterator<T>& Left, 
		const Array_iterator<T>& Right)
	{
		/* Существует значение n типа difference_type такое, что first + n == last */
		const auto last	= Left.operator->();
		const auto first = Right.operator->();
		return std::distance(first, last);
	}
	/*****************************************************************************************/
	template<typename T>
	bool operator==(const Array_iterator<T>& Left, const Array_iterator<T>& Right)
	{
		const auto left = Left.operator->();
		const auto right = Right.operator->();
		return left == right;
	}

	template<typename T>
	bool operator!=(const Array_iterator<T>& Left, const Array_iterator<T>& Right)
	{
		return !(Left == Right);
	}

	template<typename T>
	bool operator<(const Array_iterator<T>& Left, const Array_iterator<T>& Right)
	{
		const auto left	= Left.operator->();
		const auto right = Right.operator->();
		return left < right;
	}

	template<typename T>
	bool operator>(const Array_iterator<T>& Left, const Array_iterator<T>& Right)
	{
		const auto left = Left.operator->();
		const auto right = Right.operator->();
		return left > right;
	}

	template<typename T>
	bool operator<=(const Array_iterator<T>& Left, const Array_iterator<T>& Right)
	{
		return !(Left < Right);
	}

	template<typename T>
	bool operator>=(const Array_iterator<T>& Left, const Array_iterator<T>& Right)
	{
		return !(Left > Right);
	}
}

#endif /* MY_LIBRARY_ARRAY_ITERATOR_H */