/* my_vector_base.h */

#ifndef MY_LIBRARY_VECTOR_BASE_H
#define MY_LIBRARY_VECTOR_BASE_H

#include <memory>

namespace seq
{
	template<typename T, typename A>
	class My_vector;

	template<typename T, typename A = std::allocator<T>>
	class My_vector_base
	{
	public:
		template<typename T, typename A>
		friend class My_vector;

		using value_type = T;
		using allocator = A;
		using size_type = typename std::size_t;
		using pointer = T*;

	private:
		My_vector_base() = default;
		explicit My_vector_base(const allocator& Alloc, size_type Number) :
			Alloc_	{ Alloc },
			// Выделяет Number*sizeof(T) байт неинициализированной памяти путем вызова
			// ::operator new(std::size_t). Затем эта функция создает в хранилище массив
			// типа T[Number] и начинается его время жизни, но не начинается время жизни
			// ни одного из его элементов. Возвращает указатель на первый элемент массива
			// из Number объектов типа T, элементы которого еще не построены.
			First_	{ Alloc_.allocate(Number) },
			Last_	{ First_ + Number },
			End_	{ First_ + Number }
		{
		}

		My_vector_base(const My_vector_base&)				= delete;
		My_vector_base& operator=(const My_vector_base&)	= delete;

		My_vector_base(My_vector_base&&)					= default;
		My_vector_base& operator=(My_vector_base&&)			= default;

		~My_vector_base() {
			// Удаляет память, на которую ссылается указатель, который должен быть указателем,
			// полученным в результате предыдущего вызова allocate(). Аргумент Number должен
			// быть равен первому аргументу вызова allocate(), который первоначально создал Ptr.
			// Вызывает ::operator delete(void*).
			Alloc_.deallocate(First_, static_cast<size_type>(End_ - First_)); 
		}

		void swap(My_vector_base& Other) noexcept
		{
			std::swap(First_,	Other.First_);
			std::swap(Last_,	Other.Last_);
			std::swap(End_,		Other.End_);
		}		

	private:
		allocator	Alloc_{};
		pointer		First_{};
		pointer		Last_{};
		pointer		End_{};
	};
}

#endif /* MY_LIBRARY_VECTOR_BASE_H */