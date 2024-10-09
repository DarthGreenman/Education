/* my_vector_iterator.h */

#ifndef MY_LIBRARY_VECTOR_ITERATOR_H
#define MY_LIBRARY_VECTOR_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace seq
{
	template<typename T, typename A>
	class My_vector;
	
	template<typename T>
	struct My_allocator;

	template<typename T>
	class My_vector_iterator
	{
	public:
		template<typename T, typename A> 
		friend class My_vector;
		template<typename T>
		friend struct My_allocator;

		using iterator_category	= typename std::random_access_iterator_tag;
		using value_type		= T;
		using difference_type	= typename std::ptrdiff_t;
		using pointer			= T*;
		using const_pointer		= const T*;
		using reference			= T&;
		using const_reference	= const T&;
	
	private:
		explicit My_vector_iterator(pointer Iter)
			: Ptr_{ Iter } {}
	
	public:
		My_vector_iterator(const My_vector_iterator&)				= default;
		My_vector_iterator& operator=(const My_vector_iterator&)	= default;

		My_vector_iterator(My_vector_iterator&&)					= default;
		My_vector_iterator& operator=(My_vector_iterator&&)			= default;

		~My_vector_iterator() = default;
		
		reference operator*() const	{ return *Ptr_; }
		pointer operator->() const	{ return Ptr_; }
		
		My_vector_iterator& operator++()
		{
			++Ptr_;
			return *this;
		}

		My_vector_iterator operator++(int)
		{
			const auto iter{ *this };
			++(*this);
			return iter;
		}

		My_vector_iterator& operator--()
		{
			--Ptr_;
			return *this;
		}

		My_vector_iterator operator--(int)
		{
			const auto iter{ *this };
			--(*this);
			return iter;
		}

		My_vector_iterator& operator+=(const difference_type& Offset)
		{
			Ptr_ += Offset;
			return *this;
		}

		My_vector_iterator operator+(const difference_type& Offset) const
		{
			auto left{ *this };
			left += Offset;
			return left;
		}

		My_vector_iterator& operator-=(const difference_type& Offset) 
		{
			return *this += -Offset;
		}

		My_vector_iterator operator-(const difference_type& Offset) const
		{
			auto left{ *this };
			left -= Offset;
			return left;
		}

		const_reference operator[](const difference_type& Offset) const
		{
			return Ptr_[Offset];
		}
		
		reference operator[](const difference_type& Offset)
		{
			return 
				const_cast<reference>(
					static_cast<const My_vector_iterator&>(*this)[Offset]
					);
		}
		
	private:
		pointer Ptr_{};
	};

	template<typename T>
	My_vector_iterator<T> operator+(
		const typename My_vector_iterator<T>::difference_type& Offset,
		const My_vector_iterator<T>& Right
		)
	{
		return Right + Offset;
	}

	template<typename T>
	typename My_vector_iterator<T>::difference_type operator-(
		const My_vector_iterator<T>& Left, 
		const My_vector_iterator<T>& Right
		)
	{
		return &*Left - &*Right;
	}

	template<typename T>
	bool operator==(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		return &*Lhs == &*Rhs;
	}

	template<typename T>
	bool operator!=(const My_vector_iterator<T>& Lhs,
		const My_vector_iterator<T>& Rhs)
	{
		return !(Lhs == Rhs);
	}

	template<typename T>
	bool operator<(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		return &*Lhs < &*Rhs;
	}

	template<typename T>
	bool operator>(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		return &*Lhs > &*Rhs;
	}

	template<typename T>
	bool operator<=(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		return !(Lhs < Rhs);
	}

	template<typename T>
	bool operator>=(const My_vector_iterator<T>& Lhs, 
		const My_vector_iterator<T>& Rhs)
	{
		return !(Lhs > Rhs);
	}
}

#endif /* MY_LIBRARY_VECTOR_ITERATOR_H */