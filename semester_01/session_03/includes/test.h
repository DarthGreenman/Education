#pragma once

#include <stdexcept>

namespace test
{
	inline void test_bad_alloc()
	{
		throw std::bad_alloc();
	}
}
