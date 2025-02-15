/*
* GetConsoleCP() - извлекает входную кодовую страницу,
* используемую консолью, связанной с вызывающим процессом.
* Консоль использует кодовую страницу ввода для преобразования ввода
* с клавиатуры в соответствующее значение символа.
*
* GetConsoleOutputCP() - извлекает выходную кодовую страницу,
* используемую консолью, связанной с вызывающим процессом.
* Консоль использует свою выходную кодовую страницу
* для преобразования символьных значений, записанных различными выходными
* функциями, в изображения, отображаемые в окне консоли.
*/

#ifndef LOCALISATION_H
#define LOCALISATION_H

#include <Windows.h>

namespace math {
	class Console_localisation {
	public:
		Console_localisation(unsigned long int code_page_id) :
			default_in_{ GetConsoleCP() },
			default_out_{ GetConsoleOutputCP() }
		{
			SetConsoleCP(code_page_id);
			SetConsoleOutputCP(code_page_id);
		}

		~Console_localisation()
		{
			SetConsoleCP(default_in_);
			SetConsoleOutputCP(default_out_);
		}
	private:
		unsigned long int default_in_;
		unsigned long int default_out_;
	};
}

#endif /* LOCALISATION_H */
