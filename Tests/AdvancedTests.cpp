#include "JsonParser.hpp"
#include "TestsUtils.hpp"
#include "AdvancedTests.hpp"

#include "gtest/gtest.h"

TEST(advancedTests, Warrior)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Warrior");
	auto const opt_result = JsonParser::parsedObject<AdvancedTestsStructs::Warrior>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result.value().name, "Lao");
	ASSERT_NE(opt_result.value().weapon, nullptr);
	EXPECT_EQ(opt_result.value().weapon->name, "Rifle");
	EXPECT_EQ(opt_result.value().weapon->damage, 10);
}
