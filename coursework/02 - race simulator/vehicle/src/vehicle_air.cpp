/* vehicle_air.cpp */

#include "vehicle_air.h"

namespace vehicles
{
	/* Carpet */
	template Vehicle_air<types::Carpet<double>>::Vehicle_air(double speed);
	template std::string Vehicle_air<types::Carpet<double>>::name() const;
	template double Vehicle_air<types::Carpet<double>>::race_time(double distance) const;
	
	/* Eagle */
	template Vehicle_air<types::Eagle<double>>::Vehicle_air(double speed);
	template std::string Vehicle_air<types::Eagle<double>>::name() const;
	template double Vehicle_air<types::Eagle<double>>::race_time(double distance) const;
	
	/* Broom */
	template Vehicle_air<types::Broom<double>>::Vehicle_air(double speed);
	template std::string Vehicle_air<types::Broom<double>>::name() const;
	template double Vehicle_air<types::Broom<double>>::race_time(double distance) const;

	template<class T>
		requires Type_air<T>
	Vehicle_air<T>::Vehicle_air(double speed) :
		speed_{ speed }
	{
	}

	template<class T>
		requires Type_air<T>
	std::string Vehicle_air<T>::name() const {
		return
			T::name;
	}

	template<class T>
		requires Type_air<T>
	double Vehicle_air<T>::race_time(double distance) const
	{
		const double coeff{ (100.0 - T::influence_param(distance)) / 100.0 };
		return
			distance * coeff / speed_;
	}
}