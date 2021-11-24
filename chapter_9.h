#pragma once
#include <type_traits>

// Perfect Forwarding
namespace chapter_9
{	
	// 9.1 Motivation for Perfect Forwarding
	// What we Need to Perfectly Forward Arguments
	namespace sec_9_1_1a
	{
		class X {
			int m_x;
		public:
			X(int x):m_x{x}{}
		};

		void foo(const X& x){}		// for constant values (read-only access)
		void foo(X& x)		{}		// for variable values (out parameters)
		void foo(X&& x)		{}		// for values that are no longer used (move semantics)

		void run(void)
		{
			X v{1};
			const X c{1};

			foo(v);					// calls foo(X&)
			foo(c);					// calls foo(const X&)
			foo(X{1});				// calls foo(XX&&)
			foo(std::move(v));		// calls foo(XX&&)
			foo(std::move(c));		// calls foo(const X&)
		}
	}
	namespace sec_9_1_1b
	{
		class X {
			int m_x;
		public:
			X(int x) :m_x{ x } {}
		};

		void foo(const X& x) {}		// for constant values (read-only access)
		void foo(X& x) {}			// for variable values (out parameters)
		void foo(X&& x) {}			// for values that are no longer used (move semantics)

		void callFoo(const X& arg) {	// args binds to all const objects
			foo(arg);					// calls foo(const X&)
		}
		void callFoo(X& arg) {			// args binds to lvalues
			foo(arg);					// calls foo(X&)
		}
		void callFoo(X&& arg) {			// args binds to rvalues
			foo(std::move(arg));		// needs std::move() to call foo(X&&)
		}

		void run(void)
		{
			X v{ 1 };
			const X c{ 1 };

			callFoo(v);					// calls foo(X&)
			callFoo(c);					// calls foo(const X&)
			callFoo(X{ 1 });			// calls foo(XX&&)
			callFoo(std::move(v));		// calls foo(XX&&)
			callFoo(std::move(c));		// calls foo(const X&)
		}
	}
	// Pefectly Forwarding const Rvalue References
	namespace sec_9_1_1c
	{
		class X {
			int m_x;
		public:
			X(int x) :m_x{ x } {}
		};

		void foo(const X& x) {}		// for constant values (read-only access)
		void foo(X& x) {}			// for variable values (out parameters)
		void foo(X&& x) {}			// for values that are no longer used (move semantics)
		void foo(const X&& x) {}	// semanticly meaningless type

		void callFoo(const X& arg) {	// args binds to all const objects
			foo(arg);					// calls foo(const X&)
		}
		void callFoo(X& arg) {			// args binds to lvalues
			foo(arg);					// calls foo(X&)
		}
		void callFoo(X&& arg) {			// args binds to rvalues
			foo(std::move(arg));		// needs std::move() to call foo(X&&)
		}
		void callFoo(const X&& arg) {	// args binds to const rvalues
			foo(std::move(arg));		// needs std::move() to call foo(const X&&)
		}

		void run(void)
		{
			X v{ 1 };
			const X c{ 1 };

			callFoo(v);					// calls foo(X&)
			callFoo(c);					// calls foo(const X&)
			callFoo(X{ 1 });			// calls foo(XX&&)
			callFoo(std::move(v));		// calls foo(XX&&)
			callFoo(std::move(c));		// calls foo(const X&&)
		}
	}

	// 9.2 Implementing Perfect Forwarding
	namespace sec_9_2a
	{ 
		class X {
			int m_x;
		public:
			X(int x) :m_x{ x } {}
		};

		void foo(const X& x) {}		// for constant values (read-only access)
		void foo(X& x) {}			// for variable values (out parameters)
		void foo(X&& x) {}			// for values that are no longer used (move semantics)
		void foo(const X&& x) {}	// semanticly meaningless type

		template<typename T>
		void callFoo(T&& arg) {
			foo(std::forward<T>(arg));	// equivalent to foo(std::move(arg)) for passed rvalues
		}

		void run(void)
		{
			X v{ 1 };
			const X c{ 1 };

			callFoo(v);					// calls foo(X&)
			callFoo(c);					// calls foo(const X&)
			callFoo(X{ 1 });			// calls foo(XX&&)
			callFoo(std::move(v));		// calls foo(XX&&)
			callFoo(std::move(c));		// calls foo(const X&&)
		}
	}
	namespace sec_9_2b
	{
		class X1 {
			int m_x;
		public:
			X1(int x) :m_x{ x } {}
		};

		class X2 {
			int m_x;
		public:
			X2(int x) :m_x{ x } {}
		};

		void foo(const X1& x1, const X2& x2) {}		// for constant values (read-only access)
		void foo(X1& x1, X2& x2) {}					// for variable values (out parameters)
		void foo(X1&& x1, X2&& x2) {}				// for values that are no longer used (move semantics)
		void foo(const X1&& x1, const X2&& x2) {}	// semanticly meaningless type

