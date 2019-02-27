#pragma once

#include "Exposable.hpp"

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
}
