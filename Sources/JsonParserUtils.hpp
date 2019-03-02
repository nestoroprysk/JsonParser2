#pragma once

#include <string>
#include <vector>

template <typename T>
class Exposable;

namespace JsonParserUtils
{
	template <typename T>
	static constexpr bool is_exposable_v =
		std::is_base_of<Exposable<T>, T>::value;
	
	namespace ExposableDetail
	{
		template <typename T, typename = decltype(std::declval<T&>().expose())> 
		static constexpr auto has_expose_function(T const&) -> std::true_type;
		static constexpr auto has_expose_function(...) -> std::false_type;
	}

	template <typename T>
	static constexpr bool has_function_expose_v =
		decltype(ExposableDetail::has_expose_function(std::declval<T&>()))::value;

	namespace ContainerDetail
	{
		template <typename T>
		static constexpr auto is_one_of_supported_containers(std::vector<T> const&) -> std::true_type;
		static constexpr auto is_one_of_supported_containers(...) -> std::false_type;
	}

	template <typename T>
	static constexpr bool is_container_v =
		decltype(ContainerDetail::is_one_of_supported_containers(std::declval<T&>()))::value;

	template <typename T>
	using element_type_t = std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;
}
