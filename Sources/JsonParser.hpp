#pragma once

#include "JsonParserUtils.hpp"

#include <string>
#include <vector>
#include <optional>
#include <map>

class JsonParser
{
public:
	using FilePath = std::string;
	JsonParser(FilePath const&);
	template <typename T>
	auto parsedObject() -> std::optional<T>;
	template <typename T>
	auto parsedList() -> std::optional<std::vector<T>>;
private:
	using LinesList = std::vector<std::string>;
	static auto rawLines(FilePath const&) -> LinesList;
	static auto filteredLines(LinesList const&) -> LinesList;
	using Content = std::string;
	static auto content(LinesList const&) -> Content;
private:
	static auto objectContent(Content const&) -> std::optional<Content>;
	using Map = std::map<std::string, std::string>;
	static auto map(Content const&) -> Map;
private:
	static auto map(Content const&, Map&, int i = 0) -> void;
private:
	Content const m_file_content;
};

template <typename T>
auto JsonParser::parsedObject() -> std::optional<T>
{
	static_assert(JsonParserUtils::is_exposable_v<T>,
		"An object should be exposable so as to be parsed");
	const auto opt_oc = objectContent(m_file_content);
	if (!opt_oc.has_value()) return {};
	const auto m = map(opt_oc.value());
	auto result = T();
	auto const& schema = result.schema();
	for (auto const& [expectedTag, f] : schema)
		f(result, m.at(expectedTag));
	return result;
}

template <typename T>
auto JsonParser::parsedList() -> std::optional<std::vector<T>>
{
	return {};
}
