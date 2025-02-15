/* race.h */

#ifndef RACE_SIMULATOR_RACE_H
#define RACE_SIMULATOR_RACE_H

#ifdef RACE_LIBRARY_EXPORT
#define RACE_LIBRARY_DLL __declspec(dllexport)
#else
#define RACE_LIBRARY_DLL __declspec(dllimport)
#endif

#include "vehicle.h"

#include <vector>
#include <memory>

namespace race
{
	class Race
	{
	public:
		Race() = default;
		RACE_LIBRARY_DLL 
			Race(std::vector<std::unique_ptr<vehicles::Vehicle>> team, 
				double distance);
		~Race() = default;

	public:
		RACE_LIBRARY_DLL void start();
		RACE_LIBRARY_DLL 
			std::vector<std::pair<std::string, double>> finish() const;

	private:
		std::vector<std::unique_ptr<vehicles::Vehicle>> team_{};
		std::vector<std::pair<std::string, double>> result_{};
		double distance_{};
	};
}
#endif /* RACE_SIMULATOR_RACE_H */