/* text_analyser.h */

#ifndef TEXT_ANALYSER_H
#define TEXT_ANALYSER_H

#include <assert.h>
#include <cctype>
#include <initializer_list>
#include <iterator>
#include <map>
#include <string>
#include <type_traits>

namespace seq
{
	enum class Token_types : char { alfa, punct, space };

	struct Token 
	{
		Token_types type;
		char symbol{};
		static const char* name(Token_types Type)
		{
			const auto name = Type == Token_types::alfa ? "Буква" :
				Type == Token_types::punct ? "Знак пунктуации" : "Пробельный символ";
			return name;
		}
	};

	bool operator<(const Token& lhs, const Token& rhs) {
		return lhs.symbol < rhs.symbol; }

	class Text_analyser
	{
	public:
		using Original_text		= typename std::string;
		using Text_tokens		= typename std::map<Token, size_t>;
		using Types_text_tokens	= typename std::map<Token_types, size_t>;

		Text_analyser() = delete;
		Text_analyser(const std::string& Copied) :
			Text_analyser(std::cbegin(Copied), std::cend(Copied))
		{
			counter_init();
		}

		Text_analyser(std::string&& Movable) :
			Raw_text_{ std::move(Movable) }
		{
			counter_init();
		}

		template<class InputIt>
		Text_analyser(InputIt First, InputIt Last)
		{
			Raw_text_.reserve(Last - First);
			std::copy(First, Last, std::back_inserter(Raw_text_));
			counter_init();
		}

		Text_tokens operator()()
		{
			Text_tokens map{};
			/* any_char, always_equal - "заглушки" (только для данного метода)
			 * для предоставления инструкции Cmp(Elem, elem) постоянных значений */
			const auto any_char = [](char ch = 'a') ->decltype(ch)
				{ return char{ch}; };
			const auto always_equal = [](char lhs, char rhs) ->decltype(lhs == rhs)
				{ lhs = rhs; return lhs == rhs; };
			prepare(any_char(), map, always_equal);
			return map;
		}

		Text_tokens operator()(const std::initializer_list<char>& List)
		{
			Text_tokens map{};
			for (const auto& elem : List) {
				prepare(elem, map);
			}
			return map;
		}

		Types_text_tokens statistics() const { return Types_counter_; }

	private:
		template<class Compare = std::equal_to<>>
		void prepare(char Elem, Text_tokens& Map, Compare Cmp = Compare{})
		{
			for (const auto& elem : Raw_text_) {
				if (Cmp(Elem, elem)) {
					const auto count = std::count(std::cbegin(Raw_text_), 
						std::cend(Raw_text_), elem);
					Map.emplace(make_token(elem), count);
				}
			}
		}

		Token make_token(char Ch)
		{
			assert(is_correct(Ch) && "В образце текста присутсвуют специальные символы: "
				"'\n', '\r', '\t', '\v', '\f'");
			Token_types type{};
			std::isalpha(Ch) != 0 ? ++Types_counter_.at((type = Token_types::alfa)) : 
				std::ispunct(Ch) != 0 ? ++Types_counter_.at( (type = Token_types::punct)) :
					++Types_counter_.at((type = Token_types::space));

			return Token{ type, Ch };
		}

		bool is_correct(char Ch) {
			return Ch != '\n' && Ch != '\r' && Ch != '\t' && Ch != '\v' && Ch != '\f';
		}

		void counter_init()
		{
			Types_counter_.emplace(Token_types::alfa,	0);
			Types_counter_.emplace(Token_types::punct,	0);
			Types_counter_.emplace(Token_types::space,	0);
		}

	private:
		Original_text		Raw_text_{};
		Types_text_tokens	Types_counter_;
	};
}

#endif /* TEXT_ANALYSER_H */