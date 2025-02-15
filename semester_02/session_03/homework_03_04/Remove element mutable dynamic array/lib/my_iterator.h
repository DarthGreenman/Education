// my_iterator.h

#ifndef MY_ITERATOR_H
#define MY_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace my
{
	template<typename T> class My_dynamic_array;

	template<typename T>
	class My_iterator
	{
	public:
		template<typename T> friend class My_dynamic_array;

		using iterator_category	= typename std::random_access_iterator_tag;
		using value_type		= T;
		using difference_type	= typename std::ptrdiff_t;
		using pointer			= T*;
		using const_pointer		= const T*;
		using reference			= T&;
		using const_reference	= const T&;
	
	private:
		explicit My_iterator(pointer iter)
			: current_{ iter } {}
	
	public:
		My_iterator(const My_iterator&) = default;
		My_iterator& operator=(const My_iterator&) = default;

		My_iterator(My_iterator&&) = default;
		My_iterator& operator=(My_iterator&&) = default;

		~My_iterator() = default;
		
		reference operator*() const	{ return *current_; }
		pointer operator->() const	{ return current_; }
		
		auto& operator++() 
		{
			++current_;
			return *this;
		}

		auto operator++(int) 
		{
			auto tmp = *this;
			++(*this);
			return tmp;
		}

		auto& operator--() 
		{
			--current_;
			return *this;
		}

		auto operator--(int) 
		{
			auto tmp = *this;
			--(*this);
			return tmp;
		}

		auto& operator+=(const difference_type& offset) 
		{
			current_ += offset;
			return *this;
		}

		auto operator+(const difference_type& offset) const 
		{
			auto tmp =*this;
			tmp += offset;
			return tmp;
		}

		auto& operator-=(const difference_type& offset) 
		{
			return *this += -offset;
		}

		auto operator-(const difference_type& offset) const 
		{
			auto tmp = *this;
			tmp -= offset;
			return tmp;
		}

		const_reference operator[](const difference_type& offset) const 
		{
			return current_[offset];
		}
		
		reference operator[](const difference_type& offset) 
		{
			return const_cast<reference>(static_cast<const My_iterator&>(*this)[offset]);
		}
		
	private:
		pointer current_{};
	};

	
	template<typename T>
	My_iterator<T> operator+(const typename My_iterator<T>::difference_type& offset,
		const My_iterator<T>& right)
	{
		return right + offset;
	}

	template<typename T>
	typename My_iterator<T>::difference_type operator-(const My_iterator<T>& left, 
		const My_iterator<T>& right)
	{
		return &*left - &*right;
	}

	template<typename T>
	bool operator==(const My_iterator<T>& lhs, const My_iterator<T>& rhs)
	{
		return &*lhs == &*rhs;
	}

	template<typename T>
	bool operator!=(const My_iterator<T>& lhs, const My_iterator<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T>
	bool operator<(const My_iterator<T>& lhs, const My_iterator<T>& rhs)
	{
		return &*lhs < &*rhs;
	}

	template<typename T>
	bool operator>(const My_iterator<T>& lhs, const My_iterator<T>& rhs)
	{
		return &*lhs > &*rhs;
	}

	template<typename T>
	bool operator<=(const My_iterator<T>& lhs, const My_iterator<T>& rhs)
	{
		return !(lhs < rhs);
	}

	template<typename T>
	bool operator>=(const My_iterator<T>& lhs, const My_iterator<T>& rhs)
	{
		return !(lhs > rhs);
	}
}

#endif // MY_ITERATOR_H