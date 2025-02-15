// store.h

#ifndef STORE_H
#define STORE_H

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
		class reserve;
		class store : public Wt::Dbo::Dbo<store>
		{
		public:
			store() = default;
			template<typename... Args>
			store(Args&&... args) :
				_used_fields{ std::make_tuple(std::forward<Args>(args)...) },
				_bind_fields{ std::make_tuple(Wt::Dbo::collection<Wt::Dbo::ptr<reserve>>{}) } {
			}
			store(const store&) = default;
			store(store&&) = default;
			~store() = default;

			store& operator=(const store&) = default;
			store& operator=(store&&) = default;

			constexpr static auto type_name() { return "bookstore"; }
			constexpr static auto nickname() { return std::string{ "BS" }; }
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
				const auto& [title, web] = (arg.get())->get<0,1>();
				view::print({5, 50, 20}, (arg.get())->id(), title, web);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Action>
			void persist(Action& action)
			{
				Wt::Dbo::field(action,   std::get<0>(_used_fields), "title");                            // 0
				Wt::Dbo::field(action,   std::get<1>(_used_fields), "web");                              // 1
				Wt::Dbo::hasMany(action, std::get<0>(_bind_fields), Wt::Dbo::ManyToOne, type_name());
			}

		private:
			std::tuple<
				std::string,
				std::string
			> _used_fields{};

			std::tuple<
				Wt::Dbo::collection<Wt::Dbo::ptr<reserve>>
			> _bind_fields{};
		};

	} // namespace tps

	template<typename T>
	struct is_store
	{
		static const auto value = std::is_same_v<T, tps::store>;
	};
	template<typename T>
	constexpr auto is_store_v = is_store<T>::value;

} // namespace orm

#endif // !STORE_H