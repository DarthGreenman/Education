/* "race.cpp */

#include "race.h"
#include <utility>

namespace race
{
	Race::Race(std::vector<std::unique_ptr<vehicles::Vehicle>> team,
		double distance) :
		team_{ std::move(team) }, distance_{ distance }
	{
	}
	
	void race::Race::start()
	{
        for (const auto& racer : team_) {
            result_.push_back(
                std::make_pair(racer.get()->name(),
                    racer.get()->race_time(distance_))
            );
        }
	}

	std::vector<std::pair<std::string, double>> Race::finish() const {
		return 
			result_;
	}
}