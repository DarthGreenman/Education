/* my_vector.h */

#ifndef MY_LIBRARY_VECTOR_H
#define MY_LIBRARY_VECTOR_H

#include "my_vector_base.h"
#include "my_vector_iterator.h"

#include <exception>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#define _EXCEPTION_TEST

#ifdef EXCEPTION_TEST
#define MY_CALL_BAD_ALLOC throw std::bad_alloc()
#endif // EXCEPTION_TEST

namespace seq
{
	// НЕ ИСПОЛЬЗУЮТСЯ - НАЧАЛО
	// Создает массив объектов типа T и инициализирует объекты значением типа T
	// в выделенном неинициализированном хранилище, на которое указывает Ptr,
	// используя глобальное размещение - new.
	template<typename OutputIt, typename T>
	void my_uninitialized_fill(OutputIt First, OutputIt Last, const T& Value);

	// Создает массив объектов типа T и инициализирует массив элементами типа T
	// массива источника в выделенном неинициализированном хранилище, на которое
	// указывает Ptr, используя глобальное размещение - new.
	template<typename InputIt, typename OutputIt>
	void my_uninitialized_copy(InputIt First, InputIt Last, OutputIt t_First);
	// НЕ ИСПОЛЬЗУЮТСЯ - КОНЕЦ

	template<typename T, typename A>
	void swap(My_vector<T, A>& Lhs, My_vector<T, A>& Rhs) noexcept;

	template<typename T, typename A = std::allocator<T>>
	class My_vector
	{
	public:
		template<typename T, typename A>
		friend void swap(My_vector<T, A>& Lhs, My_vector<T, A>& Rhs) noexcept;

		using value_type		= T;
		using allocator			= A;
		using size_type			= typename std::size_t;
		using difference_type	= typename std::ptrdiff_t;
		using pointer			= T*;
		using const_pointer		= const T*;
		using reference			= T&;
		using const_reference	= const T&;
		using iterator			= typename My_vector_iterator<T>;
		using const_iterator	= typename My_vector_iterator<const T>;
	
	public:
		constexpr My_vector() :
			My_{}
		{
		}
		
		explicit My_vector(size_type Number, const_reference Value = value_type{},
			const allocator& Alloc = allocator{}) :
			My_{ Alloc, Number }
		{
			std::uninitialized_fill(iterator{ My_.First_ }, iterator{ My_.First_ + Number }, Value);
		}
		
		My_vector(const My_vector& Other) :
			My_{ Other.My_.Alloc_, Other.size() }
		{
			std::uninitialized_copy(Other.cbegin(), Other.cend(), iterator{My_.First_});
		}		
		
		My_vector& operator=(const My_vector& Other)
		{
			if (this == &Other) {
				return *this;
			}

			if (const size_type new_size{ Other.size() }; capacity() < new_size) {
				My_vector temp{ Other };
				seq::swap(*this,  temp);
			}
			else { // Вместимость правопреемника позволяет принять значение без переаллокации.
				if (const size_type size{ this->size() }; size < new_size) {
					// Копируем элементы в проинициализируемую область выделенной памяти правопреемника.
					const auto end_part = Other.cbegin() + size;
					std::copy(Other.cbegin(), end_part, begin());
					// Инициализируем элементы правопреемника нескопированными данными. 
					std::uninitialized_copy(end_part, Other.cend(), end());
				}
				else { // Размер правопеемника больше размера принимающего значения.
					auto current = std::copy(Other.cbegin(), Other.cend(), begin());
					// Разрушаем "лишние" данные.
					const auto end = this->end();
					for (; current != end; ++current, --My_.Last_) 
					{
						std::destroy_at(&*current);
					}
				}
			}
			return *this;
		}
		
		My_vector(My_vector&& Movable) noexcept : 
			My_vector()	{
			seq::swap(*this, Movable);
		}
		
		My_vector& operator=(My_vector&& Movable) noexcept
		{
			seq::swap(*this, Movable);
			return *this;
		}
		
		My_vector(const std::initializer_list<value_type>& List, 
			const allocator& Alloc = allocator{}) :
			My_{ Alloc, List.size() }
		{
			std::uninitialized_copy(List.begin(), List.end(), iterator{My_.First_});
		}
				
