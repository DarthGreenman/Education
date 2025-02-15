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

#include <stdexcept>
#include <system_error>
#include <Windows.h>
#include <WinNls.h>

namespace my 
{
	struct code_pages
	{
		static constexpr unsigned int rus{ 1251 };
	};

	template<unsigned int code_page_id>
	class console_localisation {
	public:
		console_localisation() :
			in_{ GetConsoleCP() },
			ou_{ GetConsoleOutputCP() }
		{
			if (!IsValidCodePage(code_page_id))
				throw std::invalid_argument{ "Code page is invalid." };

			if (!(SetConsoleCP(code_page_id) && SetConsoleOutputCP(code_page_id)))
				throw std::system_error{ std::make_error_code(std::errc::invalid_argument),
				"Input and output code pages were not specified"};
		}

		~console_localisation()
		{
			SetConsoleCP(in_);
			SetConsoleOutputCP(ou_);
		}
	private:
		unsigned int in_;
		unsigned int ou_;
	};
}

#endif /* LOCALISATION_H */
