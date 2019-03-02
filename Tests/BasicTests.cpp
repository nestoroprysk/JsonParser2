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
	EXPECT_EQ(opt_result->age, 10);
}

TEST(basicTests, Couple)
{
	const auto jsonFileName = TestsUtils::getJsonFileFullPath("Couple");
	const auto opt_result = JsonParser::parsedObject<BasicTestsStructs::Couple>(jsonFileName);
	EXPECT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->a.name, "Peter");
	EXPECT_EQ(opt_result->a.age, 10);
	EXPECT_EQ(opt_result->b.name, "Mary");
	EXPECT_EQ(opt_result->b.age, 9);
}

TEST(basicTests, Switcher)
{
	const auto jsonFileName = TestsUtils::getJsonFileFullPath("Switcher");
	const auto opt_result = JsonParser::parsedObject<BasicTestsStructs::Switcher>(jsonFileName);
	EXPECT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->turnedOn, true);
	EXPECT_EQ(opt_result->turnedOff, false);
}
