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
	EXPECT_EQ(opt_result->list.at(0).name, "Peter");
	EXPECT_EQ(opt_result->list.at(0).age, 20);
	EXPECT_EQ(opt_result->list.at(1).name, "Mary");
	EXPECT_EQ(opt_result->list.at(1).age, 19);
	EXPECT_EQ(opt_result->list.at(2).name, "George");
	EXPECT_EQ(opt_result->list.at(2).age, 21);
	EXPECT_EQ(opt_result->nbPersons, 3);
}

TEST(basicTests, BirthDateRegistrar)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("BirthDateRegistrar");
	auto const opt_result = JsonParser::parsedObject<BasicTestsStructs::BirthDateRegistrar>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	ASSERT_EQ(opt_result->list.size(), 4);
	EXPECT_EQ(*opt_result->list.cbegin(), std::numeric_limits<unsigned long long>::min());
	EXPECT_EQ(*std::next(opt_result->list.cbegin(), 1), std::numeric_limits<unsigned long long>::max());
	EXPECT_EQ(*std::next(opt_result->list.cbegin(), 2), 1970);
	EXPECT_EQ(*std::next(opt_result->list.cbegin(), 3), 2001);
}

TEST(basicTests, Arithmetic)
{
	auto const jsonFileName = TestsUtils::getJsonFileFullPath("Arithmetic");
	auto const opt_result = JsonParser::parsedList<BasicTestsStructs::Arithmetic>(jsonFileName);
	ASSERT_EQ(opt_result.has_value(), true);
	ASSERT_EQ(opt_result->size(), 2);
	auto const min = opt_result->at(0);
	EXPECT_EQ(min.ui, std::numeric_limits<unsigned int>::min());
	EXPECT_EQ(min.i, std::numeric_limits<int>::min());
	EXPECT_EQ(min.l, std::numeric_limits<long>::min());
	EXPECT_EQ(min.ll, std::numeric_limits<long long>::min());
	EXPECT_EQ(min.st, std::numeric_limits<size_t>::min());
	EXPECT_EQ(min.us, std::numeric_limits<unsigned short>::min());
	EXPECT_EQ(min.s, std::numeric_limits<short>::min());
	EXPECT_EQ(min.ul, std::numeric_limits<unsigned long>::min());
	EXPECT_EQ(min.ull, std::numeric_limits<unsigned long long>::min());
	auto const max = opt_result->at(1);
	EXPECT_EQ(max.ui, std::numeric_limits<unsigned int>::max());
	EXPECT_EQ(max.i, std::numeric_limits<int>::max());
	EXPECT_EQ(max.l, std::numeric_limits<long>::max());
	EXPECT_EQ(max.ll, std::numeric_limits<long long>::max());
	EXPECT_EQ(max.st, std::numeric_limits<size_t>::max());
	EXPECT_EQ(max.us, std::numeric_limits<unsigned short>::max());
	EXPECT_EQ(max.s, std::numeric_limits<short>::max());
	EXPECT_EQ(max.ull, std::numeric_limits<unsigned long long>::max());
	EXPECT_EQ(max.ull, std::numeric_limits<unsigned long long>::max());
}
