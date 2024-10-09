#include "Person.h"

Person::Person(const std::string& name,
	const Person::Gender& gender,
	unsigned age,
	Zodiac& zodiac
)
	: name_{ name }, gender_{ gender },	age_{ age }, zodiac_{ zodiac } {
}

Person::~Person() {
}

const Person::Gender& Person::get_gender() const {
	return gender_;
}

const unsigned Person::get_age() const {
	return age_;
}

const Zodiac& Person::get_zodiac() const {
	return zodiac_;
}