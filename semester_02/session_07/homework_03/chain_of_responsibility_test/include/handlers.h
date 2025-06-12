/// handlers.h

#ifndef HANDLERS_H_IN_MY_PROJECT
#define HANDLERS_H_IN_MY_PROJECT

#include "handler.h"
#include "logger.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>

namespace pattern
{
	namespace chain_of_responsibility
	{
		/// Обработчик предупреждения должен напечатать сообщение в консоль.
		class warning : public log_handler
		{
		public:
			warning() = default;
			warning(std::ostream& os) : _os{ os } {}
			virtual ~warning() = default;

		private:
			logger::level can_handler() const override final
			{
				return logger::level::warning;
			}
			void handle(const std::string& msg) override final
			{
				_os << __FUNCTION__ << ": " << msg << '\n';
			}

		private:
			std::ostream& _os{ std::cout };

		}; /// struct warning

		/// Обработчик обычной ошибки должен записать её в файл по указанному пути.
		class error : public log_handler
		{
		public:
			error() = delete;
			error(const std::string& file_name)
			{
				_file.open(file_name);
				if (!_file.is_open())
					throw std::ios_base::failure{ "Error opening log file" };
			}
			virtual ~error() = default;

		private:
			logger::level can_handler() const override final
			{
				return logger::level::error;
			}
			void handle(const std::string& msg) override final
			{
				_file << __FUNCTION__ << ": " << msg << '\n';
				_file.flush();
			}

		private:
			std::ofstream _file{};

		}; /// struct error

		/// Обработчик фатальной ошибки должен выбросить исключение с текстом сообщения, потому что
		/// предполагается, что программа должна прекратить выполнение после возникновения фатальной ошибки.
		class fatal : public log_handler
		{
		public:
			virtual ~fatal() = default;

		private:
			logger::level can_handler() const override final
			{
				return logger::level::fatal;
			}
			void handle(const std::string& msg) override final
			{
				using namespace std;
				throw runtime_error{ msg + " method: " + string{__FUNCTION__} +
					"\nfile: " + string{ __FILE__ } + "\nline: " + to_string(__LINE__) };
			}
		}; /// struct fatal
	} /// namespace chain_of_responsibility
} /// namespace pattern

#endif // !HANDLERS_H_IN_MY_PROJECT

