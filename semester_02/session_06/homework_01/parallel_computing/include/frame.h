// frame.h

#ifndef FRAME_H
#define FRAME_H

#include "frame_iterator.h"

#include <cassert>
#include <iterator>
#include <limits>
#include <vector>

namespace plc
{
	template<typename Type>
	class frame
	{
	public:
		using vector = typename std::vector<Type>;
		using size_type = typename std::vector<Type>::size_type;
		using iterator = frame_iterator<Type>;

		frame() = default;
		frame(size_type pre_frame_count, const vector& vec) :
			_size_old{ std::size(vec) },
			_frame_count{ _get_frame_count(pre_frame_count, std::size(vec)) },
			_frame_width{ std::size(vec) / pre_frame_count },
			_vec{ vec }
		{
			_normalize(_get_align_value(pre_frame_count, _size_old));
		}
		frame(size_type pre_frame_count, vector&& vec) :
			_size_old{ std::size(vec) },
			_frame_count{ _get_frame_count(pre_frame_count, std::size(vec)) },
			_frame_width{ std::size(vec) / pre_frame_count },
			_vec{ std::move(vec) }
		{
			_normalize(_get_align_value(pre_frame_count, _size_old));
		}
		frame(const frame<Type>&) = delete;
		frame(frame<Type>&&) = delete;
		~frame() = default;

		frame& operator=(const frame&) = delete;
		frame& operator=(frame&&) = delete;

		iterator begin()
		{
			return iterator(
				std::begin(_vec),
				std::next(std::begin(_vec), _frame_width)
			);
		}
		iterator end()
		{
			return iterator(
				std::prev(std::end(_vec), _frame_width), 
				std::end(_vec)
			);
		}
		vector get()
		{
			_vec.erase(std::next(std::begin(_vec), _size_old),
				std::end(_vec));
			return _vec;
		}
	
	private:
		size_type _get_frame_count(size_type pre_frame_count, size_type size) const
		{
			assert((pre_frame_count > 1) && "Increase your frame count");
			assert((size / pre_frame_count > 2) && "Decrease the number of frames");
			return static_cast<bool>(size % pre_frame_count) ? pre_frame_count + 1 : pre_frame_count;
		}
		size_type _get_align_value(size_type pre_frame_count, size_type size) const
		{
			return !static_cast<bool>(size % pre_frame_count) ? 0 :
				_frame_width * _frame_count - size;
		}
		void _normalize(size_type align_value)
		{
			vector tail(align_value + _frame_width, std::numeric_limits<Type>::max());
			_vec.reserve(std::size(tail));
			std::copy(std::cbegin(tail), std::cend(tail), std::back_inserter(_vec));
		}

	private:
		const size_type _size_old{};
		const size_type _frame_count{};
		const size_type _frame_width{};
		vector _vec{};
	};

} // namespace plc

#endif // ! FRAME_H
