/* address.cpp */

#include "address.h"

namespace location {
	address::address() :
		city_{}, street_{}, house_{}, apartment_{}
	{
	}

	address::address(const string& city, const string& street,
		unsigned house, unsigned apartment) :
		city_{ city }, street_{ street }, house_{ house },
		apartment_{ apartment }
	{
	}

	address::~address()
	{
	}

	const string address::get_address() const
	{
		return city_ + ", " + 
			street_ + ", " + 
			std::to_string(house_) + ", " +
			std::to_string(apartment_);
	}

	bool operator<(const address& lhs, const address& rhs)
	{
		return lhs.city_ < rhs.city_;
	}
	
	bool operator>(const address& lhs, const address& rhs)
	{
		return lhs.city_ > rhs.city_;
	}
	bool operator==(const address& lhs, const address& rhs)
	{
		return lhs.city_ == rhs.city_;
	}
	bool operator!=(const address& lhs, const address& rhs)
	{
		return lhs.city_ != rhs.city_;
	}
}