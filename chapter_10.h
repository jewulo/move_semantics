#pragma once

#include <iostream>
#include <string>
#include <vector>

// Tricky Details of Perfect Forwarding
namespace chapter_10
{
	// Universal References and const
	namespace sec_10_1_1
	{
		void iterate(std::string::iterator beg, std::string::iterator end)
		{
			std::cout << "do some non-const stuff with the passed range\n";
		}

		void iterate(std::string::const_iterator beg, std::string::const_iterator end)
		{
			std::cout << "do some const stuff with the passed range\n";
		}

		template <typename T>
		void process(T&& coll)
		{
			iterate(coll.begin(), coll.end());
			// NOTE:
			// 
			// we cannot use std::forward or std::move for coll
			// i.e iterate(std::forward(coll.begin()), std::forward(coll.end()));
			// i.e iterate(std::move(coll.begin()), std::move(coll.end()));
			// because we can only use std::forward or std::move once per object
			// 
			// Also, we cannot use std::forward or std::move once for coll
			// because we do not know the evaluation order of the parameters
			// iterate(std::forward(coll.begin()), coll.end());
			// iterate(coll.begin(), std::forward(coll.end()));
		}

		void run(void)
		{
			std::string v{"v"};
			const std::string c{"c"};

			process(v);					// coll binds to a non-const lvalue, iterators passed
			process(c);					// coll binds to a const lvalue, const_iterators passed
			process(std::string{"t"});	// coll binds to a non-const prvalue, iterators passed
			process(std::move(v));		// coll binds to a non-const xvalue, iterators passed
			process(std::move(c));		// coll binds to a const xvalue, const_iterators passed
		}
	}

	// Universal References in Detail
	namespace sec_10_1_2
	{
		// Constness Dependent Code
		template <typename T>
		void foo1(T&& arg)
		{
			if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
				std::cout << "passed argument is const\n";
			}
			else {
				std::cout << "passed argument is not const\n";
			}
		}

		// Value Category Dependent Code
		template <typename T>
		void foo2(T&& arg)
		{
			if constexpr (std::is_lvalue_reference_v<T>) {
				std::cout << "passed argument is lvalue reference\n";
			}
			else {
				std::cout << "passed argument is rvalue reference\n";
			}
		}

