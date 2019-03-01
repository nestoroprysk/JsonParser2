#include "JsonParser.hpp"
#include "TestsUtils.hpp"
#include "BasicTestsStructs.hpp"

#include "gtest/gtest.h"

TEST(basicTests, Person)
{
	const auto jsonFileName = TestsUtils::getJsonFileFullPath("Person");
	const auto opt_result = JsonParser::parsedObject<BasicTestsStructs::Person>(jsonFileName);
	EXPECT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->name, "Peter");
}

TEST(basicTests, COuple)
{
	const auto jsonFileName = TestsUtils::getJsonFileFullPath("Couple");
	const auto opt_result = JsonParser::parsedObject<BasicTestsStructs::Couple>(jsonFileName);
	EXPECT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->a.name, "Peter");
	EXPECT_EQ(opt_result->a.age, 10);
	EXPECT_EQ(opt_result->b.name, "Mary");
	EXPECT_EQ(opt_result->b.age, 9);
}
