// command_pattern_test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "include/invoker.h"
#include "include/log_command.h"
#include "include/logger.h"
#include <cstdlib>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <memory>

int main()
{
	std::ofstream logfile{ "event.log" };
	if (!logfile.is_open())
		return 1;

	using namespace pattern;

	invoker c(std::make_unique<send_to_console>(std::cout,std::make_unique<logger>()));
	invoker f(std::make_unique<send_to_journal>(logfile, std::make_unique<logger>()));

	c.send("Error information is output to the console.");
	f.send("Error information is written to the log.");

	std::cout << '\n';
	std::system("pause");
	
	return 0;
}