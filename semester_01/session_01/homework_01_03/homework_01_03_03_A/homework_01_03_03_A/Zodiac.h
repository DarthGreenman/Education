#pragma once
#ifndef ZODIAC_H
#define ZODIAC_H

#include <string>

#include "Pair.h"

class Zodiac {
public:
	enum class Nature_key { Fire = 1, Earth, Air, Water };
	enum class Sign_key {
		Aries = 1, Taurus, Gemini, Cancer, Lion, Virgo,
		Libra, Scorpion, Sagittarius, Capricorn, Aquarius, Fish
	};
	typedef Pair<Sign_key, std::string> Sign;
	typedef Pair<Nature_key, std::string> Nature;

	Zodiac() = default;
	Zodiac(const Sign& sign, const Nature& nature)
		: sign_{ sign }, nature_{ nature } {
	}
	Zodiac(const Zodiac& zodiac) noexcept {
		sign_ = zodiac.sign_;
		nature_ = zodiac.nature_;
	}
	Zodiac(const Zodiac&&) = delete;
	~Zodiac() {
	}

	Zodiac& operator=(const Zodiac&) = delete;

	void set_sign(const Sign& sign) {
		sign_ = sign;
	}
	void set_nature(const Nature& nature) {
		nature_ = nature;
	}

	const Sign& get_sign() const {
		return sign_;
	}
	const Nature& get_nature() const {
		return nature_;
	}

	const bool is_defined() const {
		bool res{ false };
		return res;
	}
private:
	Sign sign_;
	Nature nature_;
};
#endif /*ZODIAC_H*/

