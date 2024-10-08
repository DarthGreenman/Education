/* Exception.h */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

namespace bag {
    class Bad_length :
        public std::logic_error {
    public:
        Bad_length(const std::string& message) :
            std::logic_error(message) {}

        ~Bad_length() {}

        const char* what() const noexcept override {
            return std::logic_error::what();
        }
    };
}
#endif /* EXCEPTION_H */