// homework_04_04_02.cpp: определяет точку входа для приложения.

#include "homework_04_04_02.h"
#include "list.h"

#include <stdexcept>

TEST_CASE("Testing methods of the netology::List class",
	"[methods: PushBack(int), PushFront(int), PopBack(), PopFront()]")
{
	netology::List list;

	SECTION("Testing methods PushBack(int)")
	{
		INFO("After each insertion of an element at the end, "
			"the size of the container must be increased by one unit");

		for (size_t i{}, count{ 10 }; i < count; ++i)
		{
			list.PushBack(1);
			CHECK(list.Size() == (i + 1));
		}
	}

	SECTION("Testing methods PushFront(int)")
	{
		INFO("After each insertion of an element at the beginning, "
			"the size of the container must be increased by one unit");
		
		for (size_t i{}, count{ 10 }; i < count; ++i)
		{
			list.PushFront(1);
			CHECK(list.Size() == (i + 1));
		}
	}

	SECTION("Testing methods PopBack()")
	{
		INFO("After each paired insertion and removal of a container, "
			"its size should not change");
		
		const size_t size{ list.Size() };
		constexpr size_t count{ 5 };
		for (size_t i{}; i < count; ++i) 
		{
			list.PushBack(1);
		}
		for (size_t i{}; i < count; ++i) 
		{
			list.PopBack();
		}
		CHECK(list.Size() == size);
	}

	SECTION("Testing methods PopFront()")
	{
		INFO("After each paired insertion and removal of a container, "
			"its size should not change");

		const size_t size{ list.Size() };
		constexpr size_t count{ 5 };
		for (size_t i{}; i < count; ++i) 
		{
			list.PushFront(1);
		}
		for (size_t i{}; i < count; ++i) 
		{
			list.PopFront();
		}
		CHECK(list.Size() == size);
	}

	SECTION("Testing the PopBack() method on an empty list")
	{
		CHECK_THROWS_AS(list.PopBack(), std::out_of_range);
	}

	SECTION("Testing the PopFront() method on an empty list")
	{
		CHECK_THROWS_AS(list.PopFront(), std::out_of_range);
	}

	SECTION("A more complex scenario for the use of methods")
	{
		REQUIRE(list.Empty() == true);
		if (list.Empty()) {
			list.PushFront(100);
			CHECK(list.Size() == 1);
			list.PushBack(50);
			CHECK(list.Size() == 2);
		}
		list.PopBack();
		list.PopFront();
		CHECK(list.Empty() == true);
	}	
}

int main()
{
	return Catch::Session().run();
}