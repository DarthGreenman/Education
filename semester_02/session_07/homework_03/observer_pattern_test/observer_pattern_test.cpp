// observer_pattern_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/logger.h"
#include "include/viewer.h"
#include <cstdlib>
#include <iostream>
#include <memory>

int main()
{
	auto cons = std::make_shared<pattern::console>();
	auto file = std::make_shared<pattern::journal>("log.txt");

	auto& log = pattern::logger::get_instance();
	
	/// Регистрация наблюдателей
	log.viewer_reg(cons);
	log.viewer_reg(file);

	/// Запись логов
	log.warning("Warning");
	log.error("Error");
	log.fatal("Fatal");

	/// Удаление наблюдателей
	log.viewer_unreg(cons);
	log.viewer_unreg(file);

	std::cout << '\n';
	std::system("pause");
	return 0;
}
