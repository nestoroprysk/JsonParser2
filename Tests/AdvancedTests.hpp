#pragma once

#include "JsonParser.hpp"

namespace AdvancedTestsStructs
{
	struct Weapon : Exposable<Weapon>
	{
		std::string name;
		int damage = 0;
		static void expose()
		{
			Exposable<Weapon>::expose("name", &Weapon::name);
			Exposable<Weapon>::expose("damage", &Weapon::damage);
		}
	};

	struct Warrior : Exposable<Warrior>
	{
		std::string name;
		std::unique_ptr<Weapon> weapon;
		static void expose()
		{
			Exposable<Warrior>::expose("name", &Warrior::name);
			Exposable<Warrior>::expose("weapon", &Warrior::weapon);
		}
	};
}
