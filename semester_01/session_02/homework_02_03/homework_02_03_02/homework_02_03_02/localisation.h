/*
* GetConsoleCP() - извлекает входную кодовую страницу, 
* используемую консолью, св€занной с вызывающим процессом.
*  онсоль использует кодовую страницу ввода дл€ преобразовани€ ввода
* с клавиатуры в соответствующее значение символа.
* 
* GetConsoleOutputCP() - извлекает выходную кодовую страницу,
* используемую консолью, св€занной с вызывающим процессом.
*  онсоль использует свою выходную кодовую страницу
* дл€ преобразовани€ символьных значений, записанных различными выходными
* функци€ми, в изображени€, отображаемые в окне консоли.
*/

#ifndef LOCALISATION_H
#define LOCALISATION_H

#include <Windows.h>

namespace math {
	class console_localisation {
	public:
		console_localisation(unsigned long int code_page_id) :
			default_in_{ GetConsoleCP() }, 
			default_out_{ GetConsoleOutputCP() }
		{
			SetConsoleCP(code_page_id);
			SetConsoleOutputCP(code_page_id);
		}

		~console_localisation()
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