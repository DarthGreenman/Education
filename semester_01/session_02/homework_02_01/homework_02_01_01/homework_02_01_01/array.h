// array.h

#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <initializer_list>
#include <stdexcept>

namespace seq {

	template<typename T>
	class Array {
	public:
		using Value_type = T;

		Array();
		//  ŒÕ—“–” “Œ– ŒƒÕŒÃ≈–Õ€’ Ã¿——»¬Œ¬
		explicit Array(size_t);
		//  ŒÕ—“–” “Œ– 2-’ Ã≈–Õ€’ Ã¿——»¬Œ¬
		Array(size_t, size_t);
		// ”Õ»¬≈–—¿À‹Õ€…  ŒÕ—“–” “Œ– Ã¿——»Œ¬Œ –¿«Ã≈–ÕŒ—“‹ﬁ > 1
		Array(size_t, Value_type);
		Array(std::initializer_list<Value_type> list);
		//  ŒÕ—“–” “Œ–  Œœ»–”ﬁŸ»…
		Array(const Array<Value_type>&);
		//  ŒÕ—“–” “Œ– œ≈–≈Ã≈Ÿ¿ﬁŸ»…
		Array(Array<Value_type>&&) noexcept;
		~Array();

		Value_type* begin();
		Value_type* end();
		const Value_type* cbegin() const;
		const Value_type* cend() const;
		Value_type& operator[](size_t);
		const Value_type& operator[](size_t) const;
		void push_back(const Value_type&);
		// Œœ≈–¿“Œ– œ–»—¬¿»¬¿Õ»ﬂ  Œœ»–”ﬁŸ»…
		Array<Value_type>& operator=(const Array<Value_type>&);
		// Œœ≈–¿“Œ– œ–»—¬¿»¬¿Õ»ﬂ œ≈–≈Ã≈Ÿ¿ﬁŸ»…
		Array<Value_type>& operator=(Array<Value_type>&&) noexcept;
		size_t size() const;

	private:
		void resize(size_t);

	public:
		const Value_type* data() const;
		Value_type* data();

	private:
		size_t size_;
		Value_type* data_;
	};
}

namespace seq {

	template<typename Value_type>
	Array<Value_type>::Array()
		: size_{}, data_{}
	{
	}

	template<typename Value_type>
	Array<Value_type>::Array(size_t size)
		: size_{ size }, data_{ new Value_type[size] {} }
	{
	}

	template<typename Value_type>
	Array<Value_type>::Array(size_t rows, size_t cols)
		: size_{ rows }, data_{ new Value_type[rows] {} }
	{
		for (auto p = begin(); p != cend(); ++p) {
			*p = std::move(Value_type(cols));
		}
	}

	template<typename Value_type>
	Array<Value_type>::Array(size_t size, Value_type movable)
		: size_{ size }, data_{ new Value_type[size] {} }
	{
		for (auto p = begin(); p != cend(); ++p) {
			*p = std::move(movable);
		}
	}

	template<typename Value_type>
	Array<Value_type>::Array(std::initializer_list<Value_type> list)
		: Array(static_cast<size_t>(list.size()))
	{
		std::copy(list.begin(), list.end(), begin());
	}

	//  ŒÕ—“–” “Œ–  Œœ»–Œ¬¿Õ»ﬂ
	template<typename Value_type>
	Array<Value_type>::Array(const Array<Value_type>& other)
		: size_{ other.size() }, data_{ new Value_type[other.size()]{} }
	{
		std::copy(other.cbegin(), other.cend(), begin());
	}

	//  ŒÕ—“–” “Œ– œ≈–≈Ã≈Ÿ¿ﬁŸ»…
	template<typename Value_type>
	Array<Value_type>::Array(Array<Value_type>&& movable) noexcept
		: size_{ movable.size() }, data_{ movable.data() }
	{
		movable.size_ = 0;
		movable.data_ = nullptr;
	}

	template<typename Value_type>
	Array<Value_type>::~Array() {
		if (data()) {
			delete[] data_;
			data_ = nullptr;
		}
	}
	/****************************************************************************/
	template<typename Value_type>
	Value_type* Array<Value_type>::begin() {
		return data();
	}

	template<typename Value_type>
	Value_type* Array<Value_type>::end() {
		return data() + size();
	}

	template<typename Value_type>
	const Value_type* Array<Value_type>::cbegin() const {
		return data();
	}

	template<typename Value_type>
	const Value_type* Array<Value_type>::cend() const {
		return data() + size();
	}

	template<typename Value_type>
	Value_type& Array<Value_type>::operator[](size_t index)
	{
		if (index < size()) {
			return data()[index];
		}
		throw std::out_of_range("Out of range.");
	}

	template<typename Value_type>
	const Value_type& Array<Value_type>::operator[](size_t index) const
	{
		if (index < size()) {
			return data()[index];
		}
		throw std::out_of_range("Out of range.");
	}

	template<typename Value_type>
	void Array<Value_type>::push_back(const Value_type& value)
	{
		auto backup(*this);
		resize(size() + 1);
		std::copy(backup.cbegin(), backup.cend(), begin());
		data_[size() - 1] = value;
	}

	// Œœ≈–¿“Œ– œ–»—¬¿»¬¿Õ»ﬂ  Œœ»–Œ¬¿Õ»≈Ã
	template<typename Value_type>
	Array<Value_type>& Array<Value_type>::operator=(const Array<Value_type>& other)
	{
		if (this != &other) {
			resize(other.size());
			std::copy(other.cbegin(), other.cend(), begin());
		}
		return *this;
	}

	// Œœ≈–¿“Œ– œ–»—¬¿»¬¿Õ»ﬂ œ≈–≈Ã≈Ÿ¿ﬁŸ»…
	template<typename Value_type>
	Array<Value_type>& Array<Value_type>::operator=(Array<Value_type>&& value) noexcept
	{
		if (this != &value) {
			size_ = value.size();
			data_ = value.data();

			value.size_ = 0;
			value.data_ = nullptr;
		}
		return *this;
	}

	template<typename Value_type>
	size_t Array<Value_type>::size() const {
		return size_;
	}

	template<typename Value_type>
	void Array<Value_type>::resize(size_t size)
	{
		delete[] data_;
		size_ = size;
		data_ = new Value_type[this->size()]{};
	}

	template<typename Value_type>
	const Value_type* Array<Value_type>::data() const {
		return data_;
	}
	template<typename Value_type>
	Value_type* Array<Value_type>::data() {
		return data_;
	}
}
#endif /* ARRAY_H */