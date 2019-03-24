#include "JsonParser.hpp"
#include "TestsUtils.hpp"
#include "AdvancedTests.hpp"

#include "gtest/gtest.h"

TEST(advancedTests, Warrior)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Warrior");
	auto const opt_result = JsonParser::parsedObject<AdvancedTestsStructs::Warrior>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->name, "Lao");
	ASSERT_NE(opt_result->weapon, nullptr);
	EXPECT_EQ(opt_result->weapon->name, "Rifle");
	EXPECT_EQ(opt_result->weapon->damage, 10);
}

TEST(advancedTests, ListOfIntUPs)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("ListOfInts");
	auto const opt_result = JsonParser::parsedList<std::unique_ptr<int>>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->size(), 3);
	ASSERT_NE(opt_result->at(0), nullptr);
	EXPECT_EQ(*opt_result->at(0), 1);
	ASSERT_NE(opt_result->at(1), nullptr);
	EXPECT_EQ(*opt_result->at(1), 2);
	ASSERT_NE(opt_result->at(2), nullptr);
	EXPECT_EQ(*opt_result->at(2), 3);
}

TEST(advancedTests, Painting)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Painting");
	auto const opt_result = JsonParser::parsedList<AdvancedTestsStructs::Painting>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
}
