// matter_cell.cpp

#include "nucleus.h"

namespace space {
	
	Nucleus::Nucleus()
		: status_{ Nucleus::Status::dead }, place_{}, neighbours_{}
	{
		neighbours_ = new Value_type();
	}

	Nucleus::Nucleus(const _place& place, Status state)
		: status_{ state }, place_{ place }, neighbours_{}
	{
		neighbours_ = new Value_type();
	}
	
	Nucleus::Nucleus(const _place& place, Status state, const Value_type& next)
		: status_{ state }, place_{ place }, neighbours_{ new Value_type(next.size()) }
	{
		std::copy(next.cbegin(), next.cend(),
			this->neighbours()->begin()); /* copy(next); */
	}
	
	Nucleus::Nucleus(const Nucleus& other)
		: Nucleus{other.place(), other.status(), *other.neighbours()}
	{
	}

	Nucleus::Nucleus(Nucleus&& movable) noexcept
		: status_{ movable.status()}, place_{ movable.place() },
		neighbours_{ movable.neighbours() }
	{
		movable.status_ = Nucleus::Status::dead;
		movable.place_.first = 0; movable.place_.second = 0;
		movable.neighbours_ = nullptr;
	}
	
	Nucleus::~Nucleus()	
	{
		if (neighbours()) {
			delete neighbours_; neighbours_ = nullptr;
		}
	}
	
	Nucleus& Nucleus::operator=(const Nucleus& other)
	{
		if (this != &other) {
			place_ = other.place(); status_ = other.status();

			Value_type* _neighbours{ neighbours() },
				* Neighbours{ other.neighbours() };
			
			if (_neighbours->size() != Neighbours->size()) {
				resize(Neighbours->size());
			}
			std::copy(Neighbours->begin(), Neighbours->end(),
				neighbours()->begin()); /* copy(*Next); */
		}
		return *this;
	}

	Nucleus& Nucleus::operator=(Nucleus&& movable) noexcept
	{
		if (this != &movable) {
			delete neighbours_;
			status_ = movable.status(); movable.status_ = Nucleus::Status::dead;
			place_ = movable.place();
			movable.place_.first = 0; movable.place_.second = 0;
			neighbours_ = movable.neighbours(); movable.neighbours_ = nullptr;
		}
		return *this;
	}

	Nucleus::Value_type* Nucleus::neighbours() const {
		return neighbours_ != nullptr ? neighbours_ : nullptr;
	}

	void Nucleus::status(const Nucleus::Status state)	{
		status_ = state;
	}
	
	Nucleus::Status Nucleus::status() const {
		return status_;
	}

	_place Nucleus::place() const	{
		return place_;
	}

	bool Nucleus::is_life() const	{
		return static_cast<bool>(status_);
	}

	void Nucleus::resize(size_t size)
	{
		if (this->neighbours()) {
			delete neighbours_;
			neighbours_ = new Value_type(size);
		}
	}
}