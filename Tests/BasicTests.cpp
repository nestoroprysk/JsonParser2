#include "JsonParser.hpp"
#include "TestsUtils.hpp"

#include "gtest/gtest.h"

TEST(basicTests, edgeCases)
{
	const auto jsonFileName = "Struct";
	const auto p = JsonParser(TestsUtils::getJsonFileFullPath(jsonFileName));
	EXPECT_EQ(1, 0);
}
