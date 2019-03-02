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

TEST(basicTests, ListOfPersons)
{
	const auto jsonFileName = TestsUtils::getJsonFileFullPath("ListOfPersons");
	const auto opt_result = JsonParser::parsedList<BasicTestsStructs::Person>(jsonFileName);
	EXPECT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->size(), 3);
	EXPECT_EQ(opt_result->at(0).name, "Peter");
	EXPECT_EQ(opt_result->at(0).age, 10);
	EXPECT_EQ(opt_result->at(1).name, "Mary");
	EXPECT_EQ(opt_result->at(1).age, 9);
	EXPECT_EQ(opt_result->at(2).name, "George");
	EXPECT_EQ(opt_result->at(2).age, 11);
}

TEST(basicTests, ListOfCouples)
{
	const auto jsonFileName = TestsUtils::getJsonFileFullPath("ListOfCouples");
	const auto opt_result = JsonParser::parsedList<BasicTestsStructs::Couple>(jsonFileName);
	EXPECT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->size(), 3);
	EXPECT_EQ(opt_result->at(0).a.name, "Peter");
	EXPECT_EQ(opt_result->at(0).a.age, 10);
	EXPECT_EQ(opt_result->at(0).b.name, "Mary");
	EXPECT_EQ(opt_result->at(0).b.age, 9);
	EXPECT_EQ(opt_result->at(1).a.name, "George");
	EXPECT_EQ(opt_result->at(1).a.age, 11);
	EXPECT_EQ(opt_result->at(1).b.name, "Zoe");
	EXPECT_EQ(opt_result->at(1).b.age, 10);
	EXPECT_EQ(opt_result->at(2).a.name, "Mark");
	EXPECT_EQ(opt_result->at(2).a.age, 15);
	EXPECT_EQ(opt_result->at(2).b.name, "Tina");
	EXPECT_EQ(opt_result->at(2).b.age, 16);
}
