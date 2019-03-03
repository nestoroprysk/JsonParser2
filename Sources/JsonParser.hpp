#pragma once

#include "JsonParserUtils.hpp"

#include <optional>
#include <map>
#include <iostream>

class JsonParser
{
public:
	template <typename T>
	static auto parsedObject(std::string const& filePath) -> std::optional<T>;
	template <typename T>
	static auto parsedList(std::string const& filePath) -> std::optional<std::vector<T>>;
private:
	template <typename T>
	static auto parsedObjectImpl(std::string const&) -> T;
	template <typename T>
	static auto parsedListImpl(std::string const&) -> std::vector<T>;
private:
	using OptPair = std::optional<std::pair<std::string, int>>;
	using ValueExtractor = std::function<OptPair(std::string const&, int)>;
	static auto rawLines(std::string const&) -> std::vector<std::string>;
	static auto filteredLines(std::vector<std::string> const&) -> std::vector<std::string>;
	static auto content(std::vector<std::string> const&) -> std::string;
	static auto extract(std::string const&, char open, char close, int from = 0) -> OptPair;
	static auto extractIntegral(std::string const& s, int i) -> OptPair;
	static auto valueExtractors() -> std::vector<ValueExtractor> const&;
	static auto valueExtractor(std::string const&) -> std::optional<ValueExtractor>;
private:
	template <typename M>
	using Getter = std::function<M(std::string const&)>;
	template <typename M>
	static auto getter() -> std::optional<Getter<M>>;
	template <typename M>
	static auto getterFound(std::optional<Getter<M>> const&,
		std::optional<std::string> const& errorMessage = std::nullopt) -> bool;
private:
	static auto objectContent(std::string const&) -> std::optional<std::string>;
	static auto listContent(std::string const&) -> std::optional<std::string>;
	static auto map(std::string const&) -> std::map<std::string, std::string>;
private:
	template <typename T>
	friend class Exposable;
};

template <typename T>
class Exposable
{
public:
	using JsonTag = std::string;
	template <typename M>
	static void expose(JsonTag const&, M T::*);
private:
	using FieldFiller = std::function<void(T& o, std::string const&)>;
	using Schema = std::map<JsonTag, FieldFiller>;
	static auto schema() -> Schema const&;
	static void unexpose();
private:
	static std::optional<Schema> m_opt_schema;
private:
	friend class JsonParser;
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
	JsonParserUtils::assertExposable<T>();
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
	auto const opt_g = getter<T>();
	if (!getterFound(opt_g))
		return {};
	auto const g = opt_g.value();
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

template <typename M>
auto JsonParser::getter() -> std::optional<Getter<M>>
{
	if constexpr (std::is_same_v<M, std::string>)
		return [](std::string const& d){return d;};
	if constexpr (std::is_same_v<M, int>)
		return [](std::string const& d){return std::stoi(d);};
	if constexpr (std::is_same_v<M, bool>)
		return [](std::string const& d){return d == "true";};
	if constexpr (JsonParserUtils::is_exposable_v<M>)
		return [](std::string const& d){return JsonParser::parsedObjectImpl<M>(d);};
	if constexpr (JsonParserUtils::is_ord_container_v<M>)
		return [](std::string const& d){
			return JsonParserUtils::move_vector_into_ord<M>(
				JsonParser::parsedListImpl<JsonParserUtils::element_type_t<M>>(d));};
	return std::nullopt;
}

template <typename M>
auto JsonParser::getterFound(std::optional<Getter<M>> const& opt_g,
	std::optional<std::string> const& errorMessage) -> bool
{
	const auto result = opt_g.has_value();
	if (!result && errorMessage.has_value())
		std::cerr << errorMessage.value() << std::endl;
	return result;
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
	auto const opt_g = JsonParser::getter<M>();
	if (!JsonParser::getterFound(opt_g, JsonParserUtils::ignoreTagError(k)))
		return;
	auto const g = opt_g.value();
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
