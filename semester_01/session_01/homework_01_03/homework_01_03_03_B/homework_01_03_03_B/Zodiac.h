#pragma once
#ifndef ZODIAC_H
#define ZODIAC_H

#include <string>
#include <map>

class Zodiac {
public:
	enum class Nature_key {
		Fire = 1, Earth, Air, Water
	};
	enum class Sign_key {
		Aries = 1, Taurus, Gemini, Cancer, Lion, Virgo,
		Libra, Scorpion, Sagittarius, Capricorn, Aquarius, Fish
	};

	typedef std::pair<Sign_key, std::string> Sign;
	typedef std::pair<Nature_key, std::string> Nature;

	Zodiac(const Zodiac&&) = delete;
	Zodiac& operator=(const Zodiac&) = delete;

	Zodiac(const Sign&, const Nature&);
	Zodiac(const Zodiac&) noexcept;
	Zodiac();
	~Zodiac();	

	void set_sign(const Sign&);
	void set_nature(const Nature&);
	
	const Sign& get_sign() const;
	const Nature& get_nature() const;
	const bool is_defined() const;

private:
	Sign sign_;
	Nature nature_;
};
#endif /*ZODIAC_H*/
