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

		// A generic rvalue qualified with const or volatile is not a universal reference
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

		// A generic rvalue qualified with const or volatile is not a universal reference
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
		class Person {
		private:
			std::string m_name;
		public:
			Person(const char* n)
				: m_name{ n }
			{}
			// print out when we copy
			Person(const Person& p)
				: m_name{ p.m_name }
			{
				std::cout << "COPY: " << m_name << '\n';
			}
			// print out when we copy
			Person(Person&& p)
				: m_name{ std::move(p.m_name) }
			{
				std::cout << "MOVE: " << m_name << '\n';
			}
			void print() const {
				std::cout << getName() << '\n';
			}
			std::string getName() && {		// when we no longer need the value
				return std::move(m_name);	// we steal and return by value
			}
			const std::string& getName() const& {	// in all other cases
				return m_name;						// we give access to the member
			}
		};

		// In a function that takes a universal reference,
		// we might then benefit from using std::forward as follows
		template <typename T>
		void foo(T&& x)
		{
			x.print();						// OK, no need to forward the passed value category

			x.getName();					// calls getName() const&
			std::forward<T>(x).getName();	// calls getName() && for rvalues (OK, no longer need x)
			std::forward<T>(x).print();
		}
		void run()
		{
			Person p{ "Wolfgang Amadeus Morzart" };

			foo(p);
			std::cout << std::endl;
		}
	}

	// The Effects of Perfect Forwarding
	namespace sec_9_2_3
	{
		// Combining the behaviour of declaring a Universal Reference
		// and using std::forward<>() we get the following behaviour:
		class X{};
		void foo(const X&)	{}	// for constant values (read only access)
		void foo(X&)		{}	// for variable values (out parameters)
		void foo(X&&)		{}	// for values that are no longer used (move semantics)

		template<typename T>
		void callFoo(T&& arg) {
			foo(std::forward<T>(arg));	// equivalent to foo(std::move(arg)) for passed rvalues
		}

		void run()
		{
			X v;
			const X c;

			callFoo(v);				// OK, expands to callFoo(arg), so it calls foo(X&)
			callFoo(c);				// OK, expands to callFoo(arg), so it calls foo(const X&)
			callFoo(X{});			// OK, expands to callFoo(std::move(arg)), so it calls foo(X&&)
			callFoo(std::move(v));	// OK, expands to callFoo(std::move(arg)), so it calls foo(X&&)
			callFoo(std::move(c));	// OK, expands to callFoo(std::move(arg)), so it calls foo(X&&)
		}
	}

	// Rvalue References versus Universal References
	namespace sec_9_3
	{
		class Coll{};
		void foo(Coll&& arg);	// arg is an ordinary rvalue reference of type Coll

		template <typename T>
		void foo(T&& arg);		// arg is a universal/forwarding reference
	}

	// Rvalue References of Actual Types
	namespace sec_9_3_1
	{
		using Coll = std::vector<std::string>;

		void bar(Coll&& arg)
		{
		}

		void foo(Coll&& arg)		// arg is an ordinary rvalue reference of type Coll
		{
			Coll coll;				// coll can't be const (because are it is not template code)

			bar(std::move(coll));	// perfectly forward to bar() (no need to use std::forward<>() here)
		}

		void run()
		{
			Coll v;
			const Coll c;

			//foo(v);				// ERROR, can't bind rvalue reference to lvalue
			//foo(c);				// ERROR, can't bind rvalue reference to lvalue
			foo(Coll{});		// OK, arg binds to a non-const prvalue
			foo(std::move(v));	// OK, arg binds to a non-const xvalue
			//foo(std::move(c));	// ERROR, can't bind non-const rvalue reference to const xvalue
		}
	}

	// Rvalue References of Function Template Parameters
	//namespace sec_9_3_2
	//{
	//	template<typename T>
	//	void bar(T&& arg)
	//	{
	//	}

	//	template<typename T>
	//	void foo(T&& arg)		// arg is a universal/forwarding reference
	//	{
	//		T coll;				// coll may be const

	//		bar(std::forward(coll));	// perfectly forward to bar() (don't use std::move() here)
	//	}

	//	void run()
	//	{
	//		using Coll = std::vector<std::string>;

	//		std::vector<std::string> v;
	//		const std::vector<std::string> c;

	//		foo(v);				// OK, arg binds to a non-const lvalue
	//		foo(c);				// OK, arg binds to a const lvalue
	//		foo(Coll{});		// OK, arg binds to a non-const prvalue
	//		foo(std::move(v));	// OK, arg binds to a non-const xvalue
	//		foo(std::move(c));	// OK, arg binds to a const xvalue
	//	}

	//}

	// Overload Resolution with Universal References
	namespace sec_9_4
	{
		class X {};
		X v;
		const X c;

		void f(const X&);		// read only access
		void f(X&);				// OUT parameter (usually long living objects)
		void f(X&&);			// can steal value (object usually about to die)
		void f(const X&&);		// contradicting semantic meaning
		template <typename T>
		void f(T&&);			// to use perfect forwarding
		// Note that the universal reference is always the second best option.
	}

	// Fixing Overload Resolution with Universal References
	namespace sec_9_4_1
	{
		class X {
		public:
			X() = default;

			X(const X&) {
				std::cout << "copy constructor\n";
			}

			X(X&&) {
				std::cout << "move constructor\n";
			}

			template<typename T>
			X(T&&) {
				std::cout << "universal constructor\n";
			}
		};
		void run()
		{
			X xv;
			const X xc;

			X xcc{ xc };			// OK: calls copy constructor
			X xvc{ xv };			// OK: calls universal constructor
			X xvm{ std::move(xv) };	// OK: calls move constructor
			X xcm{ std::move(xc) };	// OK: calls universal constructor

		}
	}

	// Constraining Universal References since C++20
	namespace sec_9_4_1a
	{
		class X {
		public:
			X() = default;

			X(const X&) {
				std::cout << "copy constructor\n";
			}

			X(X&&) noexcept {
				std::cout << "move constructor\n";
			}

			// using C++20 template constraints
			template<typename T>
				requires (!std::is_same_v<std::remove_cvref_t<T>, X>)
			X(T&&) {
				std::cout << "universal constructor\n";
			}
		};
		void run()
		{
			X xv;
			const X xc;

			X xcc{ xc };			// OK: calls copy constructor
			X xvc{ xv };			// OK: calls universal constructor
			X xvm{ std::move(xv) };	// OK: calls move constructor
			X xcm{ std::move(xc) };	// OK: calls universal constructor

		}

	}

	// Constraining Universal References before C++20
	namespace sec_9_4_1b
	{

		class X {
		public:
			X() = default;

			X(const X&) {
				std::cout << "copy constructor\n";
			}

			X(X&&) noexcept {
				std::cout << "move constructor\n";
			}

			// using template meta programming
			template<typename T,
				typename
				= typename std::enable_if<!std::is_same<typename std::decay<T>::type,
														X>::value
										>::type>
			X(T&&) {
				std::cout << "universal constructor\n";
			}
		};

		void run()
		{
			X xv;
			const X xc;

			X xcc{ xc };			// OK: calls copy constructor
			X xvc{ xv };			// OK: calls universal constructor
			X xvm{ std::move(xv) };	// OK: calls move constructor
			X xcm{ std::move(xc) };	// OK: calls universal constructor

		}

	}

	// Perfect Forwarding in Lambdas
	namespace sec_9_5
	{
		// Combining the behaviour of declaring a Universal Reference
		// and using std::forward<>() we get the following behaviour:
		class X {};
		void foo(const X&) {}	// for constant values (read only access)
		void foo(X&) {}	// for variable values (out parameters)
		void foo(X&&) {}	// for values that are no longer used (move semantics)

		//template<typename T>
		//void callFoo(T&& arg) {
		//	foo(std::forward<T>(arg));	// equivalent to foo(std::move(arg)) for passed rvalues
		//}

		void run()
		{
			// since C++20 you can alswo use template parameters in lambdas
			auto callFoo = []<typename T>(T && arg) {
				foo(std::forward<T>(arg));	// equivalent to foo(std::move(arg)) for passed rvalues
			};

			X v;
			const X c;

			callFoo(v);				// OK, expands to callFoo(arg), so it calls foo(X&)
			callFoo(c);				// OK, expands to callFoo(arg), so it calls foo(const X&)
			callFoo(X{});			// OK, expands to callFoo(std::move(arg)), so it calls foo(X&&)
			callFoo(std::move(v));	// OK, expands to callFoo(std::move(arg)), so it calls foo(X&&)
			callFoo(std::move(c));	// OK, expands to callFoo(std::move(arg)), so it calls foo(X&&)
		}

	}

}
