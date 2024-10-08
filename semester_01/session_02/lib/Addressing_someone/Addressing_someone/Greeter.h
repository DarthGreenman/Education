/* Greeter.h */

#ifndef GREETER_H
#define GREETER_H

#ifdef ADDRESSING_SOMEONE_EXPORT
#define ADDRESSING_SOMEONE_DLL __declspec(dllexport)
#else
#define ADDRESSING_SOMEONE_DLL __declspec(dllimport)
#endif

#include <string>

namespace bag {
	class Greeter {
	public:
		ADDRESSING_SOMEONE_DLL
			Greeter(const std::string & = "Здравствуйте, ");
		ADDRESSING_SOMEONE_DLL
			~Greeter();

		ADDRESSING_SOMEONE_DLL
			const std::string greet(const std::string&) const;

	private:
		std::string message_{};
	};
}

#endif /* GREETER_H */