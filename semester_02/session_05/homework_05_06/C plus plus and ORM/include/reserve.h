// reserve.h

#ifndef RESERVE_H
#define RESERVE_H

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
		class store;
		class sale;
		class reserve : public Wt::Dbo::Dbo<reserve>
		{
		public:
			reserve() = default;
			template<typename... Args>
			reserve(Args&&... args) :
				_used_fields{ std::make_tuple(std::forward<Args>(args)...) },
				_bind_fields{ std::make_tuple(Wt::Dbo::collection<Wt::Dbo::ptr<sale>>{}) } {
			}
			reserve(const reserve&) = default;
			reserve(reserve&&) = default;
			~reserve() = default;

			reserve& operator=(const reserve&) = default;
			reserve& operator=(reserve&&) = default;

			constexpr static auto type_name() { return "reserve"; }
			constexpr static auto nickname() { return std::string{ "RV" }; }
			constexpr static auto order_by() { return std::string{ "count" }; }
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
			template<typename Action>
			void persist(Action& action)
			{
				Wt::Dbo::field(action,     std::get<0>(_used_fields), "count");                     // 0
				Wt::Dbo::belongsTo(action, std::get<1>(_used_fields), book::type_name());           // 1  ŒÕ≈÷ —œ»— ¿
				Wt::Dbo::belongsTo(action, std::get<2>(_used_fields), store::type_name());          // 2  ŒÕ≈÷ —œ»— ¿
				Wt::Dbo::hasMany(action,   std::get<0>(_bind_fields), Wt::Dbo::ManyToOne, type_name());
			}

		private:
			std::tuple<
				int,
				Wt::Dbo::ptr<book>,
				Wt::Dbo::ptr<store>
			> _used_fields{};

			std::tuple<
				Wt::Dbo::collection<Wt::Dbo::ptr<sale>>
			> _bind_fields{};
		};

	} // namespace tps

	template<typename T>
	struct is_reserve
	{
		static const auto value = std::is_same_v<T, tps::reserve>;
	};
	template<typename T>
	constexpr auto is_reserve_v = is_reserve<T>::value;

} // namespace orm

#endif // !RESERVE_H