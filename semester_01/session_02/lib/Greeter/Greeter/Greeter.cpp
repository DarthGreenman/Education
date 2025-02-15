/* Greeter.cpp */

#include "Greeter.h"

bag::Greeter::Greeter(const std::string& hello) :
	message_{ hello }
{
}

bag::Greeter::~Greeter() {}

const std::string bag::Greeter::greet(const std::string& name) const {
	return message_ + name;
}