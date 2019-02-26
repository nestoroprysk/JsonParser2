#pragma once

#include <string>
#include <vector>

class JsonParser
{
public:
	JsonParser(std::string const&);
private:
	std::vector<std::string> lines;
};
