// homework_04_04_01.cpp: определяет точку входа для приложения.
//

#include "homework_04_04_01.h"
#include "list.h"

TEST_CASE("Testing methods of the netology::List class", "[methods: Empty(), Size(), Clear()]")
{
	netology::List list;

	SECTION("Testing methods Empty()")
	{
		INFO("For an empty list, the Empty() method should return true, "
			"otherwise it should return false");
		CHECK(list.Empty() == true);
		list.PushBack(100);
		CHECK(list.Empty() == false);
	}

	SECTION("Testing methods Size()")
	{
		INFO("For an empty list, the Size() method should return 0, "
			"otherwise it should return a value greater than 0");
		CHECK(list.Size() == 0);
		list.PushBack(100);
		CHECK(list.Size() == 1);
	}

	SECTION("Testing methods Clear()")
	{
		list.PushBack(100);
		list.Clear();
		INFO("After calling the Clear() method, "
			"the Empty() method should return true and Size() should return 0");
		CHECK(list.Empty() == true);
		CHECK(list.Size() == 0);
	}
}

int main()
{
	return Catch::Session().run();
}