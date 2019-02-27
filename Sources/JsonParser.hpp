#pragma once

#include <string>
#include <vector>

class JsonParser
{
public:
	JsonParser(std::string const&);
	template <typename T>
	std::vector<T> parse();
private:
	std::vector<std::string> lines;
};

template <typename T>
std::vector<T> JsonParser::parse()
{
	return {};
}
