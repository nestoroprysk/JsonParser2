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

	struct IShape : Exposable<IShape>
	{
		virtual ~IShape(){}
		static void expose(){}
	};

	struct Circle final: IShape, Exposable<Circle>
	{
		int r = 0;
		static void expose()
		{
			Exposable<Circle>::expose("r", &Circle::r);
		}
	};

	struct Square final: IShape, Exposable<Square>
	{
		int side = 0;
		static void expose()
		{
			Exposable<Square>::expose("side", &Square::side);
		}
	};

	struct IShapeExposingHelper : InterfaceExposingHelper<IShape>
	{
		IShapeExposingHelper()
		{
			add_mapping<Circle>([this]{return tag("type") == "circle";});
			add_mapping<Square>([this]{return tag("type") == "square";});
		}
	};

	struct Painting : Exposable<Painting>
	{
		std::vector<std::unique_ptr<IShape>> shapes;
		int nbShapes = 0;
		static void expose()
		{
			Exposable<IShape>::register_helper<IShapeExposingHelper>();
			Exposable<Painting>::expose("shapes", &Painting::shapes);
			Exposable<Painting>::expose("nbShapes", &Painting::nbShapes);
		}
	};
}
