// homework_02_02_02.cpp

#include <iostream>
#include <string>

#include <math.h>

#include "localisation.h"

namespace bank {
    struct account {
        std::string account_holder_name;
        int account_number;
        double balance;
        account(const std::string name, int account, double sum)
            : account_holder_name{ name }, account_number{ account },
            balance{ sum } {
        }
    };

    void change_account_balance(account&, double);
    void view_account(const account&);
    int get_input_account(int, const std::string&,
        const std::string & = "Введите правильное цифровое значение: "
    );
}

template <typename T>
T get_input_value(const std::string & =
    "Введите правильное цифровое значение: "
);

int main()
{
    bag::console_localisation localisation{ 1251 };
    using std::cout;
    using std::cin;

    try {
        cout << "Введите 9-и значный номер счёта: ";
        int account{ bank::get_input_account(9,
            "Введите 9-и значное число: ") };

        cout << "Введите имя владельца: ";
        std::string name;
        cin >> name;

        cout << "Введите баланс: ";
        double sum{ get_input_value<double>() };

        bank::account visitor{ name, account, sum };

        cout << "Введите новый баланс: ";
        bank::change_account_balance(visitor,
            get_input_value<double>());
        
        bank::view_account(visitor);
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
    }
    return 0;
}

namespace bank {
    void change_account_balance(account& owner_account, double sum) {
        owner_account.balance = sum;
    }
    void view_account(const account& owner_account) {
        std::cout << "Ваш счёт: " << owner_account.account_holder_name << ", "
            << owner_account.account_number << ", "
            << owner_account.balance;
    }
    int get_input_account(int number_length, const std::string& message_1,
        const std::string& message_2
    )
    {
        int account{ get_input_value<int>(message_2) };
        for (; static_cast<int>(log10(account) + 1) != number_length; ) {
            std::cout << message_1;
            account = get_input_value<int>(message_2);
        }
        return account;
    }
}

template <typename T>
T get_input_value(
    const std::string& message
)
{
    while (true) {
        T result{};
        std::cin >> result;
        if (std::cin.good()) {
            return result;
        }
        else if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(),
                '\n');
            std::cout << message;
        }
        else /* if (std::cin.bad()) */ {
            throw std::exception{ "\nПроизошла фатальная ошибка!\n" };
        }
    }
}