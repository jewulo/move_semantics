#pragma once

#include <iostream>
#include <string>

// Perfect Returning with decltype(auto)
namespace chapter_12
{
	namespace sec_12_1
	{
		auto foo(const std::string& val)
		{
			return val;
		}
		auto foo(std::string&& val)
		{
			return val;
		}

		// type information is lost with ordinary auto return type
		template <typename T>
		auto callFoo(T&& ref)
		{
			return foo(std::forward<T>(ref));	// becomes foo(std::move(ref)) for passed rvalues
		}

		void run()
		{
			std::string str1{ "Hello World" };			
			auto ret1{ callFoo(str1) };

			const std::string str2{ "Hello World" };
			auto ret2{ callFoo(str2) };

			auto ret3{ callFoo(std::string{ "Hello World" }) };
		}
	}
	// decltype(auto)
	namespace sec_12_2a
	{
		void run()
		{			
			std::string s = "Hello";
			std::string& r = s;

			// initialised with name
			decltype(auto) da1 = s;				// std::string
			decltype(auto) da2(s);				// std::string
			decltype(auto) da3{ s };			// std::string
			decltype(auto) da4 = r;				// std::string&

			// initialised with expression
			decltype(auto) da5 = std::move(s);	// std::string&&
			decltype(auto) da6 = s+s;			// std::string
			decltype(auto) da7 = s[0];			// char&
			decltype(auto) da8 = (s);			// std::string&
		}
	}
	// using just auto [without decltype(auto)]
	namespace sec_12_2b
	{
		void run()
		{
			std::string s = "Hello";
			std::string& r = s;

			// initialised with name
			auto da1 = s;				// std::string
			auto da2(s);				// std::string
			auto da3{ s };				// std::string
			auto da4 = r;				// std::string

			// initialised with expression
			auto da5 = std::move(s);	// std::string
			auto da6 = s + s;			// std::string
			auto da7 = s[0];			// char
			auto da8 = (s);				// std::string
		}
	}
	// const qualifiers with decltype(auto)
	namespace sec_12_2c
	{
		void run()
		{
			const std::string s = "Hello";
			const std::string& r = s;

			// initialised with name
			decltype(auto) da1 = s;				// const std::string
			decltype(auto) da2(s);				// const std::string
			decltype(auto) da3{ s };			// const std::string
			decltype(auto) da4 = r;				// const std::string&

			// initialised with expression
			decltype(auto) da5 = std::move(s);	// const std::string&&
			decltype(auto) da6 = s + s;			// std::string
			decltype(auto) da7 = s[0];			// const char&
			decltype(auto) da8 = (s);			// const std::string&
		}
	}
	// const qualifiers with auto
	namespace sec_12_2d
	{
		void run()
		{
			const std::string s = "Hello";
			const std::string& r = s;

			// initialised with name
			auto da1 = s;				// std::string
			auto da2(s);				// std::string
			auto da3{ s };				// std::string
			auto da4 = r;				// std::string

			// initialised with expression
			auto da5 = std::move(s);	// std::string
			auto da6 = s + s;			// std::string
			auto da7 = s[0];			// char
			auto da8 = (s);				// std::string
		}
	}
	// decltype(auto) cannot have qualifiers applied
	namespace sec_12_2e
	{
		void run()
		{
			std::string s = "Hello";

			decltype(auto) da1{ s };			// std::string
			//const decltype(auto) da2{ s };	// ERROR: you cannot apply const
			//decltype(auto)* da3{ &s };		// ERROR: you cannot make a pointer
		}
	}
	// Return Type decltype(auto)
	namespace sec_12_2_1
	{
		template <typename Func, typename... Args>
		decltype(auto) call(Func f, Args&&... args)
		{
			//.....

			return f(std::forward<Args>(args)...);
		}

		std::string next_string()
		{
			return "Let's Dance";
		}

		std::ostream& print(std::ostream& strm, const std::string& val)
		{
			return strm << "value: " << val;
		}

		std::string&& return_arg(std::string&& arg)
		{
			return std::move(arg);
		}

		void run()
		{
			auto s = call(next_string);						// call() returns temporary object

			auto&& ref = call(return_arg, std::move(s));	// call() returns rvalue reference to s
			/*
			* without the auto&& compiler flags: 
			* 'auto doesn't deduce references. A possible intended copy is being made'
			* auto ref = call(return_arg, std::move(s));	// call() returns rvalue reference to s
			*/
			std::cout << "s:	" << s << '\n';
			std::cout << "ref:	" << ref << '\n';

			auto str = std::move(ref);						// move value from s and ref to str
			std::cout << "s:	" << s << '\n';
			std::cout << "ref:	" << ref << '\n';
			std::cout << "str:	" << str << '\n';

			call(print, std::cout, str) << '\n';			// call() returns reference to std::cout
		}
	}
	// Deferred Perfect Returning
	namespace sec_12_2_2
	{
		template <typename Func, typename... Args>
		decltype(auto) call(Func f, Args&&... args)
		{
			decltype(auto) ret{ f(std::forward<Args>(args)...) };
			// ...
			if constexpr (std::is_rvalue_reference_v<decltype(ret)>) {
				return std::move(ret);	// move xvalue returned by f() to caller
			}
			else {
				return ret;				// return plain value or lvalue reference
			}
		}
	}
	// Perfect Forwarding and Returning with Lambdas
	namespace sec_12_2_3
	{
		void run()
		{
			auto f = [](auto f, auto&&... args) -> decltype(auto)
			{
				decltype(auto) ret{ f(std::forward<Args>(args)...) };
				// ...
				if constexpr (std::is_rvalue_reference_v<decltype(ret)>) {
					return std::move(ret);	// move xvalue returned by f() to caller
				}
				else {
					return ret;				// return plain value or lvalue reference
				}
			};
		}
	}
	// !!!! DO NOT PUT ADDITIONAL PARENTHESES AROUND A RETURNED NAME
	// THIS IS BECAUSE decltype(auto) ALWAYS DEDUCES AN LVALUE REFERENCE
	namespace sec_12_2_3b
	{
		void run()
		{
			auto f = [](auto f, auto&&... args) -> decltype(auto)
			{
				decltype(auto) ret{ f(std::forward<Args>(args)...) };
				// ...
				if constexpr (std::is_rvalue_reference_v<decltype(ret)>) {
					return std::move(ret);	// move xvalue returned by f() to caller
				}
				else {
					return (ret);				// FATAL RUNTIME ERROR: always returns an lvalue reference
				}
			};
		}
	}

}
