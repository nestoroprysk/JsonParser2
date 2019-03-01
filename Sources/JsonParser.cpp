#include "JsonParser.hpp"

#include <fstream>
#include <cassert>

auto JsonParser::rawLines(std::string const& fp) -> std::vector<std::string>
{
	std::vector<std::string> result;
	auto ifs = std::ifstream(fp);
	for (std::string line; std::getline(ifs, line);)
		result.push_back(std::move(line));
	return result;
}

auto JsonParser::filteredLines(std::vector<std::string> const& ls) -> std::vector<std::string>
{
	std::vector<std::string> result;
	for (auto const& l : ls){
		std::string line;
		for (auto c : l)
			if (!std::isspace(c))
				line += c;
		result.push_back(std::move(line));
	}
	return result;
}

auto JsonParser::content(std::vector<std::string> const& ls) -> std::string
{
	std::string result;
	for (auto const& l : ls)
		result += l;
	return result;
}

auto JsonParser::objectContent(std::string const& c) -> std::optional<std::string>
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

auto JsonParser::map(std::string const& c) -> Map
{
	if (c.empty()) return {};
	auto result = Map();
	map(c, result);
	return result;
}

auto JsonParser::map(std::string const& c, Map& o, int i) -> void
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
	assert(c[j] == '\"' || std::isdigit(c[j]) || c[j] == '{');
	
	const auto beginsWithQuote = c[j] == '\"';
	if (beginsWithQuote){
		i = j + 1;
		++j;
		while (j < c.size() && c[j] != '\"')
			++j;
		assert(j < c.size());
		const auto value = c.substr(i, j - i);
		o.insert({key, value});
		++j;
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
	
	const auto isStruct = c[j] == '{';
	if (isStruct){
		i = j + 1;
		++j;
		auto nbBracesToClose = 1;
		while (j < c.size() && nbBracesToClose > 0){
			if (c[j] == '{')
				++nbBracesToClose;
			else if (c[j] == '}')
				--nbBracesToClose;
			++j;
		}
		const auto value = c.substr(i, j - i - 1);
		o.insert({key, value});
		if (j == c.size())
			return;
	}
	
	assert(c[j] == ',');
	++j;
	map(c, o, j);
}
