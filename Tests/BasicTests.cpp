#include "JsonParser.hpp"
#include "TestsUtils.hpp"
#include "BasicTestsStructs.hpp"

#include "gtest/gtest.h"

TEST(basicTests, edgeCases)
{
	const auto jsonFileName = "Person";
	auto p = JsonParser(TestsUtils::getJsonFileFullPath(jsonFileName));
	EXPECT_EQ(p.parse<BasicTestsStructs::Person>().empty(), false);
}
