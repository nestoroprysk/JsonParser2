#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>

class JsonParser
{
public:
	template <typename T>
	static auto parsedObject(std::string const& filePath) -> std::optional<T>;
	template <typename T>
	static auto parsedList(std::string const& filePath) -> std::optional<std::vector<T>>;
public:
	template <typename T>
	static auto parsedObjectImpl(std::string const&) -> T;
	template <typename T>
	static auto parsedListImpl(std::string const&) -> std::vector<T>;
private:
	template <typename T>
	static auto assertExposable() -> void;
private:
	static auto rawLines(std::string const&) -> std::vector<std::string>;
	static auto filteredLines(std::vector<std::string> const&) -> std::vector<std::string>;
	static auto content(std::vector<std::string> const&) -> std::string;
	using OptPair = std::optional<std::pair<std::string, int>>;
	static auto extract(std::string const&, char open, char close, int from = 0) -> OptPair;
	static auto extractIntegral(std::string const& s, int i) -> OptPair;
	using ValueExtractor = std::function<OptPair(std::string const&, int)>;
	static auto valueExtractors() -> std::vector<ValueExtractor> const&;
	static auto valueExtractor(std::string const&) -> std::optional<ValueExtractor>;
public:
	template <typename T>
	static auto getter() -> std::function<T(std::string const&)>;
public:
	static auto objectContent(std::string const&) -> std::optional<std::string>;
	static auto listContent(std::string const&) -> std::optional<std::string>;
	static auto map(std::string const&) -> std::map<std::string, std::string>;
};

template <typename T>
class Exposable
{
public:
	using JsonTag = std::string;
	using FieldFiller = std::function<void(T& o, std::string const&)>;
	using Schema = std::map<JsonTag, FieldFiller>;
public:
	template <typename M>
	static void expose(JsonTag const&, M T::*);
	static auto schema() -> Schema const&;
	static void unexpose();
private:
	static std::optional<Schema> m_opt_schema;
};

template <typename T>
auto JsonParser::parsedObject(std::string const& c) -> std::optional<T>
{
	auto const opt_oc = objectContent(content(filteredLines(rawLines(c))));
	if (!opt_oc.has_value()) return {};
	return parsedObjectImpl<T>(opt_oc.value());
}

template <typename T>
auto JsonParser::parsedList(std::string const& c) -> std::optional<std::vector<T>>
{
	auto const opt_lc = listContent(content(filteredLines(rawLines(c))));
	if (!opt_lc.has_value()) return {};
	return parsedListImpl<T>(opt_lc.value());
}

template <typename T>
auto JsonParser::parsedObjectImpl(std::string const& oc) -> T
{
	assertExposable<T>();
	auto const m = map(oc);
	auto result = T();
	auto const& schema = T::schema();
	for (auto const& [expectedTag, f] : schema)
		if (m.find(expectedTag) != m.end())
			f(result, m.at(expectedTag));
	Exposable<T>::unexpose();
	return result;
}

template <typename T>
auto JsonParser::parsedListImpl(std::string const& lc) -> std::vector<T>
{
	const auto opt_ve = valueExtractor(lc);
	if (!opt_ve) return {};
	auto const ve = opt_ve.value();
	auto const g = getter<T>();
	auto result = std::vector<T>();
	int i = 0;
	while (auto const opt_res = ve(lc, i)){
		auto const [v, j] = opt_res.value();
		auto element = T();
		result.push_back(std::move(g(v)));
		i = j + 1;
	}
	return result;
}

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

template <typename T>
auto JsonParser::assertExposable() -> void
{
	static_assert(JsonParserUtils::is_exposable_v<T>,
		"A class should inherit from Exposable<T> templated on itself so as to be parsed");
	static_assert(JsonParserUtils::has_function_expose_v<T>,
		"A class should define a static function expose() so as to be parsed");
}

template <typename M>
auto JsonParser::getter() -> std::function<M(std::string const&)>
{
	return [](std::string const& d) -> M{
		if constexpr (std::is_same_v<M, std::string>)
			return d;
		else if constexpr (std::is_same_v<M, int>)
			return std::stoi(d);
		else if constexpr (std::is_same_v<M, bool>)
			return d == "true";
		else if constexpr (JsonParserUtils::is_exposable_v<M>)
			return JsonParser::parsedObjectImpl<M>(d);
		else if constexpr (JsonParserUtils::is_container_v<M>)
			if (auto const opt_lc = JsonParser::listContent(d))
				return JsonParser::parsedListImpl<JsonParserUtils::element_type_t<M>>(opt_lc.value());
		return {};
	};
}

template <typename T>
auto Exposable<T>::schema() -> Schema const&
{
	if (!m_opt_schema.has_value()){
		m_opt_schema = Schema();
		T::expose();
	}
	return m_opt_schema.value();
}

template <typename T> template <typename M>
void Exposable<T>::expose(JsonTag const& k, M T::* p)
{
	auto const g = JsonParser::getter<M>();
	auto const f = [p, g](T& o, std::string const& d){o.*p = g(d);};
	m_opt_schema->emplace(k, f);
}

template <typename T>
void Exposable<T>::unexpose()
{
	m_opt_schema = std::nullopt;
}

template <typename T>
std::optional<typename Exposable<T>::Schema> Exposable<T>::m_opt_schema;
