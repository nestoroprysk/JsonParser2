#pragma once

#include "JsonParser.hpp"

namespace BasicTestsStructs
{
	struct Person : Exposable<Person>
	{
		std::string name;
		int age = 0;
		static void expose()
		{
			Exposable<Person>::expose("name", &Person::name);
			Exposable<Person>::expose("age", &Person::age);
		}
	};
	struct Couple : Exposable<Couple>
	{
		Person a;
		Person b;
		static void expose()
		{
			Exposable<Couple>::expose("a", &Couple::a);
			Exposable<Couple>::expose("b", &Couple::b);
		}
	};
	struct Switcher : Exposable<Switcher>
	{
		bool turnedOn = false;
		bool turnedOff = true;
		static void expose()
		{
			Exposable<Switcher>::expose("turnedOn", &Switcher::turnedOn);
			Exposable<Switcher>::expose("turnedOff", &Switcher::turnedOff);
		}
	};
}
