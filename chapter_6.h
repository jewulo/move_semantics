#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

// Moved From States
namespace chapter_6
{
	// Required and Guaranteed States of Moved-From Objects
	namespace sec_6_1
	{
	}
	// Required States of Moved-From Objects
	namespace sec_6_1_1{}
	// Guaranteed States of Moved-From Objects
	namespace sec_6_1_2
	{
		void run()
		{
			std::string s;

		}
	}
	// Broken Invariants
	namespace sec_6_1_3{}
	// Destructible and Assignable
	namespace sec_6_1
	{
	}

}
