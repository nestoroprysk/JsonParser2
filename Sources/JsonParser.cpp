#include "JsonParser.hpp"

#include <fstream>
#include <iostream>

JsonParser::JsonParser(std::string const& filePath)
{
	auto ifs = std::ifstream(filePath);

	for (std::string line; std::getline(ifs, line);)
		lines.push_back(std::move(line));
}
