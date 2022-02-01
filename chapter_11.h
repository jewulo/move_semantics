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

			// pass return value of compute() to process() with some delay
			auto&& ret{ compute(t) };	// initialize a universal reference with the return value
			process(std::forward<decltype(ret)>(ret)); // perfectly forward the return value
			// expands to 
			process(std::move(ret));	// if and only if compute() returned an rvalue
		}
	}

	// auto&& as a Non-Forwarding Reference
	namespace sec_11_3{}

	// Universal References for Range-Based for Loop
	namespace sec_11_3_1a
	{
		// Specification of Range-Based for Loop		
		void run()
		{
			// these are equivalent
			{
				std::vector<std::string> coll{ "Tam","Bam" };
				for (const auto& s : coll) {
					std::cout << s << std::endl;
				}
			}

			{
				std::vector<std::string> coll{ "Tam","Bam" };
				auto&& range = coll;		// initialise a universal reference
				auto pos = range.begin();	// to use the given range coll here
				auto end = range.end();		// and here
				for (; pos != end; ++pos) {
					const auto& s = *pos;
					std::cout << s << std::endl;
				}
			}
		}
	}
	// non-const lvalue
	namespace sec_11_3_1b
	{
		// Specification of Range-Based for Loop		
		void run()
		{
			// these are equivalent
			{
				std::vector<int> coll{ 1, 2, 3, 4, 5 };
				for (int& i : coll) {
					std::cout << i*2 << std::endl;
				}

				std::cout << std::endl;

				// check the contents of coll
				for (int i : coll) {
					std::cout << i << std::endl;
				}
			}

			std::cout << std::endl;

			{
				std::vector<int> coll{ 1, 2, 3, 4, 5 };
				auto&& range = coll;		// initialise a universal reference
				auto pos = range.begin();	// to use the given range coll here
				auto end = range.end();		// and here
				for (; pos != end; ++pos) {
					auto& i = *pos;
					std::cout << i * 2 << std::endl;
				}

				std::cout << std::endl;

				// check the contents of coll
				pos = range.begin();	// to use the given range coll here
				end = range.end();		// and here
				for (; pos != end; ++pos) {
					std::cout << *pos << std::endl;
				}
			}
		}
	}
	// const lvalue
	namespace sec_11_3_1c
	{
		// Specification of Range-Based for Loop		
		void run()
		{
			// these are equivalent
			{
				const std::vector<int> coll{ 1, 2, 3, 4, 5 };
				for (int i : coll) {
					std::cout << i * 2 << std::endl;
				}
			}

			{
				const std::vector<int> coll{ 1, 2, 3, 4, 5 };
				auto&& range = coll;		// initialise a universal reference
				auto pos = range.begin();	// to use the given range coll here
				auto end = range.end();		// and here
				for (; pos != end; ++pos) {
					auto& i = *pos;
					std::cout << i * 2 << std::endl;
				}
			}
		}
	}
	// using prvalue
	namespace sec_11_3_1d
	{
		// Specification of Range-Based for Loop		
		void run()
		{
			// these are equivalent
			{
				for (int i : std::vector<int> { 1, 2, 3, 4, 5 }) {
					std::cout << i * 2 << std::endl;
				}
			}

			{
				auto&& range = std::vector<int>{ 1, 2, 3, 4, 5 };		// initialise a universal reference
				auto pos = range.begin();	// to use the given range coll here
				auto end = range.end();		// and here
				for (; pos != end; ++pos) {
					auto& i = *pos;
					std::cout << i * 2 << std::endl;
				}
			}
		}
	}
	// !!!! BUG
	namespace sec_11_3_1e
	{
		std::vector<std::string> create_strings()
		{
			return std::vector<std::string> { "Tam","Bam", "Ram", "Aam" };
		}
		// Specification of Range-Based for Loop		
		void run()
		{
			{
				for (const auto& c : create_strings().at(0)) { // fatal runtime error
					std::cout << c << std::endl;
				}
			}

			{
				auto&& range = create_strings().at(0);	// OOPS: universal reference to reference
				auto pos = range.begin();	// return value of create_strings() destroyed here
				auto end = range.end();
				for (; pos != end; ++pos) {
					const auto& s = *pos;
					std::cout << s << std::endl;
				}
			}
		}
	}
	// !!!! sec_11_3_1e BUG FIX but not displaying properly
	namespace sec_11_3_1f
	{
		std::vector<std::string> create_strings()
		{
			return std::vector<std::string> { "Tam", "Bam", "Ram", "Aam" };
		}
		// Specification of Range-Based for Loop		
		void run()
		{
			{
				std::vector<std::string> coll{ create_strings() };
				for (const auto& c : coll.at(0)) {
					std::cout << c << std::endl;
				}
			}

			{
				std::vector<std::string> coll{ create_strings() };
				auto&& range = coll.at(0);	// OOPS: universal reference to reference
				auto pos = range.begin();	// return value of create_strings() destroyed here
				auto end = range.end();
				for (; pos != end; ++pos) {
					const auto& s = *pos;
					std::cout << s << std::endl;
				}
			}
		}
	}
	// Using the Range-Based for Loop
	namespace sec_11_3_1g
	{
		template <typename Coll, typename T>
		void assign(Coll& coll, const T& value) {
			for (auto& elem : coll) {
				elem = value;
			}
		}
		template <typename Coll>
		void display(Coll& coll) {
			for (auto& elem : coll) {
				std::cout << elem << std::endl;
			}
		}

		void run()
		{
			std::vector<int> coll1{ 1, 2, 3, 4, 5 };
			display(coll1);
			assign(coll1, 0);
			display(coll1);

			std::vector<std::string> coll2{ "Tam", "Bam", "Ram", "Aam" };
			display(coll2);
			assign(coll2, "Fammm");
			display(coll2);
		}
	}
	// it doesn't work for std::vector<bool>
	//namespace sec_11_3_1h
	//{
	//	template <typename Coll, typename T>
	//	void assign(Coll& coll, const T& value) {
	//		for (auto& elem : coll) {
	//			elem = value;
	//		}
	//	}

	//	template <typename Coll>
	//	void display(Coll& coll) {
	//		for (auto& elem : coll) {
	//			std::cout << elem << std::endl;
	//		}
	//	}

	//	void run()
	//	{
	//		std::vector<bool> coll1{ true, false, true, false };
	//		assign(coll1, false);	//	ERROR: cannot bind non-const lvalue reference to an rvalue 
	//		display(coll1);
	//	}
	//}
	// solution for std::vector<bool>
	namespace sec_11_3_1i
	{
		template <typename Coll, typename T>
		void assign(Coll& coll, const T& value) {
			for (auto&& elem : coll) {	// note: universal reference support proxy element types
				elem = value;
			}
		}

		template <typename Coll>
		void display(Coll& coll) {
			for (auto&& elem : coll) {	// note: universal reference support proxy element types
				std::cout << elem << std::endl;
			}
		}

		void run()
		{
			std::vector<bool> coll1{ true, false, true, false };

			std::cout << std::boolalpha;
			
			display(coll1);
			assign(coll1, false);
			display(coll1);

			std::cout << std::boolalpha;
		}
	}
	// Perfect Forwarding in Lambdas
	namespace sec_11_4a
	{
		void foo(const std::string& s)
		{
			std::cout << "foo(const std::string&) : " << s << std::endl;
		}
		void foo(std::string&& s)
		{
			std::cout << "foo(std::string&&) : " << s << std::endl;
		}

		void run()
		{
			auto callFoo = [](auto&& arg) {				// arg is a universal/forwarding reference
				foo(std::forward<decltype(arg)>(arg));
			};

			std::string s{"BLM"};
			callFoo(s);
			callFoo(std::move(s));
		}
	}
	// using variadics
	namespace sec_11_4b
	{
		void foo(const std::string& s)
		{
			std::cout << "foo(const std::string&) : " << s << std::endl;
		}
		void foo(const std::string& s1, const std::string& s2)
		{
			std::cout << "foo(const std::string& s1, const std::string& s2) : "
				<< s1
				<< " "
				<< s2
				<< std::endl;
		}
		void foo(const std::string& s1, const std::string& s2, const std::string& s3)
		{
			std::cout << "foo(const std::string& s1, const std::string& s2, const std::string& s3) : "
				<< s1
				<< " "
				<< s2
				<< " "
				<< s3
				<< std::endl;
		}

		void run()
		{
			auto callFoo = [](auto&&... arg) {				// arg is a universal/forwarding reference
				foo(std::forward<decltype(arg)>(arg)...);
			};

			std::string s1{ "BLM1" };
			std::string s2{ "BLM2" };
			std::string s3{ "BLM3" };
			callFoo(s1);
			callFoo(s1, s2);
			callFoo(s1, s2, s3);
		}
	}

	namespace sec_11_4c
	{
		void foo(const std::string& s)
		{
			std::cout << "foo(const std::string&) : " << s << std::endl;
		}
		void foo(std::string&& s)
		{
			std::cout << "foo(std::string&&) : " << s << std::endl;
		}

		void run()
		{
			auto callFoo = [](const auto&& arg) {				// arg is not a universal/forwarding reference
				foo(std::forward<decltype(arg)>(arg));
			};

			std::string s{ "BLM" };
			//callFoo(s);				// ERROR: not a universal/forwarding reference
			callFoo(std::move(s));
		}
	}

	// Using auto&& in C++20 Function Declarations
	// substituting auto&& with template T&&
	namespace sec_11_5a
	{
		void foo(const std::string& s)
		{
			std::cout << "foo(const std::string&) : " << s << std::endl;
		}
		void foo(std::string&& s)
		{
			std::cout << "foo(std::string&&) : " << s << std::endl;
		}

		void callFoo(auto&& arg) {				// arg is a universal/forwarding reference
			foo(std::forward<decltype(arg)>(arg));
		};

		void run()
		{
			std::string s{ "BLM" };

			//callFoo<std::string>(s);				// ERROR: cannot bind rvalue to lvalue
			callFoo<std::string&>(s);				// OK: does not move and forward s
			callFoo<std::string>(std::move(s));		// OK: does move and forward s
			callFoo<std::string&&>(std::move(s));	// OK: does move and forward s
		}
	}
	namespace sec_11_5b
	{
		void foo(const std::string& s)
		{
			std::cout << "foo(const std::string&) : " << s << std::endl;
		}
		void foo(std::string&& s)
		{
			std::cout << "foo(std::string&&) : " << s << std::endl;
		}
		template<typename T>
		void callFoo(T&& arg) {				// arg is a universal/forwarding reference
			foo(std::forward<decltype(arg)>(arg));
		};

		void run()
		{
			std::string s{ "BLM" };

			//callFoo<std::string>(s);				// ERROR: cannot bind rvalue to lvalue
			callFoo<std::string&>(s);				// OK: does not move and forward s
			callFoo<std::string>(std::move(s));		// OK: does move and forward s
			callFoo<std::string&&>(std::move(s));	// OK: does move and forward s
		}
	}
}
