// book.h

#ifndef BOOK_H
#define BOOK_H

#include "additions.h"
#include "date.h"

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <Wt/Dbo/collection.h>
#include <Wt/Dbo/Field.h>
#include <Wt/Dbo/ptr.h>

namespace orm
{
	namespace tps
	{
		class writer;
		class publisher;
		class reserve;
		class book : public Wt::Dbo::Dbo<book>
		{
		public:
			book() = default;
			template<typename... Args>
			book(Args&&... args) :
				_used_fields{ std::make_tuple(std::forward<Args>(args)...) },
				_bind_fields{ std::make_tuple(Wt::Dbo::collection<Wt::Dbo::ptr<reserve>>{}) } {
			}
			book(const book&) = default;
			book(book&&) = default;
			~book() = default;

			book& operator=(const book&) = default;
			book& operator=(book&&) = default;

			constexpr static auto type_name() { return "book"; }
			constexpr static auto nickname() { return std::string{ "BK" }; }
			constexpr static auto order_by() { return std::string{ "title" }; }
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			const static auto hash()
			{
				return std::hash<std::string>{}(type_name());
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<std::size_t... N>
			auto get() const
			{
				constexpr auto size_tuple = std::tuple_size_v<decltype(_used_fields)>;
				static_assert(((N < size_tuple) && ...), "Tuple index out of bounds");
				return std::make_tuple((std::get<N>(_used_fields)) ...);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Arg>
			const static auto view(Arg&& arg)
			{
				const auto& [title, year] = (arg.get())->get<0, 1>();
				view::print({ 4,50,7 }, (arg.get())->id(), title, year);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Action>
			void persist(Action& action)
			{
				Wt::Dbo::field(action, std::get<0>(_used_fields), "title");                    // 0
				Wt::Dbo::field(action, std::get<1>(_used_fields), "year");                     // 1
				Wt::Dbo::belongsTo(action, std::get<2>(_used_fields), writer::type_name());        // 2  ŒÕ≈÷ —œ»— ¿
				Wt::Dbo::belongsTo(action, std::get<3>(_used_fields), publisher::type_name());     // 3  ŒÕ≈÷ —œ»— ¿
				Wt::Dbo::hasMany(action, std::get<0>(_bind_fields), Wt::Dbo::ManyToOne, type_name());
			}

		private:
			std::tuple<
				std::string,
				tpsg::year,
				Wt::Dbo::ptr<writer>,
				Wt::Dbo::ptr<publisher>
			> _used_fields{};

			std::tuple<
				Wt::Dbo::collection<Wt::Dbo::ptr<reserve>>
			> _bind_fields{};
		};

	} // namespace tps

	template<typename T>
	struct is_book
	{
		static const auto value = std::is_same_v<T, tps::book>;
	};
	template<typename T>
	constexpr auto is_book_v = is_book<T>::value;

} // namespace orm

#endif // !BOOK_H