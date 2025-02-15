// matter_cell.h

#ifndef NUCLEUS_H
#define NUCLEUS_H

#include <utility>
#include <vector>

namespace space {

	using std::vector;
	using _place = std::pair<size_t, size_t>;
	
	class Nucleus {
	public:
		enum class Status : bool { live = true, dead = false };
		using Value_type = vector<_place>;

		Nucleus();
		Nucleus(const _place&, Status);
		Nucleus(const _place&, Status, const Value_type&);
		Nucleus(const Nucleus&);
		Nucleus(Nucleus&&) noexcept;
		~Nucleus();

		Nucleus& operator=(const Nucleus&);
		Nucleus& operator=(Nucleus&&) noexcept;
		
		Value_type* neighbours() const;
		void status(const Nucleus::Status);
		Status status() const;
		
		_place place() const;
		bool is_life() const;
		
	private:
		void resize(size_t);

	private:
		Status status_;
		_place place_;
		Value_type* neighbours_;
	};
}
#endif /* NUCLEUS_H */