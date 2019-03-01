#include "JsonParser.hpp"
#include "TestsUtils.hpp"
#include "BasicTestsStructs.hpp"

#include "gtest/gtest.h"

TEST(basicTests, Person)
{
	const auto jsonFileName = "Person";
	auto p = JsonParser(TestsUtils::getJsonFileFullPath(jsonFileName));
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Person>().has_value(), true);
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Person>()->name, "Peter");
}

TEST(basicTests, COuple)
{
	const auto jsonFileName = "Couple";
	auto p = JsonParser(TestsUtils::getJsonFileFullPath(jsonFileName));
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Couple>().has_value(), true);
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Couple>()->a.name, "Peter");
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Couple>()->a.age, 10);
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Couple>()->b.name, "Mary");
	EXPECT_EQ(p.parsedObject<BasicTestsStructs::Couple>()->b.age, 9);
}
