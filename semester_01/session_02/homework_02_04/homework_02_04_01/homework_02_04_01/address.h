/* address.h */

#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

namespace location {
	using std::string;

	class address {
	public:
		address() = delete;
		address(const string&, const string&, unsigned, unsigned);
		~address();

		const string get_address() const;
	private:
		string city_;
		string street_;
		unsigned house_;
		unsigned apartment_;
	};
}
#endif /* ADDRESS_H */