		template<typename T1, typename T2>
		void callFoo(T1&& arg1, T2&& arg2) {
			foo(std::forward<T1>(arg1), std::forward<T2>(arg2));	// equivalent to foo(std::move(arg)) for passed rvalues
		}
		void run(void)
		{
			X1 v1{ 1 };
			X2 v2{ 2 };
			const X1 c1{ 1 };
			const X2 c2{ 2 };

			callFoo(v1, v2);
			callFoo(c1, c2);
		}

	}

	namespace sec_9_2c
	{
		class X1 {
			int m_x;
		public:
			X1(int x) :m_x{ x } {}
		};

		class X2 {
			int m_x;
		public:
			X2(int x) :m_x{ x } {}
		};

		class X3 {
			int m_x;
		public:
			X3(int x) :m_x{ x } {}
		};

		void foo(const X1& x1, const X2& x2, const X3& x3) {}		// for constant values (read-only access)
		void foo(X1& x1, X2& x2, X3& x3) {}							// for variable values (out parameters)
		void foo(X1&& x1, X2&& x2, X3&& x3) {}						// for values that are no longer used (move semantics)
		void foo(const X1&& x1, const X2&& x2, const X3&& x3) {}	// semanticly meaningless type

		// you can use std::forward with variadic templates
		template<typename... Ts>
		void callFoo(Ts&&... args) {
			foo(std::forward<Ts>(args)...);	// equivalent to foo(std::move(arg)) for passed rvalues
		}

		void run(void)
		{
			X1 v1{ 1 };
			X2 v2{ 2 };
			X3 v3{ 3 };

			const X1 c1{ 1 };
			const X2 c2{ 2 };
			const X3 c3{ 3 };

			callFoo(v1, v2, v3);	// calls variadic callFoo
			callFoo(c1, c2, c3);	// calls variadic callFoo
		}
	}

	// 9.2.1 Universal (or Forwarding) Reference
	namespace sec_9_2_1a
	{
		// NOTE:
		// Universal References Bind To All Value Categories
		// They also preserve const

		template<typename T>
		void callFoo(T&& arg);	// arg is a universal reference

		class X {
			int m_x;
		public:
			X(int x) :m_x{ x } {}
		};

		void foo(const X& x) {}		// for constant values (read-only access)
		void foo(X& x) {}			// for variable values (out parameters)
		void foo(X&& x) {}			// for values that are no longer used (move semantics)
		void foo(const X&& x) {}	// semanticly meaningless type

		template<typename T>
		void callFoo(T&& arg) {
			foo(std::forward<T>(arg));	// equivalent to foo(std::move(arg)) for passed rvalues
		}

		void run(void)
		{
			X v{ 1 };
			const X c{ 1 };

			callFoo(v);					// calls foo(X&)
			callFoo(c);					// calls foo(const X&)
			callFoo(X{ 1 });			// calls foo(XX&&)
			callFoo(std::move(v));		// calls foo(XX&&)
			callFoo(std::move(c));		// calls foo(const X&&)
		}
	}

	/*
	namespace sec_9_2_1b
	{
		// NOTE:
		// A generic rvalue qualified with const or volatile is not a universal reference

		template<typename T>
		void callFoo(const T&& arg);	// arg is not a universal reference

		class X {
			int m_x;
		public:
			X(int x) :m_x{ x } {}
		};

		void foo(const X& x) {}		// for constant values (read-only access)
		void foo(X& x) {}			// for variable values (out parameters)
		void foo(X&& x) {}			// for values that are no longer used (move semantics)
		void foo(const X&& x) {}	// semanticly meaningless type

		template<typename T>
		void callFoo(const T&& arg) {
			foo(std::forward<T>(arg));	// equivalent to foo(std::move(arg)) for passed rvalues
		}

		void run(void)
		{
			X v{ 1 };
			const X c{ 1 };

			callFoo(v);					// calls foo(X&)
			callFoo(c);					// calls foo(const X&)
			callFoo(X{ 1 });			// calls foo(XX&&)
			callFoo(std::move(v));		// calls foo(XX&&)
			callFoo(std::move(c));		// calls foo(const X&&)
		}
	}
	*/

	/*
	namespace sec_9_2_1c
	{
		// NOTE:
		// A generic rvalue qualified with const or volatile is not a universal reference

		template<typename T>
		void callFoo(volatile T&& arg);	// arg is not a universal reference

		class X {
			int m_x;
		public:
			X(int x) :m_x{ x } {}
		};

		void foo(const X& x) {}		// for constant values (read-only access)
		void foo(X& x) {}			// for variable values (out parameters)
		void foo(X&& x) {}			// for values that are no longer used (move semantics)
		void foo(const X&& x) {}	// semanticly meaningless type

		template<typename T>
		void callFoo(volatile T&& arg) {
			foo(std::forward<T>(arg));	// equivalent to foo(std::move(arg)) for passed rvalues
		}

		void run(void)
		{
			X v{ 1 };
			const X c{ 1 };

			callFoo(v);					// calls foo(X&)
			callFoo(c);					// calls foo(const X&)
			callFoo(X{ 1 });			// calls foo(XX&&)
			callFoo(std::move(v));		// calls foo(XX&&)
			callFoo(std::move(c));		// calls foo(const X&&)
		}
	}
	*/

	// std::forward<>()
	namespace sec_9_2_2
	{

	}
}
