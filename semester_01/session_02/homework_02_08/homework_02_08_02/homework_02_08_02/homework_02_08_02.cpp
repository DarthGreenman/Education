// homework_02_08_02.cpp
/*
 * Создайте собственный класс исключения для ошибки создания геометрической фигуры.
 * Подумайте, от какого типа его унаследовать, и не забудьте поместить новый класс
 * в отдельную пару файлов.
 * 
 * Возьмите иерархию классов из предыдущего задания.Теперь при создании экземпляров
 * классов вам нужно проверять аргументы, которые поступают на вход в конструктор.
 * Если аргументы не соответствуют ограничениям, объект не должен быть создан,
 * а на консоль должно быть выведено соответствующее сообщение.
*/

/*
* Программа в автоматическом режиме демонстрирует обнаружение ошибок данных
* построения геометрических фигур. Программа строит треугольник произвольной
* формы по трем сторонам. Задаются три набора длин сторон, причем, один набор
* корректный, остальные имеют ошибки разного типа.
*/

#include "localisation.h"
#include "Triangle.h"
#include "IShape.h"
#include "Exceptions.h"

#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <vector>

struct Params {
	std::pair<std::string, double> side_one;
	std::pair<std::string, double> side_two;
	std::pair<std::string, double> side_third;
};

int main()
{
	bag::Console_localisation localisation(1251);
	using std::cout;
	using std::cerr;
	
	const std::vector<Params> params{ 
		{{"AB", 30}, {"BC", 50},{"CD", 40}},
		{{"AB", 80}, {"BC", 50},{"CD", -40}},
		{{"AB", 30}, {"BC", 150},{"CD", 30}}
	};
	
	for (const auto& param : params)
	{
		cout << "Данные для построения фигуры:\n";
		cout << param.side_one.first << " = " << param.side_one.second << "; "
			<< param.side_two.first << " = " << param.side_two.second << "; "
			<< param.side_third.first << " = " << param.side_third.second;

		std::unique_ptr<geo::Triangle> triangle{};
		try
		{
			triangle =
				std::make_unique<geo::Triangle>(
					param.side_one.second, 
					param.side_two.second,
					param.side_third.second
				);
		}
		catch (const geo::Errors_data& e) {
			cerr << '\n' << e.what() << ' ' << typeid(e).name() << "\n\n";
			continue;
		}

		auto&& [name, is_correct, sides, angles] =
			triangle.get()->type_info();
		if (is_correct) {
			cout << "\nФигура " << name << " создана:";

			cout << "\nстороны: ";
			for (size_t i{}; i < sides.size(); ++i) {
				const std::string sep{ i != sides.size() - 1 ? ", " : ";" };
				cout << sides[i] << sep;
			}

			cout << "\nуглы: ";
			for (size_t i{}; i < angles.size(); ++i) {
				const std::string sep{ i != angles.size() - 1 ? ", " : "." };
				cout << angles[i] << sep;
			}
			cout << "\n\n";
		}
	}	
	return 0;
}