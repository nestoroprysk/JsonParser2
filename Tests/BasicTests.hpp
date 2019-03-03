#pragma once

#include "JsonParser.hpp"

#include <list>

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
	struct Facebook : Exposable<Facebook>
	{
		std::vector<Person> list;
		int nbPersons = 0;
		static void expose()
		{
			Exposable<Facebook>::expose("list", &Facebook::list);
			Exposable<Facebook>::expose("nbPersons", &Facebook::nbPersons);
		}
	};
	struct BirthDateRegistrar : Exposable<BirthDateRegistrar>
	{
		std::list<int> list;
		int nbEntries = 0;
		static void expose()
		{
			Exposable<BirthDateRegistrar>::expose("list", &BirthDateRegistrar::list);
			Exposable<BirthDateRegistrar>::expose("nbEntries", &BirthDateRegistrar::nbEntries);
		}
	};
}
