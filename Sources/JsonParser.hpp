#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>

class JsonParser
{
public:
	template <typename T>
	static auto parsedObject(std::string const&) -> std::optional<T>;
public:
	template <typename T>
	static auto parsedObjectImpl(std::string const&) -> T;
private:
	static auto rawLines(std::string const&) -> std::vector<std::string>;
	static auto filteredLines(std::vector<std::string> const&) -> std::vector<std::string>;
	static auto content(std::vector<std::string> const&) -> std::string;
	using OptPair = std::optional<std::pair<std::string, int>>;
	static auto extract(std::string const&, char open, char close, int from = 0) -> OptPair;
	static auto extractIntegral(std::string const& s, int i) -> OptPair;
	using ValueExtractor = std::function<OptPair(std::string const&, int)>;
	static auto valueExtractors() -> std::vector<ValueExtractor> const&;
private:
	static auto objectContent(std::string const&) -> std::optional<std::string>;
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

namespace JsonParserUtils
{
	template <typename T>
	static constexpr bool is_exposable_v =
		std::is_base_of<Exposable<T>, T>::value;
}

template <typename T>
auto JsonParser::parsedObject(std::string const& c) -> std::optional<T>
{
	const auto opt_oc = objectContent(content(filteredLines(rawLines(c))));
	if (!opt_oc.has_value()) return {};
	return parsedObjectImpl<T>(opt_oc.value());
}

template <typename T>
auto JsonParser::parsedObjectImpl(std::string const& oc) -> T
{
	static_assert(JsonParserUtils::is_exposable_v<T>,
		"An object should be exposable so as to be parsed");
	const auto m = map(oc);
	auto result = T();
	auto const& schema = result.schema();
	for (auto const& [expectedTag, f] : schema)
		if (m.find(expectedTag) != m.end())
			f(result, m.at(expectedTag));
	Exposable<T>::unexpose();
	return result;
}

template <typename T>
std::optional<typename Exposable<T>::Schema> Exposable<T>::m_opt_schema;

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
	const auto get = [](std::string const& d) -> M{
		if constexpr (std::is_same_v<M, std::string>)
			return d;
		else if constexpr (std::is_same_v<M, int>)
			return std::stoi(d);
		else if constexpr (std::is_same_v<M, bool>)
			return d == "true";
		else if constexpr (JsonParserUtils::is_exposable_v<M>)
			return JsonParser::parsedObjectImpl<M>(d);
		else
			return {};
	};
	const auto l = [p, get](T& o, std::string const& d){o.*p = get(d);};
	m_opt_schema->emplace(k, l);
}

template <typename T>
void Exposable<T>::unexpose()
{
	m_opt_schema = std::nullopt;
}
