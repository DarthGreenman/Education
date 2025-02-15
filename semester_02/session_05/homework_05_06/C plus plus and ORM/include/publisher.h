// publisher.h

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "additions.h"

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
		class publisher : public Wt::Dbo::Dbo<publisher>
		{
		public:
			publisher() = default;
			template<typename... Args>
			publisher(Args&&... args) :
				_used_fields{ std::make_tuple(std::forward<Args>(args)...) },
				_bind_fields{ std::make_tuple(Wt::Dbo::collection<Wt::Dbo::ptr<book>>{}) } {
			}
			publisher(const publisher&) = default;
			publisher(publisher&&) = default;
			~publisher() = default;

			publisher& operator=(const publisher&) = default;
			publisher& operator=(publisher&&) = default;

			constexpr static auto type_name() { return "publisher"; }
			constexpr static auto nickname() { return std::string{ "PB" }; }
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
				const auto& [title, address] = (arg.get())->get<0,1>();
				view::print({4, 40, 40}, (arg.get())->id(), title, address);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Action>
			void persist(Action& action)
			{
				Wt::Dbo::field(action,   std::get<0>(_used_fields), "title");                           // 0
				Wt::Dbo::field(action,   std::get<1>(_used_fields), "address");                         // 1
				Wt::Dbo::hasMany(action, std::get<0>(_bind_fields), Wt::Dbo::ManyToOne, type_name());
			}
			
		private:
			std::tuple<
				std::string,
				std::string
			> _used_fields{};

			std::tuple<
				Wt::Dbo::collection<Wt::Dbo::ptr<book>>
			> _bind_fields{};
		};

	} // namespace tps

	template<typename T>
	struct is_publisher
	{
		static const auto value = std::is_same_v<T, tps::publisher>;
	};
	template<typename T>
	constexpr auto is_publisher_v = is_publisher<T>::value;

} // namespace orm

#endif // !PUBLISHER_H