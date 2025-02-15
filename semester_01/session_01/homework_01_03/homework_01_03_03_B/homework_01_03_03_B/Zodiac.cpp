#include "Zodiac.h"

Zodiac::Zodiac(const Zodiac::Sign& sign,
	const Zodiac::Nature& nature
)
	: sign_{ sign }, nature_{ nature } {
}

Zodiac::Zodiac(const Zodiac& zodiac) noexcept {
	sign_ = zodiac.sign_;
	nature_ = zodiac.nature_;
}

Zodiac::Zodiac() {
}

Zodiac::~Zodiac() {
}

void Zodiac::set_sign(const Zodiac::Sign& sign) {
	sign_ = sign;
}

void Zodiac::set_nature(const Zodiac::Nature& nature) {
	nature_ = nature;
}

const Zodiac::Sign& Zodiac::get_sign() const {
	return sign_;
}

const Zodiac::Nature& Zodiac::get_nature() const {
	return nature_;
}

const bool Zodiac::is_defined() const {
	return (static_cast<unsigned>(sign_.first) != NULL && !sign_.second.empty() &&
		static_cast<unsigned>(nature_.first) != NULL && !nature_.second.empty());
}