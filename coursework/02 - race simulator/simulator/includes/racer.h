/* racer.h */

#ifndef RACE_SIMULATOR_RACERS_H
#define RACE_SIMULATOR_RACERS_H

#include "vehicle.h"

#include <memory>
#include <vector>
#include <string>

namespace race
{
    std::unique_ptr<vehicles::Vehicle> make_racer(char bar_key);
}
#endif /* RACE_SIMULATOR_RACERS_H */