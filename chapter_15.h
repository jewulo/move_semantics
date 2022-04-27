#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <string>

namespace chapter_15
{
	// Move Semantics for Strings
	namespace sec_15_1
	{
		void run()
		{
			std::string s0;
			std::string s1{ "short" };
			std::string s2{ "a string with an extraordinarily long value" };
			std::cout << "- s0 capacity: " << s0.capacity() << " ('" << s0 << "')\n";
			std::cout << "  s1 capacity: " << s1.capacity() << " ('" << s1 << "')\n";
			std::cout << "  s2 capacity: " << s2.capacity() << " ('" << s2 << "')\n";

			std::string s3{ std::move(s1) };
			std::string s4{ std::move(s2) };
			std::cout << "- s1 capacity: " << s1.capacity() << " ('" << s1 << "')\n";
			std::cout << "  s2 capacity: " << s2.capacity() << " ('" << s2 << "')\n";
			std::cout << "  s3 capacity: " << s3.capacity() << " ('" << s3 << "')\n";
			std::cout << "  s4 capacity: " << s4.capacity() << " ('" << s4 << "')\n";

			std::string s5{ "quite a reasonable value" };
			std::cout << "- s4 capacity: " << s4.capacity() << " ('" << s4 << "')\n";
			std::cout << "  s5 capacity: " << s5.capacity() << " ('" << s5 << "')\n";

			s4 = std::move(s5);
			std::cout << "- s4 capacity: " << s4.capacity() << " ('" << s4 << "')\n";
			std::cout << "  s5 capacity: " << s5.capacity() << " ('" << s5 << "')\n";
		}
	}

	// Move Semantics for Containers
	namespace sec_15_2
	{
		void run()
		{}
	}

}