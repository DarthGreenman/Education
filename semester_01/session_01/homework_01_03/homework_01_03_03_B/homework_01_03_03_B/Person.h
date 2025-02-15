#pragma once
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <map>

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
	
	typedef std::pair<Gender_key, std::string> Gender;

	Person(const Person&&) = delete;
	Person& operator= (const Person&) = delete;

	Person(const std::string&, const Gender&, unsigned, Zodiac&);
	~Person();
	
	const Gender& get_gender() const;
	const unsigned get_age() const;
	const Zodiac& get_zodiac() const;

private:
	std::string name_;
	Gender gender_;
	unsigned age_;
	Zodiac zodiac_;
};
#endif /*PERSON_H*/
