// parallel_computing.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/plc.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <queue>
#include <ratio>
#include <thread>

// Идея: входящие векторы делятся на количество кадров (фреймов) одинакового размера.
// Количество фреймов рассчитывается на основании первоначального значения количества
// потоков. В зависимости от количества элементов во входещем векторе и предварительному
// количеству потокок, векторы могут быть дополненные "хвостом" с незначащими
// данными, например "0", или std::numeric_limits<Type>::max(), или ... Данная процедура
// нормализации позволяет ко всеми фреймами применять одинаковые алгоритмы. После вычесления
// "хвост" удалятся.
// Для каждой итерации, итератор предоставляет [beg,end) фрейма, интервалы поступают
// в виде параметров в функции. Каждая функция вычисляется в отдельном потоке. Потоки 
// аккумулируются в масииве и запускаются параллельно.

namespace utl
{
	static auto print_heading(const std::initializer_list<size_t>& count_list)
	{
		using namespace std;
		cout << "Number of parallel threads supported by the implementation: "
			<< thread::hardware_concurrency() << "\n\n";

		cout << setw(25) << "Quantity of elements";
		for_each(cbegin(count_list), cend(count_list),
			[](size_t count) { cout << setw(20) << count; });
		cout << '\n';
	};

	static auto print_result(plc::count_threads thread,
		std::queue<std::chrono::duration<double, std::milli>>& time_points)
	{
		using namespace std;
		cout << setw(20) << "Count threads: ";
		cout << setw(5) << static_cast<unsigned>(thread);
		for (; !time_points.empty(); time_points.pop())
			cout << setw(20) << time_points.front();
		cout << '\n';
	};
} // namespace utl


int main()
{
	using namespace plc;
	for (auto& count_thread : { count_threads::one, count_threads::two, count_threads::four,
		count_threads::eight, count_threads::sixteen, count_threads::thirty_two })
	{
		using namespace std;
		const initializer_list<size_t> count_list{ 1'000,10'000,100'000,1'000'000,10'000'000 };
		if (count_thread == count_threads::one)
			utl::print_heading(count_list);

		queue<chrono::duration<double, milli>> time_points{};
		for (auto& count : count_list)
		{
			const auto start = chrono::high_resolution_clock::now();
			const auto sum = adder(count_thread, make_vector(count), make_vector(count));
			const auto finish = chrono::high_resolution_clock::now();
			time_points.emplace((finish - start).count());
		}
		utl::print_result(count_thread, time_points);
	}

	std::system("pause");
	return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.