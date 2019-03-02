#include "JsonParser.hpp"
#include "TestsUtils.hpp"
#include "BasicTests.hpp"

#include "gtest/gtest.h"

TEST(basicTests, Person)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Person");
	auto const opt_result = JsonParser::parsedObject<BasicTestsStructs::Person>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->name, "Peter");
	EXPECT_EQ(opt_result->age, 10);
}

TEST(basicTests, Couple)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Couple");
	auto const opt_result = JsonParser::parsedObject<BasicTestsStructs::Couple>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->a.name, "Peter");
	EXPECT_EQ(opt_result->a.age, 10);
	EXPECT_EQ(opt_result->b.name, "Mary");
	EXPECT_EQ(opt_result->b.age, 9);
}

TEST(basicTests, Switcher)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Switcher");
	auto const opt_result = JsonParser::parsedObject<BasicTestsStructs::Switcher>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	EXPECT_EQ(opt_result->turnedOn, true);
	EXPECT_EQ(opt_result->turnedOff, false);
}

TEST(basicTests, ListOfPersons)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("ListOfPersons");
	auto const opt_result = JsonParser::parsedList<BasicTestsStructs::Person>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	ASSERT_EQ(opt_result->size(), 3);
	EXPECT_EQ(opt_result->at(0).name, "Peter");
	EXPECT_EQ(opt_result->at(0).age, 10);
	EXPECT_EQ(opt_result->at(1).name, "Mary");
	EXPECT_EQ(opt_result->at(1).age, 9);
	EXPECT_EQ(opt_result->at(2).name, "George");
	EXPECT_EQ(opt_result->at(2).age, 11);
}

TEST(basicTests, ListOfCouples)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("ListOfCouples");
	auto const opt_result = JsonParser::parsedList<BasicTestsStructs::Couple>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	ASSERT_EQ(opt_result->size(), 3);
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

TEST(basicTests, ListOfInts)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("ListOfInts");
	auto const opt_result = JsonParser::parsedList<int>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	ASSERT_EQ(opt_result->size(), 3);
	EXPECT_EQ(opt_result->at(0), 1);
	EXPECT_EQ(opt_result->at(1), 2);
	EXPECT_EQ(opt_result->at(2), 3);
}

TEST(basicTests, ListOfStrings)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("ListOfStrings");
	auto const opt_result = JsonParser::parsedList<std::string>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	ASSERT_EQ(opt_result->size(), 3);
	EXPECT_EQ(opt_result->at(0), "a");
	EXPECT_EQ(opt_result->at(1), "b");
	EXPECT_EQ(opt_result->at(2), "c");
}

TEST(basicTests, Facebook)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Facebook");
	auto const opt_result = JsonParser::parsedObject<BasicTestsStructs::Facebook>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	ASSERT_EQ(opt_result->list.size(), 3);
	EXPECT_EQ(opt_result->nbPersons, 3);
}
