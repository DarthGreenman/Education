/* vehicle_land.cpp */

#include "vehicle_land.h"

namespace vehicles
{
	/* Bactrian */
	template Vehicle_land<types::Bactrian<size_t>>::Vehicle_land(double speed,
			double time_before_rest);
	template std::string Vehicle_land<types::Bactrian<size_t>>::name() const;
	template double Vehicle_land<types::Bactrian<size_t>>::race_time(double distance) const;

	/* Dromedar */
	template Vehicle_land<types::Dromedar<size_t>>::Vehicle_land(double speed,
			double time_before_rest);
	template std::string Vehicle_land<types::Dromedar<size_t>>::name() const;
	template double Vehicle_land<types::Dromedar<size_t>>::race_time(double distance) const;

	/* Centaur */
	template Vehicle_land<types::Centaur<size_t>>::Vehicle_land(double speed,
			double time_before_rest);
	template std::string Vehicle_land<types::Centaur<size_t>>::name() const;
	template double Vehicle_land<types::Centaur<size_t>>::race_time(double distance) const;

	/* Boots */
	template Vehicle_land<types::Boots<size_t>>::Vehicle_land(double speed,
			double time_before_rest);
	template std::string Vehicle_land<types::Boots<size_t>>::name() const;
	template double Vehicle_land<types::Boots<size_t>>::race_time(double distance) const;

	template<class T>
		requires Type_land<T>
	Vehicle_land<T>::Vehicle_land(double speed, double time_before_rest) :
		speed_{ speed },
		time_before_rest_{ time_before_rest }
	{
	}

	template<class T>
		requires Type_land<T>
	std::string Vehicle_land<T>::name() const {
		return
			T::name;
	}

	template<class T>
		requires Type_land<T>
	double Vehicle_land<T>::race_time(double distance) const
	{
		double time{ distance / speed_ };
		const size_t rest{ static_cast<size_t>(time / time_before_rest_) };

		for (size_t num{}; num < rest; ++num) {
			time += T::influence_param(num);
		}
		return time;
	}
}