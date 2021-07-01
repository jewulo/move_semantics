#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace chapter_5
{
	// Return By Value
	namespace sec_5_1_1
	{
		class Person {
		private:
			std::string m_name;
			
		public:
			Person(const std::string& n)
				:m_name{ n }
			{}
			// This is safe code but, there is an
			// ineffiecient copy for every call of get_name
			std::string get_name() const {
				return m_name;
			}
		};
		void run(void)
		{
			std::vector<Person> coll{
				Person { "Ben Cook" },
				Person { "Jane White" },
				Person { "Jack Black" },
				Person { "" }
			};
			for (const auto& person : coll) {
				if (person.get_name().empty()) {	// OOPS: copies the name
					std::cout << "found empty name\n";
				}
			}
		}
	}
	// Return By Reference
	namespace sec_5_1_2
	{
		class Person {
		private:
			std::string m_name;

		public:
			Person(const std::string& n)
				:m_name{ n }
			{}

			// This is effiecient reference returned for every call of get_name
			const std::string& get_name() const {
				return m_name;
			}
		};

		Person return_person_by_value()
		{
			return Person{ "Ben Cook" };
		}

		void run(void)
		{
			// code below will crash because 'return_person_by_value().get_name()' invokes 
			// get_name() on a temporary of type Person
			for (char c : return_person_by_value().get_name()) { // OOPS: undefined behaviour
				//if (c == ' ') {
				if (std::isspace(c)) {
					std::cout << "found space\n";
				}
			}
		}
	}
	// Using Move Semantics to Solve the Dilemma
	namespace sec_5_1_3
	{		
		class Person {
		private:
			std::string m_name;

		public:
			Person(const std::string& n)
				:m_name{ n }
			{}

			// we overload get_name() with different reference qualifiers

				// NOTE: Return by std::move is fine for methods returning members
				// but not for methods or functions returning local values.
			std::string get_name() && {		// when we no longer need the value
				return std::move(m_name);	// we steal and return by value
			}

			const std::string& get_name() const& {	// in all other cases we give access to
				return m_name;						// the member via lvalue const reference
			}
		};

		Person return_person_by_value()
		{
			return Person{ "Ben Cook" };
		}

		void run(void)
		{
			// code below will work because 'return_person_by_value().get_name()' invokes 
			// get_name() on a temporary of type Person and return a copy via return std::move(m_name).
			for (char c : return_person_by_value().get_name()) {		// safe and fast (uses move())
				//if (c == ' ') {
				if (std::isspace(c)) {
					std::cout << "found space\n";
				}
			}
		}

		void run2(void)
		{
			Person p{ "Ben" };
			std::cout << p.get_name() << std::endl;							// 1) fast (returns reference)
			std::cout << return_person_by_value().get_name() << std::endl;	// 2) fast (uses move)
		}

		void run3(void)
		{
			std::vector<Person> coll {
				Person { "Ben Cook" },
				Person { "Jane White" },
				Person { "Jack Black" },
				Person { "" }
			};
			for (const auto& person : coll) {
				if (person.get_name().empty()) {							// 3) fast (returns reference)
					std::cout << "found empty name\n";
				}
			}
		}

		void run4(void)
		{
			for (char c : return_person_by_value().get_name()) {			// 4) safe and fast (uses move())
				//if (c == ' ') {
				if (std::isspace(c)) {
					std::cout << "found space\n";
				}
			}
		}

		// std::move() for calling member functions
		void run5(void)
		{
			std::vector<std::string> names;

			Person p{ "Ben Cook" };
			names.push_back(p.get_name());				// calls get_name() const&
			names.push_back(std::move(p).get_name());	// calls get_name() && (OK, p no longer used)
		}
	}

	// Overloading on Reference Qualifiers
	namespace sec_5_2
	{
		class C {
		public:
			void foo() const&
			{
				std::cout << "foo() const&\n";
			}
			void foo() &&
			{
				std::cout << "foo() &&\n";
			}
			void foo() &
			{
				std::cout << "foo() &\n";
			}
			void foo() const&&
			{
				std::cout << "foo() const&&\n";
			}
		};

		void run(void)
		{
			C x;			
			x.foo();				// calls foo() &
			C{}.foo();				// calls foo() &&
			std::move(x).foo();		// calls foo() &&

			const C cx;
			cx.foo();				// calls foo() const&
			std::move(cx).foo();	// calls foo() const&&	// COMPILER WARNING
		}

		// WARNING: You cannot overload by both reference and value qualifiers
		// &, && are reference qualifiers
		// const and volatile are a value qualifier
		class C2 {
		public:
			void foo() &&
			{
				std::cout << "foo() &&\n";
			}
			//void foo() const	// ERROR: You cannot overload by both reference and value qualifiers
			//{
			//	std::cout << "foo() const\n";
			//}
		};

	}

	// Reference Qualifiers for Assignment Operators
	namespace sec_5_3
	{
		class MyType {
		public:
			MyType() = default;

			// disable assigning value to temporary objects:
			MyType& operator=(const MyType& str) & = default;
			MyType& operator=(MyType&& str) & = default;

			// because this disables the copy/move constructor also:
			MyType(const MyType&) = default;
			MyType(MyType&&) = default;
		};

		MyType retMyType()
		{
			MyType a;
			return a;
		}

		void run(void)
		{
			MyType a;
			MyType b;

			// assign to lvalues
			a = b;					// this works fine
			a = retMyType();		// this works fine
			// assign to temporaries (assign to rvalues)

			//MyType{} = a;			// this is disabled
			//MyType() = a;			// this is disabled
			//retMyType() = a;		// this is disabled
		}
	}

	// Reference Qualifiers for Other Member Functions
	namespace sec_5_3_2
	{
	}
}
