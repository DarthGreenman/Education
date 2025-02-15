// sale.h

#ifndef SALE_H
#define SALE_H

#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <Wt/Dbo/Field.h>
#include <Wt/Dbo/ptr.h>

namespace orm
{
	namespace tps
	{
		class reserve;
		class sale : public Wt::Dbo::Dbo<sale>
		{
		public:
			sale() = default;
			template<typename... Args>
			sale(Args&&... args) :
				_used_fields{ std::make_tuple(std::forward<Args>(args)...) } {
			}
			sale(const sale&) = default;
			sale(sale&&) = default;
			~sale() = default;

			sale& operator=(const sale&) = default;
			sale& operator=(sale&&) = default;

			constexpr static auto type_name() { return "sale"; }
			constexpr static auto nickname() { return std::string{ "SL" }; }
			constexpr static auto order_by() { return std::string{ "price" }; }
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
				const auto& [count, price] = (arg.get())->get<0, 1>();
				view::print({ 4,10,10 }, (arg.get())->id(), count, price);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename Action>
			void persist(Action& action)
			{
				Wt::Dbo::field(action,     std::get<0>(_used_fields), "count");              // 0
				Wt::Dbo::field(action,     std::get<1>(_used_fields), "price");              // 1
				Wt::Dbo::belongsTo(action, std::get<2>(_used_fields), reserve::type_name()); // 2  ŒÕ≈÷ —œ»— ¿
			}

		private:
			std::tuple<
				int,
				double,
				Wt::Dbo::ptr<reserve>
			> _used_fields{};
		};
	} // namespace tps

	template<typename T>
	struct is_sale
	{
		static const auto value = std::is_same_v<T, tps::sale>;
	};
	template<typename T>
	constexpr auto is_sale_v = is_sale<T>::value;

} // namespace orm

#endif // !SALE_H