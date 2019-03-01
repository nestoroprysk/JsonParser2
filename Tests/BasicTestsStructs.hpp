#pragma once

#include "JsonParser.hpp"

namespace BasicTestsStructs
{
	struct Person : Exposable<Person>
	{
	public:
		std::string name;
		int age = 0;
	private:
		void expose() const override
		{
			field("name", &Person::name);
			field("age", &Person::age);
		}
	};
	struct Couple : Exposable<Couple>
	{
	public:
		Person a;
		Person b;
	private:
		void expose() const override
		{
			field("a", &Couple::a);
			field("b", &Couple::b);
		}
	};
}
