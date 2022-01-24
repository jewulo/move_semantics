#pragma once

#include <iostream>
#include <string>

// Perfect Passing with auto&&
namespace chapter_11
{
	// Default Perfect Passing
	namespace sec_11_1_1
	{
		void process(const std::string&)
		{
			std::cout << "process(const std::string&)\n";
		}
		void process(std::string&)
		{
			std::cout << "process(std::string&)\n";
		}
		void process(std::string&&)
		{
			std::cout << "process(std::string&&)\n";
		}

		const std::string& compute_const_lref(const std::string& str)
		{
			return str;
		}
		std::string& compute_lref(std::string& str)
		{
			return str;
		}
		std::string&& compute_rref(std::string&& str)
		{
			return std::move(str);
		}
		std::string compute_value(const std::string str)
		{
			return str;
		}
		const std::string&& compute_const_rref(std::string&& str)
		{
			return std::move(str);
		}
		const std::string compute_const_value(const std::string str)
		{
			return str;
		}

		void run()
		{
			process(compute_const_lref("tmp"));			// calls process(const std::string&)

			std::string str{ "lvalue" };
			process(compute_lref(str));					// calls process(std::string&)

			process(compute_rref("tmp"));				// calls process(std::string&&)
			process(compute_rref(std::move(str)));		// calls process(std::string&&)

			process(compute_value("tmp"));				// calls process(std::string&&)

			// const disables move-semantics
			process(compute_const_rref("tmp"));			// calls process(const std::string&)
			process(compute_const_value("tmp"));		// calls process(const std::string&)
		}
	}
	// Universal References with auto&&
	namespace sec_11_2
	{
		template <typename T>
		T compute(T& arg)
		{
			return arg;
		}

		template <typename T>
		void process(T&& arg)
		{}
		
		struct T1 {};

		void run()
		{
			T1 t;
			// pass return value of compute() to process()
			process(compute(t));	// OK, uses perfect forwarding of returned values

			// pass return value of compute() to process() with some delay
			auto&& ret1 = compute(t);	// initialize a universal reference
			process(ret1);
			process(std::forward<decltype(ret1)>(ret1));

			// use brace initialisation compute() to process() with some delay
			auto&& ret2{ compute(t) };	// initialize a universal reference
			process(ret2);
			process(std::forward<decltype(ret2)>(ret2));
		}
	}
	// Type Deduction of auto&&
	namespace sec_11_2_1
	{
		auto&& ref{10};	// ref is a universal/forwarding reference 

		template <typename T>
		void callFoo(T&& ref)	// ref is a universal/forwarding reference 
		{}

		std::string str{ "" };

		// forward declarations
		std::string ret_by_value() { return str; }
		std::string& ret_by_ref() { return str; }
		std::string&& ret_by_ref_ref() { return std::move(str); }
		const std::string& ret_by_const_ref() { return str; }
		const std::string&& ret_by_const_ref_ref() { return std::move(str); }

		std::string foo(int){ return str; }
		void run()
		{
			std::string s{ "" };

			auto&& r1{ s };							// std::string&
			auto&& r2{ std::move(s) };				// std::string&&

			auto&& r3{ ret_by_value() };			// std::string&&
			auto&& r4{ ret_by_ref() };				// std::string&
			auto&& r5{ ret_by_ref_ref() };			// std::string&&
			auto&& r6{ ret_by_const_ref() };		// const std::string&
			auto&& r7{ ret_by_const_ref_ref() };	// const std::string&&

			auto&& r8{ "hello" };	// const char(&)[]
			auto&& r9{ foo };		// lvalue of type std::string(&)(int)
		}
	}
	
	// Perfectly Forwarding an auto&&
	namespace sec_11_2_2
	{
		void foo(const std::string&) {}
		void foo(std::string&&) {}

		template <typename T>
		void callFoo(T&& ref)	// ref is a universal/forwarding reference 
		{
			foo(std::forward<T>(ref));	// becomes foo(std::move(ref)) for passed rvalues
		}

	}

}
