/* table.h */

#ifndef TABLE_H
#define TABLE_H

#include <new>
#include <utility>
#include <memory>

namespace seq
{
	template<typename T>
	class Matrix
	{
	public:
		Matrix() = delete;
		Matrix(size_t Row, size_t Col) try :
			offset_{ Row }, number_{ Col }, sequence_{ std::make_unique<T[]>(Row * Col) }
		{
		}
		catch (const std::bad_alloc& e) {
			throw e;
		}

		Matrix(const Matrix<T>& Copied) = delete;
		Matrix& operator=(const Matrix<T>& Copied) = delete;
		Matrix(Matrix<T>&& Movable) noexcept = delete;
		Matrix& operator=(Matrix<T>&& Movable) noexcept = delete;
		
		~Matrix() = default;

		T* operator[](size_t Offset) const 
		{
			/* Ѕез контрол€ выхода за границы диапазона индекса */
			const auto index = Offset * number_;
			const auto first = sequence_.get();
			return first + index;
		}

		T* operator[](size_t Offset)
		{
			/* Ѕез контрол€ выхода за границы диапазона индекса */
			const auto index = Offset * number_;
			const auto first = sequence_.get();
			return first + index;
		}

		std::pair<size_t, size_t> size() const {
			return
				std::make_pair(offset_, number_);
		}

	private:
		size_t offset_{};
		size_t number_{};
		std::unique_ptr<T[]> sequence_{};
	};
}
#endif /* TABLE_H */