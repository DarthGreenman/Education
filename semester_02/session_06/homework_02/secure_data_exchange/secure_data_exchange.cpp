// secure_data_exchange.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/mutex_queue_counter.h"

#include <cstdlib>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <mutex>
#include <thread>

int main()
{
	constexpr std::size_t headcount{ 1'000'000 };
	std::mutex mux{};
	std::condition_variable con{};

	using std::cout;
	
	th::mutex_queue_counter counter_inc{};
	std::thread inc{ [&counter_inc, &mux, &con, headcount]()
		{
			std::lock_guard<std::mutex> lk{mux};
			cout << "Value of the counter_inc object\n - before increment: " << counter_inc.get_value();
			while (counter_inc < headcount)
				counter_inc.increment();
			cout << "\n - after increment: " << counter_inc.get_value();
			con.notify_one();
		}
	};
		
	th::mutex_queue_counter counter_dec{};	
	std::thread dec{ [&counter_dec, &counter_inc, &mux, &con, headcount]()
		{
			std::unique_lock<std::mutex> lk{mux};
			con.wait(lk, [headcount]() { return headcount; });
			
			cout << "\n\nValues of objects before exchange"
				<< "\n - counter_inc: " << counter_inc.get_value()
				<< "\n - counter_dec: " << counter_dec.get_value();
			
			th::swap(counter_dec, counter_inc);
			
			cout << "\n\nValues of objects after exchange"
				<< "\n - counter_inc: " << counter_inc.get_value()
				<< "\n - counter_dec: " << counter_dec.get_value();

			do {
				counter_dec.decrement();
			} while (counter_dec != std::numeric_limits<std::size_t>::min());
			
			cout << "\n\nValue of the object counter_dec\nafter decrement: "
				<< counter_dec.get_value() << '\n';
			
			lk.unlock();
		}
	};

	inc.join();
	dec.join();

	std::cout << '\n';
	std::system("pause");
    return 0;
}