#include "Leaver.h"

namespace bag
{
	Leaver::Leaver(const std::string& goodbye) :
		message_{ goodbye } {}

	Leaver::~Leaver() {}

	const std::string Leaver::leave(const std::string& name) const {
		return message_ + name;
	}
}