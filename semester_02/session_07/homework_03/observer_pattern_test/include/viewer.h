/// viewer.h

#ifndef VIEWER_H_IN_MY_PROJECT
#define VIEWER_H_IN_MY_PROJECT

#include <fstream>
#include <ios>
#include <iostream>
#include <string>

namespace pattern
{
	struct viewer
	{
		virtual ~viewer() = default;
		virtual void warning(const std::string& message) = 0;
		virtual void error(const std::string& message) = 0;
		virtual void fatal(const std::string& message) = 0;
	}; /// struct viewer

	class journal : public viewer
	{
	public:
		journal() = delete;
		journal(const std::string& file_name)
		{
			_file.open(file_name);
			if (!_file.is_open())
				throw std::ios_base::failure{ "Error opening log file" };
		}
		virtual ~journal() = default;
		void warning(const std::string& message) override final
		{
			_file << __FUNCTION__ << '\t' << message << '\n';
			_file.flush();
		}
		void error(const std::string& message) override final
		{
			_file << __FUNCTION__ << '\t' << message << '\n';
			_file.flush();
		}
		void fatal(const std::string& message) override final
		{
			_file << __FUNCTION__ << '\t' << message << '\n';
			_file.flush();
		}

	private:
		std::ofstream _file{};

	}; /// class journal

	class console : public viewer
	{
	public:
		console() = default;
		console(std::ostream& os) : _os{ os } {}
		virtual ~console() = default;

		void warning(const std::string& message) override final
		{
			_os << __FUNCTION__ << '\t' << message << '\n';
		}
		void error(const std::string& message) override final
		{
			_os << __FUNCTION__ << '\t' << message << '\n';
		}
		void fatal(const std::string& message) override final
		{
			_os << __FUNCTION__ << '\t' << message << '\n';
		}

	private:
		std::ostream& _os{ std::cout };

	}; /// class console
} /// namespace pattern

#endif // !VIEWER_H_IN_MY_PROJECT
