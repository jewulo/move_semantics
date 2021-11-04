#pragma once
#include <string>

// Value Categories
namespace chapter_8
{
	// History of Value Categories
	namespace sec_8_1_1
	{
		void run(void)
		{
			{
				42;         // a integer literal is not lvalue
				// &(42);   // therefore you cannot take its address

				"hello";    // a string literal is an lvalue
				&"hello";   // therefore you can take its address

				"hello"s;       // a string object is not an lvalue
				//&"hello"s;    // therefore you cannot take its address

				// the above is equivalent to
				std::string("hello");       // a string object is not an lvalue
				//&std::string("hello");    // therefore you cannot take its address

				cout << "hello" << "\n";
				cout << &"hello" << "\n";
			}

			{
				int i = 42;
				i = 77;		// OK
				//77 = i;		// ERROR
			}

			{
				int x;		// x is an lvalue when used in an expression
				x = 42;		// OK because x is an lvalue and the type matches
				//42 = x;		// ERROR: 42 is an rvalue and can be only on the right-hand side of an assignment
			}

			{
				int x;			// x is an lvalue when used in an expression
				int* p1 = &x;	// OK: & is fine for lvalues (object has a specified location)
				//int* p2 = &42;	// ERROR: & is not allowed for rvalues (object has no specified location)
			}

			{
				const int c = 42;	// Is an lvalue or an rvalue?
				//c = 42;				// ERROR:: we cannot modify const (so that c should no longer be an lvalue)
				const int* p1 = &c;	// still OK (so that c should still be an lvalue)
			}
			/*
			* Note:
			* lvalue stands for locator value.
			* rvalue stands for readable value.
			*/

			// Objects and FDTs (Fundamental Data Types) behave differently.
			{
				// Objects
				std::string s;
				std::move(s) = "hello";		// OK (behaves like an lvalue)
				//auto ps = &std::move(s);	// ERROR (behaves like an rvalue)

				// FDTs(Fundamental Data Types)
				int i;
				//std::move(i) = 42;		// ERROR (behaves like an rvalue)
				//auto pi = &std::move(i);	// ERROR (behaves like an rvalue)
			}
		}
	}

	// Value Categories Since C++11
	namespace sec_8_1_2
	{
		class X {};
		void f(X param_x){}

		void run(void)
		{
			X v;
			const X c;

			f(v);				// passes a modifiable lvalue
			f(c);				// passes a non-modifiable lvalue
			f(X());				// passes a prvalue (old syntax of creating a temporary)
			f(X{});				// passes a prvalue (new syntax of creating a temporary)
			f(std::move(v));	// passes a xvalue

			int x = 3;			// here, x is a variable, not an lvalue
			int y = x;			// here, x is an lvalue
		}
	}

	// Value Categories Since C++17
	namespace sec_8_1_3
	{
		class C {
		public:
			C(){};
			C(const C&) = delete;	// this class is neither copyable...
			C(C&&) = delete;		// ...nor movable
		};

		C createC()
		{
			return C{};		// Always creates a conceptual temporary prior to C++17
		}					// In C++17, no temporary object is created at this point

		void takeC(C val)
		{
		}

		class X {};
		void f(const X& param_x) {}	// accepts an expression of any value category but expects a glvalue

		void run(void)
		{
			auto n = createC();	// OK since C++17 (error prior to C++17)
			takeC(createC());	// OK since C++17 (error prior to C++17)
			f(X{});				// creates a temporary prvalue and passes it materialized as an xvalue
		}
	}

	// Special Rules for Value Categories

	// Value Category of Functions
	namespace sec_8_2_1
	{
		void f(int){}

		void(&fref1)(int) = f;	// fref1 is an lvalue (fref1 is a reference to a function)
		void(&&fref2)(int) = f;	// fref2 is also an lvalue (fref1 is a reference to a function)

		auto& ar = std::move(f);	// OK 
	}

	// Value Category of Data Members
	namespace sec_8_2_2
	{
	}
}