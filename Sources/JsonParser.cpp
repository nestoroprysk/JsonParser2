#include "JsonParser.hpp"

#include <fstream>
#include <iostream>

JsonParser::JsonParser(std::string const& filePath)
{
	std::ifstream ifs(filePath);

	for (std::string line; std::getline(ifs, line);)
		lines.push_back(std::move(line));
}
