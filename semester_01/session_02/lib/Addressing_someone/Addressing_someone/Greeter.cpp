/* Greeter.cpp */

#include "Greeter.h"

namespace bag
{
	Greeter::Greeter(const std::string& hello) :
		message_{ hello } {}

	Greeter::~Greeter() {}

	const std::string Greeter::greet(const std::string& name) const {
		return message_ + name;
	}
}