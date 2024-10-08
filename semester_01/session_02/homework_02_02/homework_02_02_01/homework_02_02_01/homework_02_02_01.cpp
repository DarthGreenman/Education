// homework_02_02_01.cpp

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <exception>

namespace bag {
	enum class months {
		january = 1,
		february,
		march,
		april,
		may,
		june,
		july,
		august,
		september,
		october,
		november,
		december
	};

	const std::string get_month(int);
	const std::string get_month(
		const std::vector<std::pair<int, std::string>>&,
		int
	);
	const int get_number_months_year();
}

template <typename T>
T get_input_value(const std::string & =
	"Введите правильное цифровое значение: "
);


int main()
{
	const std::vector<std::pair<int, std::string>> months_ru{
		{1, "Январь"},
		{2, "Февраль"},
		{3, "Март"},
		{4, "Апрель"},
		{5, "Май"},
		{6, "Июнь"},
		{7, "Июль"},
		{8, "Август"},
		{9, "Сентябрь"},
		{10, "Октябрь"},
		{11, "Ноябрь"},
		{12, "Декабрь"}
	};
	const std::vector<std::pair<int, std::string>> months_en{
		{1, "January"},
		{2, "February"},
		{3,	"March"},
		{4,	"April"},
		{5,	"May"},
		{6,	"June"},
		{7,	"July"},
		{8,	"August"},
		{9,	"September"},
		{10, "October"},
		{11, "November"},
		{12, "December"}
	};

	setlocale(LC_ALL, "Russian");
	using std::cout;
	
	for (int number{ std::numeric_limits<int>::max() }; number != 0; ) {
		cout << "Введите номер месяца: ";
		try {
			number = get_input_value<int>();
		}
		catch (const std::exception& e) {
			std::cerr << e.what();
		}
		
		if (const int number_months_year{ bag::get_number_months_year() };
			number > 0 && number <= number_months_year) {
			cout << bag::get_month(number) << '\n';
			/* bag::get_month(months_ru, number) - вариант с подключением календаря на кирилице */
			/* bag::get_month(months_en, number) - вариант с подключением календаря на латинице */
		}
		else if (number > number_months_year) {
			cout << "Неправильный номер!\n";
		}
	}
	cout << "До свидания";
	return 0;
}

namespace bag {
	const std::string get_month(int month_number)
	{
		std::string month{};
		switch (month_number)
		{
			case static_cast<int>(bag::months::january):
				month = "Январь";
				break;
			case static_cast<int>(bag::months::february):
				month = "Февраль";
				break;
			case static_cast<int>(bag::months::march):
				month = "Март";
				break;
			case static_cast<int>(bag::months::april):
				month = "Апрель";
				break;
			case static_cast<int>(bag::months::may):
				month = "Май";
				break;
			case static_cast<int>(bag::months::june):
				month = "Июнь";
				break;
			case static_cast<int>(bag::months::july):
				month = "Июль";
				break;
			case static_cast<int>(bag::months::august):
				month = "Август";
				break;
			case static_cast<int>(bag::months::september):
				month = "Сентябрь";
				break;
			case static_cast<int>(bag::months::october):
				month = "Октябрь";
				break;
			case static_cast<int>(bag::months::november):
				month = "Ноябрь";
				break;
			case static_cast<int>(bag::months::december):
				month = "Декабрь";
				break;
			default:
				throw std::exception("\nВведенное значение "
					"не входит в допустимый интервал.\n");
		}
		return month;
	}

	const std::string get_month(
		const std::vector<std::pair<int, std::string>>& months,
		int month_number
	)
	{
		if (month_number < 1 || month_number > months.size()) {
			throw std::exception("\nВведенное значение "
				"не входит в допустимый интервал.\n");
		}
		std::string month{};
		for (const auto& m : months) {
			if (m.first == month_number) {
				month = m.second;
				break;
			}
		}
		return month;
	}
	const int get_number_months_year()
	{
		return 12;
	}
}

template <typename T>
T get_input_value(
	const std::string& message
)
{
	while (true) {
		T result{};
		std::cin >> result;
		if (std::cin.good()) {
			return result;
		}
		else if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(),
				'\n');
			std::cout << message;
		}
		else /* if (std::cin.bad()) */ {
			throw std::exception{ "\nПроизошла фатальная ошибка!\n" };
		}
	}
}
