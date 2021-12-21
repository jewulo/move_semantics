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
				values.push_back(arg);
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

	// Rvalue References of Parameters in Full Specializations
	namespace sec_10_2_3b
	{
		// Note:
		// You have to declare/define full specializations of member
		// function template outside the class definitions
		template <typename T>
		class Cont {
		private:
			std::vector<std::string> coll;
		public:
			// primary template
			template <typename U>
			void insert(U&& v) {	// universal reference
				coll.push_back(std::forward<U>(v));
			}
		};

		// full specializations for Cont<T>::insert<>()
		// - have to be outside the class
		// - need specializations for rvalues and lvalues

		// specialization for rvalue, where T = std::string
		template<>
			template<>
		void Cont<std::string>::insert<>(std::string&& v)
		{
			coll.push_back(std::move(v));
		}
		// specialization for lvalue, where T = std::string
		template<>
			template<>
		void Cont<std::string>::insert<>(const std::string& v)
		{
			coll.push_back(v);
		}

		void run(void)
		{
			Cont<std::string> contstr;
			std::string s{ "string" };
			const std::string cs{ "const string" };

			contstr.insert(s);		// calls insert<std::string&>(std::string&)
			contstr.insert(cs);		// calls insert<const std::string&>(const std::string&)

			contstr.insert(std::string{ "string" });	// calls insert<std::string&&>(std::string&&)
			contstr.insert(std::move(s));				// calls insert<std::string&&>(std::string&&)
			contstr.insert(std::move(cs));				// calls insert<const std::string&&>(const std::string&&)

			// when there are no specialisations only the universal template is called
			// by applying reference folding rules.
			// Note that Cont<> was not specialized fot int.
			/*Cont<int> contint;
			int i{ 0 };
			const int ci{ 1 };

			contint.insert(i);
			contstr.insert(ci);

			contint.insert(int{0});
			contstr.insert(std::move(i));*/
		}
	}

	// How the Standard Specifies Perfect Forwarding
	namespace sec_10_3
	{
		template <typename T>
		void g(const T& coll)
		{}
		template <typename T>
		void g(T& coll)
		{}
		//template <typename T>
		//void g(T&& coll)
		//{}

		// This is why it is called a Universal References.
		template <typename T>
		void f(T&& arg)				// arg is universal/forwarding reference
		{
			g(std::forward<T>(arg));	// perfectly forward (move() only for passed rvalues)
		}

		class MyType{};
		void func(int){}

		void run(void)
		{
			{
				MyType v;

				f(MyType{});				// T is deduced as MyType, so arg is declared as MyType&&
				f(std::move(v));			// T is deduced as MyType, so arg is declared as MyType&&
			}

			{
				MyType v;
				const MyType c;

				f(v);			// T is deduced as MyType& and arg as this type
				f(c);			// T is deduced as const MyType& and arg as this type
			}

			{
				MyType v;
				const MyType c;

				f(v);				// T and arg are MyType&, forward() has no effect in this case 
				f(c);				// T and arg are const MyType&, forward() has no effect in this case 
				f(MyType{});		// T is MyType, arg is MyType&&, forward() is equivalent to move()
				f(std::move(v));	// T is MyType, arg is MyType&&, forward() is equivalent to move()
			}

			{
				// NOTE: string literals are lvalues, so we deduce T and arg as follows
				f("hi");				// lvalue passed, so T and arg have type const char(&)[3]
				f(std::move("hi"));		// xvalue passed, so T is deduced as const char[3]
										//				  and arg have type const char(&&)[3]
			}

			{
				// References to functions are always lvalues
				f(func);				// lvalue passed to f(), so T and arg have type void(&)(int)
				f(std::move(func));		// lvalue passed to f(), so T and arg have type void(&)(int)
			}

		}
	}
	// Explicit Specification of Types for Universal Refernces
	namespace sec_10_3_1a
	{
		template <typename T>
		void g(const T& coll)
		{}
		template <typename T>
		void g(T& coll)
		{}

		template <typename T>
		void f(T&& arg)				// arg is universal/forwarding reference
		{
			g(std::forward<T>(arg));	// perfectly forward (move() only for passed rvalues)
		}

		void run(void)
		{
			std::string s{ "Hello" };

			/*
			* f<std::string>(...);			// arg is a raw rvalue reference binding to rvalues only
			* f<std::string&>(...);			// arg is an lvalue reference binding to non-const lvalues only
			* f<const std::string&>(...);	// arg is an const lvalue reference binding to everything
			* f<std::string&&>(...)			// arg is a raw rvalue reference binding to rvalues only
			*/
			//f<std::string>(s);				// ERROR: cannot bind rvalue reference to lvalue
			f<std::string&>(s);					// OK: does not move and forward s
			f<std::string>(std::move(s));		// OK: does move and forward s
			f<std::string&&>(std::move(s));		// OK: does move and forward s
			// The last two calls are considered equivalent
		}
	}
	// Explicit Specification of Types for Universal Refernces
	namespace sec_10_3_1b
	{
		// The rules for Explicit Specification of Types for Universal Refernces
		// also applies in C++20 with auto&& parameters.
		template <typename T>
		void g(const T& coll)
		{}
		template <typename T>
		void g(T& coll)
		{}

		void f(auto&& arg)				// arg is universal/forwarding reference
		{
			g(std::forward<decltype(arg)>(arg));	// perfectly forward (move() only for passed rvalues)
		}

		void run(void)
		{
			std::string s{ "Hello" };

			/*
			* f<std::string>(...);			// arg is a raw rvalue reference binding to rvalues only
			* f<std::string&>(...);			// arg is an lvalue reference binding to non-const lvalues only
			* f<const std::string&>(...);	// arg is an const lvalue reference binding to everything
			* f<std::string&&>(...)			// arg is a raw rvalue reference binding to rvalues only
			*/
			//f<std::string>(s);				// ERROR: cannot bind rvalue reference to lvalue
			f<std::string&>(s);					// OK: does not move and forward s
			f<std::string>(std::move(s));		// OK: does move and forward s
			f<std::string&&>(std::move(s));		// OK: does move and forward s
			// The last two calls are considered equivalent
		}
	}
	// Conflicting Template Parameter Deduction with Universal Refernces
	namespace sec_10_3_2{}
	// Problem:
	namespace sec_10_3_2a
	{
		//template <typename T>
		//void insert(std::vector<T>& vec, T&& elem)
		//{
		//	vec.push_back(std::forward<T>(elem));
		//}

		//void run(void)
		//{
		//	std::vector<std::string> coll;
		//	std::string s;

		//	insert(coll, s);	// ERROR: No matching function call
		//}
	}
	// Solution 1:
	namespace sec_10_3_2b
	{
		template <typename T>
		void insert(std::vector<std::remove_reference_t<T>>& vec, T&& elem)
		{
			vec.push_back(std::forward<T>(elem));
		}

		void run(void)
		{
			std::vector<std::string> coll;
			std::string s;

			insert(coll, s);	// OK: with T deduced as std::string& vec now binds to coll
		}
	}
	// Solution 2:
	namespace sec_10_3_2c
	{
		template <typename T1, typename T2>
		void insert(std::vector<T1>& vec, T2&& elem)
		{
			vec.push_back(std::forward<T2>(elem));
		}

		void run(void)
		{
			std::vector<std::string> coll;
			std::string s;

			insert(coll, s);	// OK: with T deduced as std::string& vec now binds to coll
		}
	}
	// Solution 3:
	namespace sec_10_3_2d
	{
		template <typename Coll, typename T>
		void insert(Coll& vec, T&& elem)
		{
			vec.push_back(std::forward<T>(elem));
		}

		void run(void)
		{
			std::vector<std::string> coll;
			std::string s;

			insert(coll, s);	// OK: 
		}
	}

	// Pure RValue References of Generic Types
	namespace sec_10_3_3a
	{
		class MyType {};

		template<typename T>	// primary template
		void foo(T&& arg)		// arg is a universal reference
		{}

		template <typename T>
		requires (!std::is_lvalue_reference_v<T>)	// bind to rvalues only
		void callFoo(T&& arg)
		{
			foo(std::forward<T>(arg));
		}

		void run(void)
		{
			MyType m;			
			const MyType cm;

			//callFoo(m);		// ERROR: passing an lvalue
			//callFoo(cm);		// ERROR: passing a const lvalue

			callFoo(MyType{});	// passing an rvalue
			callFoo(MyType());	// passing an rvalue

			callFoo(std::move(m));	// passing an xvalue
			callFoo(std::move(cm));	// passing an const xvalue - semantically stupid
		}
	}
	namespace sec_10_3_3b
	{
		class MyType {};

		template<typename T>	// primary template
		void foo(T&& arg)		// arg is a universal reference
		{}
		// before C++20, the type trait std::enable_if<> had to be used 
		template <typename T,
				  typename
					= typename std::enable_if<!std::is_lvalue_reference<T>::value
											>::type>	// bind to rvalues only
		void callFoo(T&& arg)
		{
			foo(std::forward<T>(arg));
		}

		void run(void)
		{
			MyType m;
			const MyType cm;

			//callFoo(m);		// ERROR: passing an lvalue
			//callFoo(cm);		// ERROR: passing a const lvalue

			callFoo(MyType{});	// passing an rvalue
			callFoo(MyType());	// passing an rvalue

			callFoo(std::move(m));	// passing an xvalue
			callFoo(std::move(cm));	// passing an const xvalue - semantically stupid
		}
	}

	// Nasty Details of Perfect Forwarding
	namespace sec_10_4
	{
		void run(void)
		{}
	}
	// Universal versus Forwarding References
	namespace sec_10_4_1
	{
		void run(void)
		{}
	}
	// Why && for Both Ordinary Rvalues and Universal References?
	namespace sec_10_4_2
	{
		void run(void)
		{}
	}
}

