// homework_04_06_01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "lib/localisation.h"
#include "lib/text_analyser.h"

#include <algorithm>
#include <cstdlib>
#include <iomanip> 
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace helper
{
    void print(const std::map<seq::Token, size_t>& Map);
    void print(const std::vector<std::pair<seq::Token, size_t>>& Vector);
    template<class InputIt>
    void print(const InputIt First, const InputIt Last);
    
    std::vector<std::pair<seq::Token, size_t>> 
        map_to_vector(const std::map<seq::Token, size_t>& Map);
}

int main()
{
    const bag::Console_localisation locale{ bag::Console_localisation::Code_pages::rus };
    using std::cout;

    std::string message{ 
        "std::map is a sorted associative container that contains key - value pairs "
        "with unique keys. Keys are sorted by using the comparison function compare, "
        "search, removal, and insertion operations have logarithmic complexity." 
    };
    
    const auto cmp =
        [](const std::pair<seq::Token, size_t>& lhs, const std::pair<seq::Token, size_t>& rhs)
        { return lhs.second > rhs.second; };
    
    cout << "Анализ (сортировка по количеству вхождений) всех символов в предложении:\n" << message << '\n';
    seq::Text_analyser text{ std::move(message) };
    auto analysing_all_characters = text();    
    /* Сортировка по количеству вхождений */
    auto sorted = helper::map_to_vector(analysing_all_characters);
    std::sort(std::begin(sorted), std::end(sorted), cmp);
    helper::print(sorted);
         
    cout << "\nАнализ символов из списка: 'e', 'a', 'C', '+', '.', ',', ':', 'K'\n";
    auto analysing_specified_characters = text({ 'e', 'a', 'C', '+', '.', ',', ':', 'K' });
    /* Сортировка по количеству вхождений */
    sorted = helper::map_to_vector(analysing_specified_characters);
    std::sort(std::begin(sorted), std::end(sorted), cmp);
    helper::print(sorted);
   
    cout << "\nСтатистика количества вхождений типов символов .\n";
    const auto state_token_types = text.statistics();
    for (const auto& elem : state_token_types) {
        auto&& [token_type, count] = elem;
        cout << seq::Token::name(token_type) << ": " << count << '\n';
    }

    cout << '\n';
    std::system("pause");
    return 0;
}

namespace helper
{

    void print(const std::map<seq::Token, size_t>& Map)
    {
        print(std::cbegin(Map), std::cend(Map));
    }

    void print(const std::vector<std::pair<seq::Token, size_t>>& Vector)
    {
        print(std::cbegin(Vector), std::cend(Vector));
    }

    template<class InputIt>
    void print(const InputIt First, const InputIt Last)
    {
        using std::pair;
        using seq::Token;
        constexpr unsigned margin_width{ 3 };
        for (auto first = First; first != Last; ++first) {
            std::cout << std::setw(margin_width) << '-';
        };
        std::cout << '\n'; 
        std::for_each(First, Last, [](const pair<Token, size_t>& elem) {
                std::cout << std::setw(margin_width) << elem.first.symbol; });
        std::cout << '\n';
        std::for_each(First, Last, [](const pair<Token, size_t>& elem) {
                std::cout << std::setw(margin_width) << elem.second; });
        std::cout << '\n';
    }

    std::vector<std::pair<seq::Token, size_t>> 
        map_to_vector(const std::map<seq::Token, size_t>& Map)
    {
        std::vector<std::pair<seq::Token, size_t>> vec{};
        vec.reserve(Map.size());
        std::copy(std::cbegin(Map), std::cend(Map), 
            std::back_inserter(vec));
        return vec;
    }
    
}