#pragma once

#include "Exposable.hpp"

#include <type_traits>

namespace JsonParserUtils
{
	template <typename T>
	static constexpr bool is_exposable_v =
		std::is_base_of<Exposable<T>, T>::value;
}
