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
		const auto prefix = std::string(Detail::getPathToTestsDir());
		const auto jsonTestFilesDir = std::string("BasicTestsJsons");
		const auto suffix = ".json";
		return prefix + '/' + jsonTestFilesDir + '/' + name + suffix;
	}
}
