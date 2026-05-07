// fox.h

#ifndef FOX_H_FOR_THE_ARDUINO_UNO_PROJECT
#define FOX_H_FOR_THE_ARDUINO_UNO_PROJECT

namespace fox {

	template <typename... T>
	struct package_size {
		static constexpr auto value = sizeof... (T);
	};

	struct expand_parameter_package {
		template <typename... T> constexpr expand_parameter_package(T...) {}
	};

	template <typename T, typename U> struct pair { T first; U second; };
	template <typename T, typename U> constexpr decltype(auto) make_pair(T&& first, U&& second) {
		return fox::pair<T, U>{first, second};
	}

	template <typename T, typename U> struct is_same;
	template <typename T> struct is_same<T, T> { static constexpr bool value = true; };
	template <typename T, typename U> struct is_same { static constexpr bool value = false; };

	template <bool B, typename T = void> struct enable_if;
	template <typename T> struct enable_if<true, T> { using type = T; };

	template <typename T> float map(T x, T in_min, T in_max, T out_min, T out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
} /// namespace fox

#endif // !FOX_H_FOR_THE_ARDUINO_UNO_PROJECT