// C plus plus and ORM.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/additions.h"
#include "include/app.h"
#include "include/book.h"
#include "include/date.h"
#include "include/my_localisation.h"
#include "include/publisher.h"
#include "include/reserve.h"
#include "include/sale.h"
#include "include/store.h"
#include "include/writer.h"

#include "include/message.h"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <Wt/Dbo/Exception.h>
#include <Wt/Dbo/ptr.h>

#define FIRST_LAUNCH

int main()
{
	const my::console_localisation<my::code_pages::rus> loc{};

	const std::string connection_string
	{
		"host=localhost "
		"port=5432 "
		"dbname=bookmarket "
		"user=postgres "
		"password=489aMARy"
	};

	try
	{
#ifdef FIRST_LAUNCH

		// Создать схему базы данных для сопоставленных таблиц. Создание схемы завершится исключением,
		// если одна или несколько таблиц уже существовали, и или есть ошибки в даннх для связывания таблиц.
		// Создание таблиц выполняется в транзакции, которая откатывается при возникновении ошибки.
		orm::drv::driver init_data{ connection_string };
		init_data.make_schema();

		using namespace orm::drv;
		using namespace orm::tps;
		using namespace tpsg;

		// Авторы       
		const auto Pushkin = init_data.add<writer>(person_name{ "Alexander", "Sergeevich", "Pushkin" }, "poet",
			date{ day{6}, month{month::jun()}, year{1799} }, date{ day{10}, month{month::feb()}, year{1837} });
		const auto Dostoevsky = init_data.add<writer>(person_name{ "Fyodor", "Mikhailovich", "Dostoevsky" }, "novelist",
			date{ day{10}, month{month::feb()}, year{1837} }, date{ day{9}, month{month::feb()}, year{1881} });
		const auto Lermontov = init_data.add<writer>(person_name{ "Mikhail", "Yurievich", "Lermontov" }, "poet",
			date{ day{15}, month{month::oct()}, year{1814} }, date{ day{27}, month{month::jul()}, year{1841} });
		const auto Tolstoy = init_data.add<writer>(person_name{ "Alexey", "Nikolaevich", "Tolstoy" }, "novelist",
			date{ day{29}, month{month::dec()}, year{1882} }, date{ day{23}, month{month::feb()}, year{1945} });

		// Издательства
		const auto Goslitizdat = init_data.add<publisher>("State Publishing House of Fiction", "Moscow, Novaya Basmannaya st., 19, building 1");
		const auto Young_Guard = init_data.add<publisher>("Publishing House Young Guard", "Moscow, Sushchevskaya st., 19, building 5");
		const auto Glazunov = init_data.add<publisher>("Ilya Glazunov's Typography", "Saint Petersburg");
		const auto Gosizdat = init_data.add<publisher>("State Publishing House", "Moscow, Orlikov lane, building 3");
		const auto Pravda = init_data.add<publisher>("Publishing and printing house Pravda", "Moscow, Tverskaya st., building 48");

		// Книги
		const std::vector<Wt::Dbo::ptr<book>> books
		{
			init_data.add<book>("Ruslan and Ludmila", year{1952}, Pushkin, Goslitizdat),                          // 0
			init_data.add<book>("Eugene Onegin", year{1956}, Pushkin, Gosizdat),                                  // 1
			init_data.add<book>("Crime and Punishment", year{1953}, Dostoevsky, Goslitizdat),                     // 2
			init_data.add<book>("Crime and Punishment", year{1967}, Dostoevsky, Young_Guard),                     // 3
			init_data.add<book>("The Gambler", year{1969}, Dostoevsky, Young_Guard),                              // 4
			init_data.add<book>("The Idiot", year{1968}, Dostoevsky, Young_Guard),                                // 5
			init_data.add<book>("The Brothers Karamazov", year{1970}, Dostoevsky, Goslitizdat),                   // 6
			init_data.add<book>("A Hero of our time", year{1840}, Lermontov, Glazunov),                           // 7
			init_data.add<book>("Lermontov M.Yu.Collected Works in Four Volumes", year{1986}, Lermontov, Pravda), // 8
			init_data.add<book>("Hyperboloid of engineer Garin", year{1927}, Tolstoy, Gosizdat),                  // 9
			init_data.add<book>("Walking in torment", year{1920}, Tolstoy, Gosizdat)                              // 10

		};
		// Магазины
		const std::vector<Wt::Dbo::ptr<store>> shop
		{
			init_data.add<store>("Bookmanic", "kniganika.ru"),                                         // 0
			init_data.add<store>("Second-hand bookseller bookstore", "bukinistkniga.ru"),              // 1
			init_data.add<store>("Bookstore Tsiolkovsky", "primuzee.ru"),                              // 2
			init_data.add<store>("Antique and second-hand bookstore Book Entropy", "entropiabook.ru")  // 3
		};
		// Зарезервировано
		const std::vector<Wt::Dbo::ptr<reserve>> stock
		{
			init_data.add<reserve>(1, books.at(9), shop.at(3)),  // 0
			init_data.add<reserve>(2, books.at(2), shop.at(0)),  // 1
			init_data.add<reserve>(1, books.at(6), shop.at(2)),  // 2
			init_data.add<reserve>(1, books.at(6), shop.at(1)),  // 3
			init_data.add<reserve>(2, books.at(0), shop.at(2)),  // 4
			init_data.add<reserve>(1, books.at(10), shop.at(0)), // 5
			init_data.add<reserve>(1, books.at(3), shop.at(3)),  // 6
			init_data.add<reserve>(1, books.at(7), shop.at(0)),  // 7
			init_data.add<reserve>(3, books.at(4), shop.at(1)),  // 8
			init_data.add<reserve>(1, books.at(8), shop.at(3))   // 9
		};

		init_data.add<sale>(1, 100.45, stock.at(0));
		init_data.add<sale>(1, 190.34, stock.at(1));
		init_data.add<sale>(1, 225.67, stock.at(1));
		init_data.add<sale>(1, 1225.77, stock.at(2));
		init_data.add<sale>(1, 2252.97, stock.at(3));
		init_data.add<sale>(2, 2301.37, stock.at(4));

#endif // FIRST_LAUNCH

#ifndef FIRST_LAUNCH

		orm::app used_bookstore{ connection_string, "БУКИНИСТИЧЕСКИЙ КНИЖНЫЙ МАГАЗИН" };
		used_bookstore.exec();

#endif // FIRST_LAUNCH

	}
	catch (const Wt::Dbo::Exception& error) { std::cerr << error.what() << '\t'; }
	catch (const std::exception& error) { std::cerr << error.what() << '\t'; }

	std::system("pause");
	return 0;
}
