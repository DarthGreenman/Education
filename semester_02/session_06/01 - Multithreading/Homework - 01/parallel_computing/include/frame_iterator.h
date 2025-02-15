// iterator.h

#ifndef FRAME_ITERATOR_H
#define FRAME_ITERATOR_H

#include <iterator>
#include <utility>
#include <vector>

namespace plc
{
	template<typename Type>
	class frame_iterator
	{
	public:
		template<typename Type>	friend class frame;
		using iterator_category = typename std::output_iterator_tag;
		using size_type = typename std::vector<Type>::size_type;
		using value_type = typename std::pair<typename std::vector<Type>::iterator,
			typename std::vector<Type>::iterator>;
		using pointer = typename std::pair<typename std::vector<Type>::iterator,
			typename std::vector<Type>::iterator>*;
		using reference = typename std::pair<typename std::vector<Type>::iterator,
			typename std::vector<Type>::iterator>&;
	
	private:		
		template<typename InputIt>
		frame_iterator(InputIt first, InputIt last) :
			_frame{ std::make_pair(first, last) }
		{
		}
		
	public:
		frame_iterator(const frame_iterator&) = default;
		frame_iterator(frame_iterator&&) = default;
		~frame_iterator() = default;

		frame_iterator& operator=(const frame_iterator&) = default;
		frame_iterator& operator=(frame_iterator&&) = default;

		frame_iterator& operator++()
		{
			const auto frame_width = _frame.second - _frame.first;
			_frame.first = std::next(_frame.first, frame_width);
			_frame.second = std::next(_frame.second, frame_width);
			return *this;
		}
		frame_iterator operator++(int)
		{
			auto prev{ *this };
			++*this;
			return prev;
		}
		reference operator*() { return _frame; }
		pointer operator->() { return _frame; }

		value_type get() const { return _frame; }
		
	private:
		value_type _frame{};
	};

	template<typename Type>
	auto operator==(const frame_iterator<Type>& lhs, 
		const frame_iterator<Type>& rhs)
	{
		const auto& [lf, ls] = lhs.get();
		const auto& [rf, rs] = rhs.get();
		return lf == rf && ls == rs;
	}
	template<typename Type>
	auto operator!=(const frame_iterator<Type>& lhs, 
		const frame_iterator<Type>& rhs)
	{
		return !(lhs == rhs);
	}

} // namespace plc

#endif // !FRAME_ITERATOR_H
