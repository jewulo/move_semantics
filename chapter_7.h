#pragma once

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

// Move Semantics and noexcept
namespace chapter_7
{
	namespace sec_7_1_1
	{
		class Person {
		private:
			std::string m_name;
		public:
			Person(const char* n)
				: m_name{ n }
			{}
			// print out when we copy or move
			Person(const Person& p)
				: m_name{ p.m_name }
			{
				std::cout << "COPY: " << m_name << '\n';
			}
			Person(Person&& p)
				: m_name{ std::move(p.m_name) }
			{
				std::cout << "MOVE: " << m_name << '\n';
			}
			std::string getName() const {
				return m_name;
			}
		};
		void run()
		{
			std::vector<Person> coll{ "Wolfgang Amadeus Morzart",
									  "Johann Sebastian Bach",
									  "Ludwig van Beethoven" };

			std::cout << "capacity: " << coll.capacity() << '\n';
			coll.push_back("Pjotr Iljitsch Tschaiowski");

			std::cout << std::endl;
		}
	}
	// declaring noexcept is significant to the definition of the move constructor
	namespace sec_7_1_2
	{
		class Person {
		private:
			std::string m_name;
		public:
			Person(const char* n)
				: m_name{ n }
			{}
			// print out when we copy or move
			Person(const Person& p)
				: m_name{ p.m_name }
			{
				std::cout << "COPY: " << m_name << '\n';
			}
			Person(Person&& p) noexcept
				: m_name{ std::move(p.m_name) }
			{
				std::cout << "MOVE: " << m_name << '\n';
			}
			std::string getName() const {
				return m_name;
			}
		};
		void run()
		{
			std::vector<Person> coll{ "Wolfgang Amadeus Morzart",
									  "Johann Sebastian Bach",
									  "Ludwig van Beethoven" };

			std::cout << "capacity: " << coll.capacity() << '\n';
			coll.push_back("Pjotr Iljitsch Tschaiowski");

			std::cout << std::endl;
		}
	}
	// conditional noexcept declartion
	namespace sec_7_1_2b
	{
		class Person {
		private:
			std::string m_name;
		public:
			Person(const char* n)
				: m_name{ n }
			{}
			// print out when we copy or move
			Person(const Person& p)
				: m_name{ p.m_name }
			{
				std::cout << "COPY: " << m_name << '\n';
			}
			Person(Person&& p)
				noexcept(std::is_nothrow_move_constructible_v<std::string>
					&& noexcept(std::cout << m_name))
				: m_name{ std::move(p.m_name) }
			{
				std::cout << "MOVE: " << m_name << '\n';
			}
			std::string getName() const {
				return m_name;
			}
		};
		void run()
		{
			std::vector<Person> coll{ "Wolfgang Amadeus Morzart",
									  "Johann Sebastian Bach",
									  "Ludwig van Beethoven" };

			std::cout << "capacity: " << coll.capacity() << '\n';
			coll.push_back("Pjotr Iljitsch Tschaiowski");

			std::cout << std::endl;
		}
	}
	// default generated move
	namespace sec_7_1_2c
	{
		class Person {
		private:
			std::string m_name;
		public:
			Person(const char* n)
				: m_name{ n }
			{}
			// print out when we copy or move
			Person(const Person& p)
				: m_name{ p.m_name }
			{
				std::cout << "COPY: " << m_name << '\n';
			}
			Person(Person&& p) = default;

			std::string getName() const {
				return m_name;
			}
		};
		void run()
		{
			std::vector<Person> coll{ "Wolfgang Amadeus Morzart",
									  "Johann Sebastian Bach",
									  "Ludwig van Beethoven" };

			std::cout << "capacity: " << coll.capacity() << '\n';
			coll.push_back("Pjotr Iljitsch Tschaiowski");

			std::cout << std::endl;
		}
	}
	// Is noexcept Worth It?
	namespace sec_7_1_3
	{
		struct Str {
			std::string val;

			// ensure each string hass 100 characters
			Str()
				: val(100,'a') // don't use brace initialisation here
			{} 

			// enable copying
			Str(const Str&) = default; // don't use brace initialisation here

			// enable moving (with and without noexcept)
			Str(Str&& s) noexcept
				: val{std::move(s.val)}
			{}
		};

		void run()
		{
			// create vector of 1 Million wrapped strings
			std::vector<Str> coll;
			coll.resize(1000000);

			// measure time to reallocate memory for all elements
			auto t0 = std::chrono::steady_clock::now();
			coll.reserve(coll.capacity() + 1);
			auto t1 = std::chrono::steady_clock::now();

			std::chrono::duration<double, std::milli> d{t1 - t0};
			std::cout << d.count() << "ms\n";
		}
	}
	// Details of noexcept Declarations
	// Rules for Declaring Functions with noexcept
	namespace sec_7_2_1a
	{
		class Base {
		public:
			virtual void foo(int) noexcept;
			//virtual void foo(int);			// ERROR: overload on different noexcept clause only
			virtual void bar(int);
		};

		class Derived : public Base {
		public:
			//virtual void foo(int) override;	// ERROR: override giving up the noexcept guarantee
			virtual void bar(int) noexcept;	// OK (here we have guarantee not to throw)
		};
	}
	namespace sec_7_2_1b
	{
		class Base {
		public:
			virtual void foo(int) noexcept;
		};

		class Derived : public Base {
		public:
			//virtual void foo(int);		// OK: hiding instead of overriding
		};
	}
	namespace sec_7_2_1c
	{
		class Base {
		public:
			virtual void func() noexcept(sizeof(int) < 8);	// might throw if sizeof(int) >= 8
		};

