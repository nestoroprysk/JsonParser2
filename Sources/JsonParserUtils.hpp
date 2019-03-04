#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <functional>
#include <optional>

template <typename T>
class Exposable;

#define RETURNS(...) \
	noexcept(noexcept(__VA_ARGS__)) \
	-> decltype( __VA_ARGS__ ) \
	{ return __VA_ARGS__; }

#define OVERLOADS_OF(...) \
	[](auto&&...args) \
	RETURNS( __VA_ARGS__( decltype(args)(args)... ))

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

	namespace OrdContainerDetail
	{
		template <typename T>
		static constexpr auto is_one_of_supported_ord_containers(std::vector<T> const&) -> std::true_type;
		template <typename T>
		static constexpr auto is_one_of_supported_ord_containers(std::list<T> const&) -> std::true_type;
		static constexpr auto is_one_of_supported_ord_containers(...) -> std::false_type;
	}

	template <typename T>
	static constexpr bool is_ord_container_v =
		decltype(OrdContainerDetail::is_one_of_supported_ord_containers(std::declval<T&>()))::value;

	template <typename T>
	using element_type_t = std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;

	template <typename T>
	auto exposable() -> void
	{
		static_assert(JsonParserUtils::is_exposable_v<T>,
			"A class should inherit from Exposable<T> templated on itself so as to be parsed");
		static_assert(JsonParserUtils::has_function_expose_v<T>,
			"A class should define a static function expose() so as to be parsed");
	}

	template <typename T>
	auto defaultConstructable() -> void
	{
		static_assert(std::is_default_constructible_v<T>,
			"A class should be default constructable to be parsed");
	}

	inline std::string ignoreTagError(std::string_view t)
	{
		auto r = std::string();
		r += "Warning: tag [";
		r += t;
		r += "] ignored.";
		return r;
	}

	inline std::string parseError(std::string_view c)
	{
		auto r = std::string();
		r += "Warning: failed to extract value from [";
		r += std::string(c);
		r += "]";
		return r;
	}

	namespace MoveVectorIntoDetail
	{
		template <typename T, typename M>
		auto append_move_from_to_ord(std::vector<T>&& src, M& dst) -> void
		{
			std::copy(std::make_move_iterator(src.begin()),
				std::make_move_iterator(src.end()), std::back_inserter(dst));
		}
	}

	template <typename T>
	auto move_vector_into_ord(std::vector<element_type_t<T>>&& src) -> T
	{
		T result;
		MoveVectorIntoDetail::append_move_from_to_ord(std::move(src), result);
		return result;
	}

	template <typename T>
	auto safelyConvertedArithmetic(char const* s, std::function<T(char const*)> const& f) -> std::optional<T>
	{
		try{
			return f(s);
		}
		catch (std::out_of_range const&){
			return {};
		}
		catch (std::invalid_argument const&){
			return {};
		}
	}

	namespace ExtractedArithmeticDetail
	{
		template <typename T>
		auto nbLen(T n) -> int
		{
			static_assert(std::is_integral_v<T>, "An integral type expected");
			if (n == 0) return 1;
			int result = 0;
			while (n != 0){
				n /= 10;
				++result;
			}
			return result;
		}
	}

	template <typename T>
	auto extractedArithmetic(std::string_view s, int i, std::function<T(char const*)> const& f) -> std::optional<std::pair<std::string_view, int>>
	{
		if (i >= s.size()) return {};
		auto const opt_r = safelyConvertedArithmetic(&s[i], f);
		if (!opt_r) return {};
		auto const len = ExtractedArithmeticDetail::nbLen(opt_r.value());
		auto const r = std::string_view(&s[i], len);
		return std::pair<std::string_view, int>{r, i + r.size()};
	}
}
