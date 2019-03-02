#pragma once

#include <string>

namespace TestsUtils
{
	namespace Detail
	{
		inline const char* getPathToTestsDir()
		{
			return PATH_TO_TESTS_DIR;
		}
	}
	inline std::string getJsonFileFullPath(std::string const& name)
	{
		auto const prefix = std::string(Detail::getPathToTestsDir());
		auto const jsonTestFilesDir = std::string("Jsons");
		auto const suffix = ".json";
		return prefix + '/' + jsonTestFilesDir + '/' + name + suffix;
	}
}