		class Derived : public Base {
		public:
			//virtual void func() noexcept(sizeof(int) < 4) override;	// might throw if sizeof(int) >= 4
		};
	}
	// noexcept for Special Member Functions
	// noexcept for Copying and Moving Special Member Functions
	namespace sec_7_2_2
	{
		class B {
			std::string s;
		};
		void run()
		{
			std::cout << std::boolalpha;
			std::cout << std::is_nothrow_default_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_copy_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_move_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_copy_assignable<B>::value << '\n';
			std::cout << std::is_nothrow_move_assignable<B>::value << '\n';
			std::cout << std::noboolalpha;
		}
	}

	namespace sec_7_2_2b
	{
		class B {
			std::string s;
		public:
			B(const B&)				= default;	// noexcept condition automatically generated
			B(B&&)					= default;	// noexcept condition automatically generated
			B& operator= (const B&)	= default;	// noexcept condition automatically generated
			B& operator= (B&&)		= default;	// noexcept condition automatically generated
		};
		void run()
		{
			std::cout << std::boolalpha;
			std::cout << std::is_nothrow_default_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_copy_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_move_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_copy_assignable<B>::value << '\n';
			std::cout << std::is_nothrow_move_assignable<B>::value << '\n';
			std::cout << std::noboolalpha;
		}
	}
	namespace sec_7_2_2c
	{
		class C {
			std::string s;
		public:
			C(const C&) noexcept = default;						// guarantees not to throw (OK since C++20)
			C(C&&) noexcept(false) = default;					// specifies that it might throw (OK since C++20)
			C& operator= (const C&) noexcept(false) = default;	// specifies that it might throw (OK since C++20)
			C& operator= (C&&) noexcept(false) = default;		// specifies that it might throw (OK since C++20)
		};
		void run()
		{
			std::cout << std::boolalpha;
			std::cout << std::is_nothrow_default_constructible<C>::value << '\n';
			std::cout << std::is_nothrow_copy_constructible<C>::value << '\n';
			std::cout << std::is_nothrow_move_constructible<C>::value << '\n';
			std::cout << std::is_nothrow_copy_assignable<C>::value << '\n';
			std::cout << std::is_nothrow_move_assignable<C>::value << '\n';
			std::cout << std::noboolalpha;
		}
	}
	// noexcept for destructors
	namespace sec_7_2_2d
	{
		class B {
			std::string s;
		public:
			B(const B&) = default;				// noexcept condition automatically generated
			B(B&&) = default;					// noexcept condition automatically generated
			B& operator= (const B&) = default;	// noexcept condition automatically generated
			B& operator= (B&&) = default;		// noexcept condition automatically generated
			~B() {}								// automatically declared as ~B() noexcept
		};
		void run()
		{
			std::cout << std::boolalpha;
			std::cout << std::is_nothrow_default_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_copy_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_move_constructible<B>::value << '\n';
			std::cout << std::is_nothrow_copy_assignable<B>::value << '\n';
			std::cout << std::is_nothrow_move_assignable<B>::value << '\n';
			std::cout << std::noboolalpha;
		}

	}
#include "isnothrowmovable.h"

	// I DID NOT UNDERSTAND THIS SECTION
	// noexcept Declarations in Class Heirarchies
	namespace sec_7_3a
	{
		class Base {
			std::string id;
		public:
			virtual void print() const = 0;	// pure virtual function (forces abstract base class)
			virtual ~Base() = default;
		protected:
			// protected copy and move semantics (also forces abstract base class)
			Base(const Base&) = default;
			Base(Base&&) = default;
			// disable assignment operators (due to the problem of slicing)
			Base& operator= (const Base&) = delete;
			Base& operator= (Base&&) = delete;

		};

		class MemType {};
		class Drv : public Base {
		private:
			MemType member;
			Drv(Drv&& drv) noexcept(std::is_nothrow_move_constructible_v<Base>&&
				std::is_nothrow_move_constructible_v<MemType>);
		};

		void run()
		{
			std::cout << std::boolalpha;
			std::cout << "std::is_nothrow_move_constructible_v<Base>: "
				<< std::is_nothrow_move_constructible_v<Base> << '\n';
			std::cout << "is_nothrow_moveable_v<Base>: "
				<< is_nothrow_movable_v<Base> << '\n';
		}
	}

	namespace sec_7_3b
	{
		class Base {
			std::string id;
		public:
			virtual void print() const = 0;	// pure virtual function (forces abstract base class)
			virtual ~Base() = default;
		protected:
			// protected copy and move semantics (also forces abstract base class)
			Base(const Base&) = default;
			Base(Base&&) = default;
			// disable assignment operators (due to the problem of slicing)
			Base& operator= (const Base&) = delete;
			Base& operator= (Base&&) = delete;

		};
		class MemType {};
		class Drv : public Base {
		private:
			MemType member;
			Drv(Drv&& drv) noexcept(is_nothrow_move_constructible_v<Base>&&
				is_nothrow_move_constructible_v<MemType>);
		};

		void run()
		{
			std::cout << std::boolalpha;
			std::cout << "std::is_nothrow_move_constructible_v<Base>: "
				<< std::is_nothrow_move_constructible_v<Base> << '\n';
			std::cout << "is_nothrow_moveable_v<Base>: "
				<< is_nothrow_movable_v<Base> << '\n';
		}

	}

}
