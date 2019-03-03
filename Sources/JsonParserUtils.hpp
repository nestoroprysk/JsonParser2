#pragma once

#include <string>
#include <vector>
#include <list>

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
		template <typename T>
		static constexpr auto is_one_of_supported_containers(std::list<T> const&) -> std::true_type;
		static constexpr auto is_one_of_supported_containers(...) -> std::false_type;
	}

	template <typename T>
	static constexpr bool is_container_v =
		decltype(ContainerDetail::is_one_of_supported_containers(std::declval<T&>()))::value;

	template <typename T>
	using element_type_t = std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;

	template <typename T>
	auto assertExposable() -> void
	{
		static_assert(JsonParserUtils::is_exposable_v<T>,
			"A class should inherit from Exposable<T> templated on itself so as to be parsed");
		static_assert(JsonParserUtils::has_function_expose_v<T>,
			"A class should define a static function expose() so as to be parsed");
	}

	inline std::string ignoreTagError(std::string const& t)
	{
		return std::string("Warning: tag [" + t + "] ignored.");
	}

	namespace MoveVectorIntoDetail
	{
		template <typename T, typename M>
		auto append_move_from_to(std::vector<T>&& src, M& dst) -> void
		{
			std::copy(std::make_move_iterator(src.begin()),
				std::make_move_iterator(src.end()), std::back_inserter(dst));
		}
	}

	template <typename T>
	auto move_vector_into(std::vector<element_type_t<T>>&& src) -> T
	{
		T result;
		MoveVectorIntoDetail::append_move_from_to(std::move(src), result);
		return result;
	}
}
