/* Greeter.h */

#ifndef GREETER_H
#define GREETER_H

#include <string>

namespace bag {
	class Greeter {
	public:
		Greeter(const std::string & = "Здравствуйте, ");
		~Greeter();

		const std::string greet(const std::string&) const;

	private:
		std::string message_{};
	};
}

#endif /* GREETER_H */