/// decorator_text.h

#ifndef DECORATOR_TEXT_H_IN_MY_PROJECT
#define DECORATOR_TEXT_H_IN_MY_PROJECT

#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

class Text
{
public:
	virtual void render(const std::string& data) const
	{
		std::cout << data;
	}
}; /// class Text

class DecoratedText : public Text
{
public:
	friend class ItalicText;
	friend class BoldText;
	friend class Paragraph;
	friend class Reversed;
	friend class Link;

	explicit DecoratedText(std::unique_ptr<Text> text)
		: text_{ std::move(text) } {
	}
private:
	std::unique_ptr<Text> text_{};
}; /// class DecoratedText

class ItalicText : public DecoratedText
{
public:
	explicit ItalicText(std::unique_ptr<Text> text)
		: DecoratedText{ std::move(text) } {
	}
	void render(const std::string& data) const
	{
		std::cout << "<i>";
		text_->render(data);
		std::cout << "</i>";
	}
}; /// class ItalicText

class BoldText : public DecoratedText
{
public:
	explicit BoldText(std::unique_ptr<Text> text) 
		: DecoratedText{ std::move(text) } {
	}
	void render(const std::string& data) const
	{
		std::cout << "<b>";
		text_->render(data);
		std::cout << "</b>";
	}
}; /// class BoldText

class Paragraph : public DecoratedText
{
public:
	explicit Paragraph(std::unique_ptr<Text> text)
		: DecoratedText{ std::move(text) } {
	}
	void render(const std::string& data) const
	{
		std::cout << "<p>";
		text_->render(data);
		std::cout << "</p>";
	}
}; /// class Paragraph

class Reversed : public DecoratedText
{
public:
	explicit Reversed(std::unique_ptr<Text> text)
		: DecoratedText{ std::move(text) } {
	}
	void render(const std::string& data) const
	{
		std::string text{};
		text.reserve(std::size(data));
		std::copy(std::crbegin(data), std::crend(data), 
			std::back_inserter(text));
		text_->render(std::move(text));
	}
}; /// class Reversed

class Link : public DecoratedText
{
public:
	explicit Link(std::unique_ptr<Text> text)
		: DecoratedText{ std::move(text) } {
	}
	void render(const std::string& ref, const std::string& data) const
	{
		std::cout << "<a href=" << ref <<'>';
		text_->render(data);
		std::cout << "</a>";
	}
}; /// class Link

#endif // !DECORATOR_TEXT_H_IN_MY_PROJECT