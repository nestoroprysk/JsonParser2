#include "JsonParser.hpp"

#include <fstream>
#include <cassert>
#include <iostream>

JsonParser::JsonParser(FilePath const& fp)
	: m_file_content(content(filteredLines(rawLines(fp))))
{
}

auto JsonParser::rawLines(FilePath const& fp) -> LinesList
{
	LinesList result;
	auto ifs = std::ifstream(fp);
	for (std::string line; std::getline(ifs, line);)
		result.push_back(std::move(line));
	return result;
}

auto JsonParser::filteredLines(LinesList const& ls) -> LinesList
{
	LinesList result;
	for (auto const& l : ls){
		std::string line;
		for (auto c : l)
			if (!std::isspace(c))
				line += c;
		result.push_back(std::move(line));
	}
	return result;
}

auto JsonParser::content(LinesList const& ls) -> Content
{
	Content result;
	for (auto const& l : ls)
		result += l;
	return result;
}

auto JsonParser::objectContent(Content const& c) -> std::optional<Content>
{
	if (c.empty()) return {};
	auto begin = c.cbegin();
	while (begin != c.cend() && *begin != '{')
		++begin;
	if (begin == c.cend()) return {};
	auto end = c.cend();
	--end;
	while (end != c.cbegin() && *end != '}')
		--end;
	if (end == c.cbegin()) return {};
	return std::string(begin + 1, end);
}

auto JsonParser::map(Content const& c) -> Map
{
	if (c.empty()) return {};
	auto result = Map();
	map(c, result);
	return result;
}

auto JsonParser::map(Content const& c, Map& o, int i) -> void
{
	if (i >= c.size())
		return;
	assert(c[i] == '\"');
	auto j = i + 1;
	while (j < c.size() && c[j] != '\"')
		++j;
	assert(j != c.size());
	auto const key = c.substr(i + 1, j - i - 1);
	++j;
	assert(c[j] == ':');
	++j;
	assert(j < c.size());
	assert(c[j] == '\"' || std::isdigit(c[j]));
	const auto beginsWithQuote = c[j] == '\"';
	if (beginsWithQuote){
		i = j + 1;
		++j;
		while (j < c.size() && c[j] != '\"')
			++j;
		const auto value = c.substr(i, j - i - 1);
		o.insert({key, value});
		if (j == c.size())
			return;
	}
	const auto isNumber = std::isdigit(c[j]);
	if (isNumber){
		i = j;
		while (j < c.size() && std::isdigit(c[j]))
			++j;
		const auto value = c.substr(i, j - i);
		o.insert({key, value});
		if (j == c.size())
			return;
	}
	++j;
	assert(c[j] == ',');
	++j;
	map(c, o, j);
}
