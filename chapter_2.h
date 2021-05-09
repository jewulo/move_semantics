#pragma once

#include <iostream>
#include <utility>
#include <string>

namespace chapter_2
{
	std::string return_string_by_value()	
	{
		std::string rs{ "hello" };
		return rs;
	}

	// r-value references in detail
	namespace sec_2_1_1
	{
		void run(void)
		{
			{
				std::string s{ "hello" };

				// std::string&& r1{ s };						// ERROR, you cannot copy or initialise rvalues directly
				std::string&& r2{ std::move(s) };				// OK
				std::string&& r3{ return_string_by_value() };	// OK, extends lifetime of return value

			}
			{
				std::string s1{ "hello" };
				std::string&& r1 = std::move(s1);				// OK, rvalue references to s1

				std::string s2{ "hello" };
				std::string&& r2{ std::move(s2) };				// OK, rvalue references to s2

				std::string s3{ "hello" };
				std::string&& r3(std::move(s3));				// OK, rvalue references to s3

			}
			{
				std::string s1{ "hello" };
				std::string&& r1 = std::move(s1);				// OK, rvalue references to s1

				std::string s2{ "hello" };
				std::string&& r2{ std::move(s2) };				// OK, rvalue references to s2

				std::string&& r3{ return_string_by_value() };	// OK, extends lifetime of return value
			}
		}
	}

	// r-value references as parameters	
	namespace sec_2_1_2
	{
		// foo takes objects whoes internals are about to be discarded
		void foo(std::string&& rv)
		{
		}

		void run(void)
		{
			std::string s1{ "hello" };
			//foo(s1);							// ERROR, foo(std::string&&) only accepts r-values

			std::string s2{ "hello" };
			foo(std::move(s2));					// OK, std::move conversion to rvalue reference to s2 calls foo(std::string&&)

			foo(return_string_by_value());		// OK, extends lifetime of return value

			std::string s3{ "hello" };
			foo(std::move(s3));					// OK, std::move conversion to rvalue reference to s3 calls foo(std::string&&)
			// after the move the state odf s3 is undefined
			std::cout << s3 << '\n';			// OOPS, you don't know which value is printed
			foo(std::move(s3));					// OOPS, you don't know which value is passed
			s3 = "hello again";					// OK, but rarely done. Don't assign to a moved-from object
			foo(std::move(s3));					// OK, std::move conversion to rvalue reference to s3 calls foo(std::string&&)

		}
	}

	// using std::move
	namespace sec_2_2
	{
		void foo1(const std::string& lr) {}		// binds to the passed argument without modifying it
		void foo1(std::string&& rv) {}			// binds to the passed argument and might steal/modify the value

		void foo2(const std::string& lr) {}		// binds to the passed argument without modifying it

		void foo3(std::string& cv) {}			// modifies the passed argument
		
		void run()
		{
			std::string s1{ "hello" };
			foo1(s1);							// calls the 1st overload, it keeps its value
			foo1(std::move(s1));				// calls the 2nd overload, it might lose its value

			std::string s2{ "hello" };
			foo2(s1);							// calls foo2, it keeps its value
			foo2(std::move(s1));				// also calls foo2, it keeps its value

			// an object marked with std::move() cannot be passed to a non-const lvalue reference
			std::string s3{ "hello" };
			foo3(s3);							// take reference
			//foo3(std::move(s3));				// ERROR, you cannot pass a moved object to a non-const lvalue reference

			// std::move is implemented as a type-cast to an rvalue reference
			std::string s4{ "hello" };
			foo1(s4);											// calls the 1st overload, it keeps its value
			foo1(static_cast<std::string&&>(s4));				// calls the 2nd overload, it might lose its value

			std::string s5{ "hello" };
			foo1(static_cast<decltype(s5)&&>(s5));				// calls the 2nd overload, use decltype to deduce the type of s5

		}
	}

	// Moved from Objects
	namespace sec_2_3_1
	{
		void foo1(std::string&& rv) {}							// binds to the passed argument and might steal/modify the value

		void run()
		{
			std::string s1{ "hello" };

			foo1(std::move(s1));								// keeps s1 in a valid but unclear state
			std::cout << s1 << '\n';							// OK (don't know which value is written)
			std::cout << s1.size() << '\n';						// OK (writes current number of characters)
			std::cout << s1[0] << '\n';							// ERROR (potentially undefined behaviour)
			std::cout << s1.front() << '\n';					// ERROR (potentially undefined behaviour)

			s1 = "new value";									// OK

			foo1(std::move(s1));								// keeps s1 in a valid but unclear state
			for (std::size_t i = 0; i < s1.size(); ++i) {
				std::cout << s1[i];								// OK
			}
		}
	}

	// Reusing Moved-From Objects
#include <vector>
	namespace sec_2_3_2
	{
		void run()
		{
			std::vector<std::string> all_rows;
			std::string row;
			while (std::getline(std::cin, row)) {	// read next line into row
				all_rows.push_back(std::move(row));	// and move it to somewhere
			}
		}

		// swap using move semantics
		template <typename T>
		void swap(T& a, T& b)
		{
			T tmp{ std::move(a) };		// move construct tmp from a.
			a = std::move(b);			// assign new value to moved-from a
			b = std::move(tmp);			// assign new value to moved-from b
		}
	}

	namespace sec_2_3_3
	{
		struct X {
			X() = default;
			~X() = default;
			X(const X&) = default;
			X(X&&) = default;
			X& operator=(const X&) = default;
			X& operator=(X&&) = default;
		};

		void run()
		{
			X x;
			x = std::move(x);	// afterwards x is valid but has an unclear value
		}
	}

	// const rvalue references
	namespace sec_2_4
	{
		// There are now four different ways to overload on references
		// 
		// The first 3 are useful
		void foo(const std::string& arg) {}	// implies that foo as read only access to the object passed into the parameter arg
		void foo(std::string& arg) {}		// implies that foo as read/write access to the object passed into the parameter arg
		void foo(std::string&& arg) {}		// implies that foo is going to steal from object passed into the parameter arg

		// The final one is odd
		void foo(const std::string&& arg) {}		// implies that foo1 is going to accept const temporaries

		const std::string get_value()
		{
			const std::string s{ "data" };
			return s;
		}

		void run()
		{
			const std::string s1{ "data" };
			foo(std::move(s1));				// calls foo(const std::string&& arg)
			foo(get_value());				// calls foo(const std::string&& arg)

			const std::string s2{ "data" };
			foo(s2);						// calls foo(const std::string& arg)

			std::string s3{ "data" };
			foo(s3);						// calls foo(std::string& arg)
			foo(std::move(s3));				// calls foo(std::string&& arg)
		}
	}

	// Passing by Value
	namespace sec_2_5
	{
		void foo(std::string arg) {}		// implies that foo take the parameter arg by value

		const std::string get_value()
		{
			const std::string s{ "data" };
			return s;
		}

		void foo_by_value(std::string arg) {}		// implies that foo take the parameter arg by value
		void foo_by_ref(std::string&& arg) {}		// implies that foo take the parameter arg by rvalue reference

		void run()
		{
			std::string s{ "data" };
			foo(s);							// s is copied into arg
			foo(std::move(s));				// s is moved into arg
			foo(get_value());				// the return value of get_value() is moved into arg

			std::string s1{ "data" };
			std::string s2{ "data" };
			foo_by_value(std::move(s1));	// s1 is moved into arg
			foo_by_ref(std::move(s2));		// s2 might be moved into arg
		}
	}
}
