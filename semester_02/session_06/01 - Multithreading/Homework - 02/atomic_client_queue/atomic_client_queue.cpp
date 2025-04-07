// atomic_client_queue.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/atomic_queue_counter.h"

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <utility>

/*
Атомарная очередь клиентов
Нужно модифицировать задание 1 к первому уроку так, чтобы счётчик клиентов был атомарным.
Все операции со счётчиков должны быть атомарными.
Проверьте работу различными способами упорядочения доступа к памяти.
*/

int main()
{
	const std::unordered_map<std::memory_order, std::string> memory_order_list
	{
		{ std::memory_order::relaxed, "std::memory_order::relaxed" },
		{ std::memory_order::consume, "std::memory_order::consume" },
		{ std::memory_order::acquire, "std::memory_order::acquire" },
		{ std::memory_order::release, "std::memory_order::release" },
		{ std::memory_order::acq_rel, "std::memory_order::acq_rel" },
		{ std::memory_order::seq_cst, "std::memory_order::seq_cst" }
	};

	std::cout << "Working of atomic operations with\ndifferent memory naccess nordering methods: ";
	for (const auto& memory_order : memory_order_list)
	{
		constexpr std::size_t headcount{ 10 };
		using atomic_queue_counter = th::atomic_queue_counter<headcount>;
		auto (atomic_queue_counter:: * increment)(std::chrono::milliseconds) = &atomic_queue_counter::increment;
		auto (atomic_queue_counter:: * decrement)(std::chrono::milliseconds) = &atomic_queue_counter::decrement;

		constexpr auto wait = std::make_pair(std::chrono::milliseconds{150},
			std::chrono::milliseconds{300});
		atomic_queue_counter counter{ memory_order.first };

		std::cout << "\n\n" << memory_order.second << '\n';
		const auto s = std::chrono::high_resolution_clock::now();
		{
			std::jthread inc{ increment, &counter, wait.first }, dec{ decrement, &counter, wait.second };
		}
		const auto f = std::chrono::high_resolution_clock::now();
		std::cout << '\n' << std::chrono::duration<double>{ f - s };
	}

	std::cout << '\n';
	std::system("pause");
	return 0;
}
