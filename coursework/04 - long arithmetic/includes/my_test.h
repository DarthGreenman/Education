﻿// my_test.h

#ifndef MY_TEST_H
#define MY_TEST_H

#include "includes/my_types.h"
#include "includes/my_ultralong.h"

#include <ctime>
#include <iostream>
#include <random>

namespace my
{
	template<Addable T>
	struct sum
	{
		using value_type = T;
		auto operator()(const T& lhs, const T& rhs) { return lhs + rhs; }
	};

	template<Multiplyable T>
	struct mul
	{
		using value_type = T;
		auto operator()(const T& lhs, const T& rhs) { return lhs * rhs; }
	};

	// Концепты ////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	concept Is_reference = Addable<std::size_t> || Multiplyable<std::size_t>;

	template<typename T, std::size_t N = 2048ull>
	concept Is_sample = Addable<ultralong<N>> || Multiplyable<ultralong<N>>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<Is_reference Reference, Is_sample Sample>
	auto test(std::mt19937 engine, std::size_t count, Reference reference, Sample sample, char sign)
	{
		engine.seed(std::time(nullptr));
		for (decltype(count) i{}; i < count; ++i)
		{
			const typename Reference::value_type ar{ engine() }, br{ engine() };
			std::cout << "Random number:   \t" << ar << " " << sign << " " << br << " = "
				<< reference(ar, br) << '\n';

			const typename Sample::value_type as{ std::to_string(ar) }, bs{ std::to_string(br) };
			std::cout << "Ultralong number:\t" << as << " " << sign << " " << bs << " = "
				<< sample(as, bs) << "\n\n";
		}
	}
}

#endif // !MY_TEST_H