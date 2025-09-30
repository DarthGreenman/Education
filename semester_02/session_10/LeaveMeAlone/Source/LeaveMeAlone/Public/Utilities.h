#pragma once

namespace Helper
{
template <typename Type> bool IsValid(const Type* Value)
{
	return Value != nullptr ? true : false;
}

} // namespace Helper