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
		std::string ns_str{ "namespace string"};

		std::pair<std::string, std::string&> foo()
		{
			std::string foo_str{ "foo string" };
			return std::pair<std::string, std::string&>(foo_str,ns_str) ;
		}

		void run()
		{
			std::vector<std::string> coll;

			coll.push_back(foo().first);	// moves because first is an xvalue here
			coll.push_back(foo().second);	// copies because second is an lvalue here

			// you need std::move to move the second member
			coll.push_back(std::move(foo().second));	// moves
		}
	}

	// std::move() for Plain Data Members
	namespace sec_8_2_2b
	{
		std::pair<std::string, std::string> foo()
		{
			std::string foo_str_1{ "foo string 1" };
			std::string foo_str_2{ "foo string 2" };
			return std::pair<std::string, std::string>(foo_str_1, foo_str_2);
		}

		void run()
		{
			std::vector<std::string> coll;

			std::pair<std::string, std::string> sp1{ "foo string 1" ,"foo string 2" };
			coll.push_back(std::move(sp1.first));	// moves first string into coll
			coll.push_back(std::move(sp1.second));	// moves second string into coll

			// However the following code has the same effect
			std::pair<std::string, std::string> sp2{ "foo string 1" ,"foo string 2" };
			coll.push_back(std::move(sp2).first);	// moves first string into coll
			coll.push_back(std::move(sp2).second);	// moves second string into coll

		}
	}

	// std::move() for reference or static Members
	namespace sec_8_2_2c
	{
		// using std::move() for an object has no effect on reference or static Members
		std::string foo_str{ "foo string" };
		struct  S {
			static std::string statString;		// static member
			std::string& refString;				// reference member

			S(std::string refStr)
				: refString{ refStr }
			{}
		};
		std::string S::statString{ "static string" };

		void run()
		{
			{
				S obj(foo_str);
				std::vector<std::string> coll;

				// note: The reference or static members of a rvalue are NOT rvalues they are still lvalues.
				coll.push_back(std::move(obj).statString);	// copies statString
				coll.push_back(std::move(obj).refString);	// copies refString
			}

			{
				S obj(foo_str);
				std::vector<std::string> coll;
				// note: To move the reference or static members you need to explicitly move them
				coll.push_back(std::move(obj.statString));	// moves statString
				coll.push_back(std::move(obj.refString));	// moves refString
			}
		}
	}

	// In your application you would NOT really desire to move reference or static Members
	// so only ever always apply std::move to the enclosing object
	namespace sec_8_2_2d
	{
		// using std::move() for an object has no effect on reference or static Members
		std::string foo_str{ "foo string" };
		struct  S {
			static std::string	mem1;	// static member
			std::string&		mem2;	// reference member
			std::string			mem3;	// value data member

			S(std::string s1, std::string s2)
				: mem2{s1}, mem3{s2}
			{}
		};
		std::string S::mem1{ "static string" };

		void run()
		{
			S obj(foo_str, "bar string");
			std::vector<std::string> coll;

			// note: The reference or static members of a rvalue are NOT rvalues they are still lvalues.
			coll.push_back(std::move(obj).mem1);	// move value, copy reference/static
			coll.push_back(std::move(obj).mem2);	// move value, copy reference/static
			coll.push_back(std::move(obj).mem3);	// move value, copy reference/static
		}
	}

	// Impact of Value Categories When Binding References
	namespace sec_8_3
	{
		std::string creatString();				// forward declaration
		std::string foo1(const std::string&);	// forward declaration
		std::string foo2(std::string&);			// forward declaration

		void run()
		{
			const std::string& r1{ creatString() };	// OK
			std::string& r2{ creatString() };		// ERROR

			foo1(std::string{"hello"});				// OK
			foo2(std::string{"hello"});				// ERROR
		}
	}

	// Overload Resolution with RValue References
	namespace sec_8_3_1
	{
		class X {};

		X v{};
		const X c{};

		void f(const X&);	// read only access
		void f(X&);			// OUT parameter (usually long living object)
		void f(X&&);		// can steal value (object usually about to die)
		void f(const X&&);	// no clear semantic meaning

		void run()
		{
			f(v);				// f(X&);
			f(c);				// f(const X&);
			f(std::move(v));	// f(X&&);
			f(std::move(c));	// f(const X&&);
		}
	}
}