/* vehicle_land.h */

/*
* Тип наземных транспортных средств описывается шаблоном класса.
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

#ifndef RACE_SIMULATOR_VEHICLE_LAND_H
#define RACE_SIMULATOR_VEHICLE_LAND_H

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
	concept Type_land = std::same_as<T, types::Bactrian<size_t>> ||
		std::same_as<T, types::Dromedar<size_t>> ||
		std::same_as<T, types::Centaur<size_t>> ||
		std::same_as<T, types::Boots<size_t>>;
	
	template<class T> 
		requires Type_land<T> 
	class Vehicle_land : 
		public Vehicle
	{
	public:
		Vehicle_land() = default;
		VEHICLE_LIBRARY_DLL Vehicle_land(double speed, double time_before_rest);
		~Vehicle_land() =default;

	public:
		VEHICLE_LIBRARY_DLL std::string name() const override final;
		VEHICLE_LIBRARY_DLL double race_time(double distance) const override final;

	private:
		double speed_{};
		double time_before_rest_{};
	};
}
#endif /* RACE_SIMULATOR_VEHICLE_LAND_H */