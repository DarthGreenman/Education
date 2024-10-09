/* array_smart.h */

#ifndef MY_LIBRARY_ARRAY_SMART_H
#define MY_LIBRARY_ARRAY_SMART_H

#include "test.h"
#include "utilities.h"

#include <cstddef>
#include <stdexcept>

#define NO_EXCEPTION_TEST

namespace seq
{
	template<typename T, typename S>
	class Array;

	template<typename T, 
		typename S = seq::Size_control<seq::Method::netology>>
	class Array_smart
	{
	public:
		template<typename T, typename S> 
		friend class Array;
		using value_type	= T;
		using size_control	= S;
		using size_type		= typename std::size_t;
		using pointer		= value_type*;

	private:
		Array_smart() = default;
		explicit Array_smart(size_type Count, size_type Reserve = 0)
			: Array_smart()
		{
			auto&& [count, reserve] = size_control::construct(Count);
			allocate(count, reserve);
		}
		/*****************************************************************************************/
		Array_smart(const Array_smart& Copied) = delete;
		Array_smart& operator=(const Array_smart& Copied) = delete;
		/*****************************************************************************************/
		Array_smart(Array_smart&& Movable) noexcept
			: Array_smart()
		{
			swaps_contents(Movable);
		}
		Array_smart& operator=(Array_smart&& Movable) noexcept
		{
			auto seq(std::move(Movable));
			swaps_contents(seq);
			return *this;
		}
		/*****************************************************************************************/
		~Array_smart()
		{
			release();
		}
		/*****************************************************************************************/
		void resize(size_type New_count)
		{
			auto&& [count, reserve] = size_control::scale(last_ - first_, New_count);
			release();
			allocate(count, reserve);
		}
		void swaps_contents(Array_smart& Movable) noexcept
		{
			std::swap(first_,	Movable.first_);
			std::swap(last_,	Movable.last_);
			std::swap(end_,		Movable.end_);
		}
		void allocate(size_type Count, size_t Reserve)
		{
			try {
				first_ = new value_type[Count + Reserve]{};
				#ifdef EXCEPTION_TEST
					test::test_bad_alloc();
				#endif /* EXCEPTION_TEST */
			}
		catch (const std::bad_alloc&) {
			throw;
		}
			last_	= first_ + Count;
			end_	= last_ + Reserve;
		}
		void release()
		{
			if (first_ != end_) {
				delete[] first_;
			}
		}

	private:
		pointer	first_{};	/* Указатель на первый элемент */
		pointer	last_{};	/* Указатель на элемент следующий за последним */
		pointer	end_{};		/* Указатель на адрес памяти следующий за последним выделенным */
	};
}
#endif /* MY_LIBRARY_ARRAY_SMART_H */