		~My_vector() {
			for (auto current = begin(); current != end(); ++current) {
				std::destroy_at(&*current);
			}
		}
		
		iterator begin()	{ return iterator{ My_.First_ }; }
		iterator end()		{ return iterator{ My_.Last_ }; }

		const_iterator begin() const	{ return const_iterator{ My_.First_ }; }
		const_iterator end() const		{ return const_iterator{ My_.Last_ }; }
		
		const_iterator cbegin() const	{ return begin(); }
		const_iterator cend() const		{ return end(); }
	
		size_type size() const		{ return static_cast<size_type>(My_.Last_ - My_.First_); }
		size_type capacity() const	{ return static_cast<size_type>(My_.End_ - My_.First_); }

		const_reference operator[](size_type Pos) const
		{
			return My_.First_[Pos];
		}

		reference operator[](size_type Pos)
		{
			return 
				const_cast<reference>(
					static_cast<const My_vector&>(*this)[Pos]
					);
		}

		const_reference at(size_type Pos) const
		{
			if (Pos >= size()) {
				throw std::out_of_range("Index of an element outside the array.");
			}
			return operator[](Pos);
		}

		reference at(size_type Pos)
		{
			return
				const_cast<reference>(
					static_cast<const My_vector&>(*this).at(Pos)
					);
		}

		void push_back(const value_type& Value)
		{
			if (capacity() == size()) {
				// Вектор заполненный - необходимо перераспределение памяти для добавления
				// нового элемента. Для предовращения "лишних" операций при инициализации
				// My_vector, создаем объект My_vector_base.
				const size_type size{ this->size() == 0 ? 1 : this->size() + 1 };
				reserve(size * 3 / 2);
			}
			std::construct_at(My_.Last_, Value);
			++My_.Last_;
		}

		void reserve(size_type Number)
		{
			// Вектор заполненный - необходимо перераспределение памяти для добавления
			// нового элемента. Для предовращения "лишних" операций при инициализации
			// My_vector, создаем объект My_vector_base.
			My_vector_base<value_type, allocator> temp(My_.Alloc_, Number);
			// Перенастраиваем размер.
			temp.Last_ = temp.First_ + this->size();
			// Копируем в temp данные из this.
			std::uninitialized_move(cbegin(), cend(), temp.First_);
			My_.swap(temp);
		}
		
	private:
		void swap(My_vector& Other) noexcept
		{
			std::swap(My_.First_,	Other.My_.First_);
			std::swap(My_.Last_,	Other.My_.Last_);
			std::swap(My_.End_,		Other.My_.End_);
		}

	private:
		My_vector_base<value_type, allocator> My_{};
	};
}
#endif /* MY_LIBRARY_VECTOR_H */

namespace seq
{
	// НЕ ИСПОЛЬЗУЮТСЯ - НАЧАЛО
	template<typename OutputIt, typename T>
	void my_uninitialized_fill(OutputIt First, OutputIt Last, const T& Value)
	{
		auto current{ First };
		try {
		#ifdef EXCEPTION_TEST
			MY_CALL_BAD_ALLOC;
		#endif /* EXCEPTION_TEST */
			for (; current != Last; ++current) {
				std::construct_at(&*current, Value);
			}
		}
		catch (const std::bad_alloc&) {
			for (; First != current; ++First) {
				std::destroy_at(&*First);
			}
			throw;
		}
	}
	
	template<typename InputIt, typename OutputIt>
	void my_uninitialized_copy(InputIt First, InputIt Last, OutputIt t_First)
	{
		auto current{ t_First };
		try {
		#ifdef EXCEPTION_TEST
			MY_CALL_BAD_ALLOC;
		#endif /* EXCEPTION_TEST */
			for (; First != Last; ++First, ++current) {
				std::construct_at(&*current, *First);
			}
		}
		catch (const std::bad_alloc&) {
			for (; t_First != current; ++t_First) {
				std::destroy_at(&*First);
			}
			throw;
		}
	}
	// НЕ ИСПОЛЬЗУЮТСЯ - КОНЕЦ

	template<typename T, typename A>
	void swap(My_vector<T, A>& Lhs, My_vector<T, A>& Rhs) noexcept {
		Lhs.swap(Rhs);
	}
}