		void run(void)
		{
			std::string v{ "v" };
			const std::string c{ "c" };

			foo1(v);
			foo1(c);

			foo2(v);
			foo2(std::string{ "v" });
		}
	}

	// Universal References of Specific Types

	// When the type must match (no implicit conversions allowed)
	namespace sec_10_1_3a
	{
		// This definition is a catch all. Everything will be accepted.
		// This is why it is called a Universal References.
		template <typename T>
		void process_string(T&& coll);

		// .... but we can constrain T by using requires clauses

		// When the type must match (no implicit conversions allowed)
		template <typename T>
		void process_string(T&& coll)
			requires std::is_same_v<std::remove_cvref_t<T>, std::string>
		{}

		void run(void)
		{
			std::string s{ "string" };

			process_string(s);			// OK
			//process_string("string");	// linker error [error LNK2019: unresolved external symbol]
		}
	}

	// When implicit conversions are allowed
	namespace sec_10_1_3b
	{
		// This definition is a catch all. Everything will be accepted.
		// This is why it is called a Universal References.
		template <typename T>
		void process_string(T&& coll);

		// .... but we can constrain T by using requires clauses

		// When implicit conversions are allowed
		template <typename T>
		void process_string(T&& coll)
			requires std::is_convertible_v<T, std::string>
		{}

		void run(void)
		{
			std::string s{ "string" };

			process_string(s);			// OK
			process_string("string");	// OK convert from const char*
		}
	}

	// When explicit conversions should be allowed
	namespace sec_10_1_3c
	{
		// This definition is a catch all. Everything will be accepted.
		// This is why it is called a Universal References.
		template <typename T>
		void process_string(T&& coll);

		// .... but we can constrain T by using requires clauses

		// When implicit conversions are allowed
		template <typename T>
		void process_string(T&& coll)
			requires std::is_constructible_v<std::string, T>
		{}

		void run(void)
		{
			std::string s{ "string" };

			process_string(s);			// OK
			process_string("string");	// OK convert from const char*
		}
	}

	// Conversions in C++11 before C++20 you need std::enable_if<>

	// When implicit conversions are allowed
	namespace sec_10_1_3d
	{
		// When implicit conversions are allowed
		template <typename T,
					typename = std::enable_if<
									std::is_convertible<T, std::string>::value
											>::type>
		void process_string(T&& coll)			
		{}

		void run(void)
		{
			std::string s{ "string" };

			process_string(s);			// OK
			process_string("string");	// OK convert from const char*
		}
	}

	// To restrict types in C++11
	namespace sec_10_1_3e
	{
		// To restrict types in C++11
		template <typename T,
					typename = std::enable_if<
									std::is_same<typename std::decay<T>::type,
													std::string
												>::value
											>::type>
			void process_string(T&& coll)
		{}

		void run(void)
		{
			std::string s{ "string" };

			process_string(s);			// OK
			//process_string("string");	// ERROR: only std::string is allowed
		}
	}

	// Universal or Ordinary Rvalue Reference
	
	// Rvalue References of Members of Generic Types
	namespace sec_10_2_1
	{
		// Note: An rvalue reference to a member type of a template parameter
		// is not a universal reference
		template <typename T>
		void foo(typename T::value_type&& arg);	// T::value_type is not a universal reference

		template <typename T>
		void insert(T& coll, typename T::value_type&& arg)
		{
			coll.push_back(arg);
		}

		void run(void)
		{
			std::vector<std::string> coll;

			insert(coll, std::string{ "prvalue" });		// OK

			std::string str{ "lvalue" };
			//insert(coll, str);			// ERROR T::value_type&& is not a universal reference
			insert(coll, std::move(str));	// OK
		}
	}

	// Rvalue References of Parameters in Class Templates
	namespace sec_10_2_2
	{
		template<typename T>
		class C {
			T&& member;			// member is not a universal reference

			void foo(T&& arg);	// arg is not a universal reference
		};

		template<typename T>
		class Coll {
		private:
			std::vector<T> values;		// member is not a universal reference
		public:
			Coll() = default;

			// function in class template:
			void insert(T&& arg) {
				values.push_back(val);
			}
		};

		void run(void)
		{
			Coll<std::string> coll;

			std::string str{ "lvalue" };
			// coll.insert(str);			// ERROR: T&& of Col<T> is not a universal reference
			coll.insert(std::move(str));	// OK
		}
	}

	// Rvalue References of Parameters in Full Specializations
	namespace sec_10_2_3
	{
		template<typename T>	// primary template
		void foo(T&& arg);		// arg is a universal reference

		template<>						// full sepcialization (for rvalues only)
		void foo(std::string&& arg);	// arg is not a universal reference

		template<>						// full sepcialization (for lvalues only)
		void foo(std::string& arg);		// arg is not a universal reference

		// primary template taking a universal reference
		template<typename Coll, typename T>
		void insert(Coll& coll, T&& arg)
		{
			std::cout << "primary template for type T called\n";
			coll.push_back(arg);
		}

		// full specialization for rvalues of type std::string
		template<>
		void insert(std::vector<std::string>& coll, std::string&& arg)
		{
			std::cout << "full specialization for type std::string&& called\n";
			coll.push_back(arg);
		}

		// full specialization for lvalues of type std::string
		template<>
		void insert(std::vector<std::string>& coll, const std::string& arg)
		{
			std::cout << "full specialization for type const std::string& called\n";
			coll.push_back(arg);
		}

		void run(void)
		{
			std::vector<std::string> coll;

			insert(coll, std::string{ "prvalue" });	// calls full specialization for rvalues
			std::string str{"lvalue"};
			insert(coll, str);						// calls full specialization for lvalues
			insert(coll, std::move(str));			// calls full specialization for rvalues
		}
	}

	// How the Standard Specifies Perfect Forwarding
	namespace sec_10_3
	{ }
}

