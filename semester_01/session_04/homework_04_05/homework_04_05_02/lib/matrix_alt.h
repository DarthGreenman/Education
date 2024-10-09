/* matrix.h */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdexcept>
#include <utility>

namespace alternative
{
	template<typename T, size_t Row, size_t Col>
	class Matrix
	{
	public:
		Matrix() = default;
		
		Matrix(const Matrix<T, Row, Col>& Copied) = delete;
		Matrix& operator=(const Matrix<T, Row, Col>& Copied) = delete;
		Matrix(Matrix<T, Row, Col>&& Movable) noexcept = delete;
		Matrix& operator=(Matrix<T, Row, Col>&& Movable) noexcept = delete;
		
		~Matrix() = default;

		T& operator()(size_t Row, size_t Col) const
		{
			if (Row >= row_ || Col >= col_) {
				throw std::out_of_range("Index out of range");
			}
			return sequence_[Row][Col];
		}

		T& operator()(size_t Row, size_t Col)
		{
			if (Row >= row_ || Col >= col_) {
				throw std::out_of_range("Index out of range");
			}
			return sequence_[Row][Col];
		}

		std::pair<size_t, size_t> size() const {
			return
				std::make_pair(row_, col_);
		}

	private:
		size_t row_{ Row };
		size_t col_{ Col };
		T sequence_[Row][Col]{};
	};
}
#endif /* MATRIX_H */