// writer.h

#ifndef WRITER_H
#define WRITER_H

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
		class book;
		class writer : public Wt::Dbo::Dbo<writer>
		{
		public:
			writer() = default;
			template<typename... Args>
			writer(Args&&... args) :
				_used_fields{ std::make_tuple(std::forward<Args>(args)...) },
				_bind_fields{ std::make_tuple(Wt::Dbo::collection<Wt::Dbo::ptr<book>>{}) } {
			}
			writer(const writer&) = default;
			writer(writer&&) = default;
			~writer() = default;

			writer& operator=(const writer&) = default;
			writer& operator=(writer&&) = default;

			constexpr static auto type_name() { return "writer"; }
			constexpr static auto nickname() { return std::string{ "WR" }; }
			constexpr static auto order_by() { return std::string{ "firstname" }; }
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
				const auto& [name, genre, birth, death] = (arg.get())->get<0, 1, 2, 3>();
				view::print({ 4, 35, 12, 12, 12 }, (arg.get())->id(), name, genre, birth, death);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Action>
			void persist(Action& action)
			{
				Wt::Dbo::field(action,   std::get<0>(_used_fields), "name");                       // 0
				Wt::Dbo::field(action,   std::get<1>(_used_fields), "genre");                      // 1
				Wt::Dbo::field(action,   std::get<2>(_used_fields), "birth");                      // 2
				Wt::Dbo::field(action,   std::get<3>(_used_fields), "death");                      // 3
				Wt::Dbo::hasMany(action, std::get<0>(_bind_fields), Wt::Dbo::ManyToOne, type_name());
			}

		private:
			std::tuple<
				tps::person_name,
				std::string,
				tpsg::date,
				tpsg::date
			> _used_fields{};

			std::tuple<
				Wt::Dbo::collection<Wt::Dbo::ptr<book>>
			> _bind_fields{};

		};

	} // namespace tps

	template<typename T>
	struct is_writer
	{
		static const auto value = std::is_same_v<T, tps::writer>;
	};
	template<typename T>
	constexpr auto is_writer_v = is_writer<T>::value;

} // namespace orm

#endif // !WRITER_H