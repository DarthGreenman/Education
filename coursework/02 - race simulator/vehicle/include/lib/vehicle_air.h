/* vihicle_air.h */

/*
* Тип воздушных транспортных средств описывается шаблоном класса.
* Единственный параметр шаблона может принимать агрумент
* удовлетворяющий условию std::same_as<T, ...>.
* Так как класс имеет поля скалярного типа и операции с выделением
* и освобождением памяти не производятся, то определим:
*  - конструктор по умолчанию;
*  - конструктор копии и оператор присваивания копированием;
*  - конструктор перемещения и оператор присваивания перемещением;
*  - деструктор,
* как автоматически создаваемыми компилятором. Конструктор по умолчанию
* и деструктор определяем явно.
*/

#ifndef RACE_SIMULATOR_VEHICLE_AIR_H
#define RACE_SIMULATOR_VEHICLE_AIR_H

#ifdef VEHICLE_LIBRARY_EXPORT
#define VEHICLE_LIBRARY_DLL __declspec(dllexport)
#else
#define VEHICLE_LIBRARY_DLL __declspec(dllimport)
#endif

#include "vehicle.h"
#include "types.h"

#include <string>
#include <concepts>

namespace vehicles
{
	template<typename T>
	concept Type_air = std::same_as<T, types::Carpet<double>> ||
		std::same_as<T, types::Eagle<double>> ||
		std::same_as<T, types::Broom<double>>;

	template<class T>
		requires Type_air<T>
	class Vehicle_air :	
		public Vehicle
	{
	public:
		Vehicle_air() = default;
		VEHICLE_LIBRARY_DLL Vehicle_air(double speed);
		~Vehicle_air() = default;

	public:
		VEHICLE_LIBRARY_DLL std::string name() const override final;
		VEHICLE_LIBRARY_DLL double race_time(double distance) const override final;
	
	private:
		double speed_;
	};
}
#endif /* RACE_SIMULATOR_VEHICLE_AIR_H */