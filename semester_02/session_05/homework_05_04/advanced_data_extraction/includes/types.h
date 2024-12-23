// types.h

#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace my
{
	struct person_name
	{
		std::string forename{};
		std::string surname{};
	};

	// mailbox - является пользовательским аккаунтом, приложением или аккаунтом системной роли,
	// может содержать информацию для дальнейшей маршрутизации или идентификаторы используемые
	// для сортировки, автоматизации или отслеживания
	// максимвльное количество символов - 64
	// hostname — доменн, субдоменн, сервер, сервис, ip - адрес, хост
	// максимвльное количество символов - 253 с учетом 2-х точек
	enum params { mailbox = 0, hostname, first_param = params::mailbox, last_param = params::hostname };
	params& operator++(params& param);
	params operator++(params& param, int);
	params beg_params();
	params end_params();

	class email_address
	{
	public:
		using string = typename std::string;

		email_address() = default;
		email_address(const string& address);
		email_address(const email_address&) = default;
		email_address(email_address&&) = default;
		~email_address() = default;

		email_address& operator=(const email_address&) = default;
		email_address& operator=(email_address&&) = default;

		std::pair<string, string> get() const;
		static bool is_email_address(const string& address);

	private:
		void check(const std::vector<string>& parts) const;
		void check(const std::vector<string>& parts, params param) const;

	private:
		std::pair<string, string> address_{};
	};

	class phone_number
	{
	public:
		using string = typename std::string;

		phone_number() = default;
		phone_number(const string& number);
		phone_number(const phone_number&) = default;
		phone_number(phone_number&&) = default;
		~phone_number() = default;

		phone_number& operator=(const phone_number&) = default;
		phone_number& operator=(phone_number&&) = default;

		std::string normalization() const;
		std::tuple<string, string, string, string> get() const;
		static bool is_phone_number(const string& number);

	private:
		std::string normalization(const string& number) const;
		void check(const string& normalized_number) const;
	
	public:
		// Североамериканский план нумерации
		enum size_codes { country = 2, zone = 3, node = 3, number = 4 };
	
	private:
		// code_[0] - код страны
		// code_[1] - код зоны, допустимые значения: [2-9] для первой цифры, [0-9] - для второй и третьей
		// code_[2] - код узла, допустимые значения: [2-9] для первой цифры, [0-9] - для второй и третьей
		// code_[3] - уникальный 4-значный номер в пределах телефонного узла, [0-9] для каждой из цифр
		std::vector<string> codes_{};
	};
}

#endif // !TYPES_H
