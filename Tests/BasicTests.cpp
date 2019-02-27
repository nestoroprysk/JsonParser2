#include "JsonParser.hpp"
#include "TestsUtils.hpp"
#include "BasicTestsStructs.hpp"

#include "gtest/gtest.h"

TEST(basicTests, edgeCases)
{
	const auto jsonFileName = "Person";
	auto p = JsonParser(TestsUtils::getJsonFileFullPath(jsonFileName));
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Person>().has_value(), true);
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Person>()->name, "Peter");
}
