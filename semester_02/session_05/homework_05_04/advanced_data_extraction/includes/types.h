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
		std::string forename_{};
		std::string surname_{};
	};

	// mailbox - �������� ���������������� ���������, ����������� ��� ��������� ��������� ����,
	// ����� ��������� ���������� ��� ���������� ������������� ��� �������������� ������������
	// ��� ����������, ������������� ��� ������������
	// ������������ ���������� �������� - 64
	// hostname � ������, ���������, ������, ������, ip - �����, ����
	// ������������ ���������� �������� - 253 � ������ 2-� �����
	enum params { mailbox = 0, hostname, count, first_param = params::mailbox, last_param = params::hostname };
	params& operator++(params& param);
	params operator++(params& param, int);
	params beg_params();
	params end_params();

	class email_address
	{
	public:
		email_address() = default;
		email_address(const std::string& address);
		email_address(const email_address&) = default;
		email_address(email_address&&) = default;
		~email_address() = default;

		email_address& operator=(const email_address&) = default;
		email_address& operator=(email_address&&) = default;

		std::pair<std::string, std::string> get() const;

	private:
		void check(const std::vector<std::string>& parts) const;
		void check(const std::vector<std::string>& parts, params param) const;

	private:
		std::pair<std::string, std::string> address_{};
	};

	class phone_number
	{
	public:
		phone_number() = default;
		phone_number(const std::string& number);
		phone_number(const phone_number&) = default;
		phone_number(phone_number&&) = default;
		~phone_number() = default;

		phone_number& operator=(const phone_number&) = default;
		phone_number& operator=(phone_number&&) = default;

		std::string normalization() const;
		std::tuple<std::string, std::string, std::string, std::string> get() const;

	private:
		std::string normalization(const std::string& number) const;
		void check(const std::string& normalized_number) const;
	
	public:
		// ������������������ ���� ���������
		enum size_codes { country = 2, zone = 3, node = 3, number = 4 };
	
	private:
		// code_[0] - ��� ������
		// code_[1] - ��� ����, ���������� ��������: [2-9] ��� ������ �����, [0-9] - ��� ������ � �������
		// code_[2] - ��� ����, ���������� ��������: [2-9] ��� ������ �����, [0-9] - ��� ������ � �������
		// code_[3] - ���������� 4-������� ����� � �������� ����������� ����, [0-9] ��� ������ �� ����
		std::vector<std::string> codes_{};
	};
}

#endif // !TYPES_H