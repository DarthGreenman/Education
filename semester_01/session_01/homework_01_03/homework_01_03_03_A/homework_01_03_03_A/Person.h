#pragma once
#ifndef PERSON_H
#define PERSON_H

#include <string>

#include "Zodiac.h"

class Person {
public:
	enum class Gender_key { 
		Female = 1, Male
	};
	enum class Age { 
		Newborns = 0, Teenager = 15, Young = 30, 
		Mature = 40, Longlived = 90
	};
	typedef Pair<Gender_key, std::string> Gender;

	Person(const std::string& name,const Gender& gender,
		unsigned age, const Zodiac& zodiac)
		: name_{ name }, gender_{ gender },
		age_{ age }, zodiac_{ zodiac } {
	}
	Person(const Person&& person) = delete;
	~Person() {
	}

	Person& operator= (const Person& person) = delete;

	const auto& get_gender() const {
		return gender_;
	}
	unsigned get_age() const {
		return age_;
	}
	const auto& get_zodiac() const {
		return zodiac_;
	}

private:
	std::string name_;
	Gender gender_;
	unsigned age_;
	Zodiac zodiac_;
};
#endif /*PERSON_H*/

