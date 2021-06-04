#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <ratio>

namespace chapter_4
{
	// Avoid Objects with Names: Favour Values
	namespace sec_4_1
	{		
		struct Mytype {
			int m_v;
			std::string m_n;
		};

		void foo(Mytype v) {};
		void run(void)
		{
			Mytype x{ 42,"hello" };
			// if x is not used afterwards then it is a wasted declaration that
			// disables move semantic optimizations
			foo(x);
			// prefer rvalues to lvalues: this automatically uses move semantics
			foo(Mytype{ 42, "hello" });
			// if a named variable must be used for clarity then you can use std::move
			// if you are sure that the value will not be required later
			foo(std::move(x));
		}
	}

	// When You Cannot Avoid Using Names
	namespace sec_4_1_1
	{
		std::string get_data()
		{
			std::string str{ "foobar" };
			return str;
		}

		void run(void)
		{
			std::vector<std::string> coll1;
			std::vector<std::string> coll2;

			std::string str{ get_data() };
			coll1.push_back(str);					// copy (we still need the value of str)
			coll2.push_back(std::move(str));		// move (no longer need the value of str)
		}

		void run2(void)
		{
			std::string line;
			std::vector<std::string> coll;
			while (std::getline(std::cin, line)) {
				coll.push_back(std::move(line));	// move (we no longer need the value of line)
			}
		}
	}

	// Avoid Unnecessary std::move
	namespace sec_4_2
	{
		std::string foo1(void)
		{
			std::string s{"Hello"};
			//.....
			return std::move(s);	// BAD: Don't do this
		};

		std::string foo2(void)
		{
			std::string s{ "Hello" };
			//.....
			return s;	// GOOD: Optimised by RVO or Move Sematics
		};

		std::string create_string(void)
		{
			std::string s{ "Hello" };
			//.....
			return s;	// GOOD: Optimised by RVO or Move Sematics
		};

		void foo3(void)
		{
			// GOOD: Optimised by RVO or Move Sematics
			std::string s1{ create_string() };

			// BAD: Don't do this
			std::string s2{ std::move(create_string()) };
			//.....

		};

		void run(void)
		{
			foo1();
			foo2();
		}
	}

	// Initialisze Members With Move Semantics
	namespace sec_4_3
	{
	}

	// Initialisze Members The Classical Way
	namespace sec_4_3_1
	{
		class Person {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person(const std::string& f, const std::string& l)
				:m_first{ f }, m_last{ l }
			{}
		};
		void run(void)
		{
			Person p{ "Ben", "Cook" };
		}
	}

	// Using non-const lvalue References
	namespace sec_4_3_1b
	{
		class Person {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person(std::string& f, std::string& l)
				:m_first{ f }, m_last{ l }
			{}
		};
		void run(void)
		{
			// this will not compile, you cannot bind temporaries to non-const lvalues
			//Person p{ "Ben", "Cook" };
		}
	}

	// Initialisze Members via Moved Parameters Passed by Value
	namespace sec_4_3_2
	{
		class Person {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person(std::string f, std::string l)
				:m_first{ std::move(f) }, m_last{ std::move(l) }
			{}
		};
		void run(void)
		{
			Person p1{ "Ben", "Cook" };

			std::string name1{ "Jane" };
			std::string name2{ "White" };
			Person p2{ name1, name2 };	// OK, copy names into parameters and moved into members
		}
	}

	// Initialisze Members via rvalue References
	namespace sec_4_3_3
	{
		class Person {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person(std::string&& f, std::string&& l)
				:m_first{ std::move(f) }, m_last{ std::move(l) }
			{}
		};
		void run(void)
		{
			Person p1{ "Ben", "Cook" }; // pass temporaries

			std::string name1{ "Jane" };
			std::string name2{ "White" };
			Person p2{ std::move(name1), std::move(name2) };	// OK, move names into parameters and moved into members
		}
		// Overloading For rvalue and lvalue References
		void run_2(void)
		{
			// passing temporaries works OK for rvalue References....
			Person p1{ "Ben", "Cook" };

			std::string name1{ "Jane" };
			std::string name2{ "White" };
			// ....but, we cannot pass named objects into rvalue reference parameters
			// because we cannot bind lvalues to rvalue references 
			// Person p2{ name1, name2 };
		}
	}

	// Overload for Rvalue and Lvalue References
	namespace sec_4_3_3b
	{
		class Person {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person(const std::string& f, const std::string& l)
				:m_first{ f }, m_last{ l }
			{}

			Person(const std::string& f, std::string&& l)
				:m_first{ f }, m_last{ std::move(l) }
			{}

			Person(std::string&& f, const std::string& l)
				:m_first{ std::move(f) }, m_last{ l }
			{}

			Person(std::string&& f, std::string&& l)
				:m_first{ std::move(f) }, m_last{ std::move(l) }
			{}
		};
		void run(void)
		{
			// passing temporaries works OK for rvalue References....
			Person p1{ "Ben", "Cook" }; // pass temporaries

			std::string name1{ "Jane" };
			std::string name2{ "White" };
			// OK, move names into parameters and moved into members
			Person p2{ std::move(name1), std::move(name2) };

			std::string name3{ "Jane" };
			std::string name4{ "White" };
			// OK we can pass named objects into lvalue reference parameters
			Person p3{ name1, name2 };

			// copy via char*
			char str1[] = "Jane";
			char str2[] = "White";
			Person p4{ str1, str2 };

			Person p5{ "Jane", "White" };
		}
	}
	
	// Overloading Even for string literals
	// The more parameters you have the more overloading
	// is required to cover all 3 parameter combination reference types (const lref, rref, value)
	// in this case 2 parameters for 3 is 3^2 = 9
	namespace sec_4_3_3c
	{
		class Person {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person(const std::string& f, const std::string& l)
				:m_first{ f }, m_last{ l }
			{}

			Person(const std::string& f, std::string&& l)
				:m_first{ f }, m_last{ std::move(l) }
			{}

			Person(std::string&& f, const std::string& l)
				:m_first{ std::move(f) }, m_last{ l }
			{}

			Person(std::string&& f, std::string&& l)
				:m_first{ std::move(f) }, m_last{ std::move(l) }
			{}

			Person(const char* f, const char* l)
				:m_first{ f }, m_last{ l }
			{}

			Person(const char* f, const std::string& l)
				:m_first{ f }, m_last{ l }
			{}

			Person(const char* f, std::string&& l)
				:m_first{ f }, m_last{ std::move(l) }
			{}

			Person(const std::string& f, const char* l)
				:m_first{ f }, m_last{ l }
			{}

			Person(std::string&& f, const char* l)
				:m_first{ std::move(f) }, m_last{ l }
			{}
		};
		void run(void)
		{
			// passing temporaries works OK for rvalue References....
			Person p1{ "Ben", "Cook" }; // pass temporaries

			std::string name1{ "Jane" };
			std::string name2{ "White" };
			// OK, move names into parameters and moved into members
			Person p2{ std::move(name1), std::move(name2) };

			std::string name3{ "Jane" };
			std::string name4{ "White" };
			// OK we can pass named objects into lvalue reference parameters
			Person p3{ name1, name2 };

			// copy via char*
			char str1[] = "Jane";
			char str2[] = "White";
			Person p4{ str1, str2 };

			Person p5{ "Jane", "White" };

			// pass temporary strings
			Person p6{ std::string { "Jane" }, std::string {"White"} };
			// pass temporary strings using operator ""s
			// operator ""s is defined in std::literals as std::literals::s			
			Person p7{ "Jane"s, "White"s };
			
		}
	}

	// Compare the Different Approaches
	namespace sec_4_3_4
	{
		class Person {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person(const std::string& f, const std::string& l)
				:m_first{ f }, m_last{ l }
			{}

			Person(const std::string& f, std::string&& l)
				:m_first{ f }, m_last{ std::move(l) }
			{}

			Person(std::string&& f, const std::string& l)
				:m_first{ std::move(f) }, m_last{ l }
			{}

			Person(std::string&& f, std::string&& l)
				:m_first{ std::move(f) }, m_last{ std::move(l) }
			{}

			Person(const char* f, const char* l)
				:m_first{ f }, m_last{ l }
			{}

			Person(const char* f, const std::string& l)
				:m_first{ f }, m_last{ l }
			{}

			Person(const char* f, std::string&& l)
				:m_first{ f }, m_last{ std::move(l) }
			{}

			Person(const std::string& f, const char* l)
				:m_first{ f }, m_last{ l }
			{}

			Person(std::string&& f, const char* l)
				:m_first{ std::move(f) }, m_last{ l }
			{}
		};
		// measure num initializations of whatever is currently defined as Person
		std::chrono::nanoseconds measure(int num)
		{
			std::chrono::nanoseconds total_dur{ 0 };
			for (size_t i = 0; i < static_cast<size_t>(num); i++) {
				std::string fname = "a firstname a bit too long for SSO";
				std::string lname = "a lastname a bit too long for SSO";
				// measure how long it takes to create 3 persons in different ways:
				auto t0 = std::chrono::steady_clock::now();
					Person p1 { "a firstname a bit too long for SSO","a lastname a bit too long for SSO" };
					Person p2 { fname, lname };
					Person p3 { std::move(fname), std::move(lname) };
				auto t1 = std::chrono::steady_clock::now();
				total_dur += t1 - t0;
			}
			return total_dur;
		}

		void run(void)
		{
			std::cout << "chapter_4::sec_4_3_4\n";
			int  num = 1000;	// num iterations to measure

			// a few iterations to avoid measuring initial behaviour, ignore return
			auto _ = measure(5);

			// measure (in integral nano and floating point milliseconds)
			std::chrono::nanoseconds ns_dur { measure(num) };
			std::chrono::duration<double, std::milli> ms_dur { ns_dur };

			// print result
			std::cout << num << " iterations take: "
					  << ms_dur.count() << "ms\n";
			std::cout << " 3 inits take on average: "
					  << ns_dur.count() / num << "ns\n";
		}

		// expensive members that will not benefit from move
		class Person2 {
		private:
			std::string m_name;
			std::array<double, 10000> values;	// move can't optimize here
		public:
		};
	}

	// Summary for Member Initialization
	namespace sec_4_3_5
	{
		// Rule for initialising member vectors or lists
		// pass parameter by value and move into member
		class Person1 {
		private:
			std::string m_name;
			std::vector<std::string> m_values;
		public:
			Person1(std::string n, std::vector<std::string> v)
				: m_name{ std::move(n) }, m_values{ std::move(v) }
			{}
		};

		// Rule for initialising member arrays
		// overload on lvalue-ref and rvalue-ref
		class Person2 {
		private:
			std::string m_name;
			std::array<std::string, 1000> m_values;
		public:
			// copy array if passed by lvalue-ref
			Person2(std::string n, const std::array<std::string, 1000>& arr)
				: m_name{ std::move(n) }, m_values{ arr }
			{}
			// move array if passed by rvalue-ref
			Person2(std::string n, std::array<std::string, 1000>&& arr)
				: m_name{ std::move(n) }, m_values{ std::move(arr) }
			{}
		};
	}

	// Should We Now Always Pass by Value and Move?
	// .... it depends.
	namespace sec_4_3_6
	{
		class Person1 {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person1(std::string f, std::string l)
				:m_first{ std::move(f) }, m_last{ std::move(l) }
			{}

			// take by value and move
			void set_first_name(std::string f)
			{
				m_first = std::move(f);
			}
		};
		void run(void)
		{
			Person1 p1{ "Ben", "Cook" };

			std::string name1{ "Ann" };
			std::string name2{ "Constantin Alexander" };
			
			p1.set_first_name(name1);
			p1.set_first_name(name2);
			p1.set_first_name(name1);
			p1.set_first_name(name2);
		}
		class Person2 {
		private:
			std::string m_first;
			std::string m_last;
		public:
			Person2(std::string f, std::string l)
				:m_first{ std::move(f) }, m_last{ std::move(l) }
			{}

			// take by lvalue reference and assign
			void set_first_name(const std::string& f)
			{
				m_first = f;
			}
			// take by rvalue reference and move
			void set_first_name(std::string&& f)
			{
				m_first = std::move(f);
			}

		};
		void run2(void)
		{
			Person2 p1{ "Ben", "Cook" };

			p1.set_first_name("Constantin Alexander");	// would allocate enough memory
			p1.set_first_name("Ann");					// would allocate enough memory
			p1.set_first_name("Constantin Alexander");	// would have to allocate again
		}

		// initialization of members is not the only application of "take by value and move"
		// adding values to a member container would be another application
		class Person3 {
		private:
			std::string m_name;
			std::vector<std::string> m_values;
		public:
			Person3() = default;
			Person3(std::string n, std::vector<std::string> v)
				: m_name{ std::move(n) }, m_values{ std::move(v) }
			{}
			// better pass by value and move to insert a new element
			void add_value(std::string s)
			{
				m_values.push_back(std::move(s));
			}

			void set_name(const std::string& f)
			{
				m_name = f;
			}
		};
		void run3(void)
		{
			Person3 p;

			p.set_name("Constantin Alexander");
			p.add_value("Ann");
			p.add_value(std::string{ "Constantina Alexandra" });
			p.add_value("Annie Bloom"s); // use operator ""s to convert char* to std::string
		}

	}

	// Move Semantics In class Hierarchies
	namespace sec_4_4
	{
		// Implementing a Polymorphic Base Class
		namespace sec_4_4_1
		{
			namespace without_members
			{
				// default delete call the right destructor but it disables default move semantics
				// in this case it doesn't matter because we have no members
				class GeoObj {
				public:
					virtual void draw() const = 0;	// pure virtual function (introducing the API)
					virtual ~GeoObj() = default;	// lets delete call the right destructor
				};
			}
			namespace with_members
			{

				// default delete call the right destructor but it disables default move semantics
				// in this case it does matter because we have a member m_name
				class GeoObj {
				protected:
					std::string m_name;				// name of the geometric object
				public:
					virtual void draw() const = 0;	// pure virtual function (introducing the API)
					virtual ~GeoObj() = default;	// default destructor disables move semantics for member m_name
				};
			}
			namespace slicing_problem
			{
				class GeoObj {
				protected:
					std::string m_name;				// name of the geometric object
				public:
					GeoObj()
						: m_name{}
					{}
					GeoObj(std::string n)
						: m_name{ std::move(n) }
					{}
					virtual void draw() const = 0;	// pure virtual function (introducing the API)
					virtual ~GeoObj() = default;	// default destructor disables move semantics for member m_name
				};

				class Circle : public GeoObj {
				public:
					Circle(std::string n)
						: GeoObj(n)
					{}
					virtual void draw() const {}
				};

				void run()
				{
					Circle c1{ std::string{ "c1" } }, c2{ std::string{ "c2" } };
					GeoObj& geo_ref{ c1 };
					geo_ref = c2;	// OOPS: uses default GeoObj::operator=() and assigns no Cirle members
				}
			}
			namespace solve_slicing_problem
			{
				class GeoObj {
				protected:
					std::string m_name;				// name of the geometric object
					GeoObj(std::string n)
						: m_name{std::move(n)}
					{}
				public:
					virtual void draw() const = 0;	// pure virtual function (introducing the API)
					virtual ~GeoObj() = default;	// default destructor disables move semantics for member m_name
				protected:
					// enable copy and move semantics (callable only for derived classes)
					GeoObj(const GeoObj&) = default;
					GeoObj(GeoObj&&) = default;
					// disable assignment operators (due to problem of slicing)
					GeoObj& operator= (const GeoObj&) = delete;
					GeoObj& operator= (GeoObj&&) = delete;
				};
				class Circle : public GeoObj {
				public:
					Circle(std::string n)
						:GeoObj(n)
					{}
					virtual void draw() const {}
				};

				void run()
				{
					Circle c1{ std::string{"c1"} }, c2{ std::string{"c2"} };
					GeoObj& geo_ref{ c1 };
					//geo_ref = c2;				// OOPS: avoids GeoObj::operator=() and slicing
					//geo_ref = std::move(c2);	// OOPS: avoids GeoObj::operator=() and slicing
				}
			}
		}
		// Implementing a Polymorphic Derive Class
		namespace sec_4_4_2a // disables move semantics via default destructor
		{
			class GeoObj {
			protected:
				std::string m_name;				// name of the geometric object
				GeoObj(std::string n)
					: m_name{ std::move(n) }
				{}
			public:
				virtual void draw() const = 0;	// pure virtual function (introducing the API)
				virtual ~GeoObj() = default;	// default destructor disables move semantics for member m_name
			protected:
				// enable copy and move semantics (callable only for derived classes)
				GeoObj(const GeoObj&) = default;
				GeoObj(GeoObj&&) = default;
				// disable assignment operators (due to problem of slicing)
				GeoObj& operator= (const GeoObj&) = delete;
				GeoObj& operator= (GeoObj&&) = delete;
			};

			// http://www.cppmove.com/code/poly/coord.hpp.html
			class Coord {
			private:
				int m_x{ 0 };
				int m_y{ 0 };
			public:
				// default constructor
				Coord() = default;
				Coord(int xarg, int yarg)
					: m_x{xarg}, m_y{yarg}
				{}
				Coord(const Coord&) = default;
				Coord(Coord&&) = default;
				Coord& operator= (const Coord&) = default;
				Coord& operator= (Coord&&) = default;

				friend Coord operator+ (Coord c1, Coord c2) { // plus
					return Coord{c1.m_x+c2.m_x, c1.m_y + c2.m_y };
				}
				friend Coord operator- (Coord c1, Coord c2) { // diff
					return Coord{ c1.m_x - c2.m_x, c1.m_y - c2.m_y };
				}
				Coord operator- () const { // negate
					return Coord{-m_x, -m_y};
				}
				void operator+= (Coord c) { // +=
					*this = *this + c;	// or: m_x+=c.m_x; m_y+=c.m_y
				}
				void operator-= (Coord c) { // -=
					operator+=(-c);	// or as above
				}
				int getX() const {
					return m_x;
				}
				int getY() const {
					return m_y;
				}
				friend std::ostream& operator<<(std::ostream& strm, Coord c) {
					return strm << '(' << c.m_x << ',' << c.m_y << ')';
				}
			};
			// http://www.cppmove.com/code/poly/polygon.hpp.html
			class Polygon : public GeoObj {
			protected:
				std::vector<Coord> m_points;
			public:
				Polygon(std::string s, std::initializer_list<Coord> pl= {})
					: GeoObj{ std::move(s) }, m_points { std::move(pl) }
				{}
				virtual void draw() const override {
					std::cout << "polygon '" << m_name << "' over";
					for (auto& p : m_points) {
						std::cout << " " << p;
					}
					std::cout << "\n";
				}
				virtual ~Polygon() = default; // disables move semantics
			};
			void run()
			{
				Polygon p0{ "Poly1", {Coord{1,1}, Coord{1,9}, Coord{9,9}, Coord{9,1}} };
				Polygon p1{p0};					// copy
				Polygon p2{ std::move(p0) };	// move

				p0.draw();
				p1.draw();
				p2.draw();
			}
		}
		namespace sec_4_4_2b
		{
			class GeoObj {
			protected:
				std::string m_name;				// name of the geometric object
				GeoObj(std::string n)
					: m_name{ std::move(n) }
				{}
			public:
				virtual void draw() const = 0;	// pure virtual function (introducing the API)
				virtual ~GeoObj() = default;	// default destructor disables move semantics for member m_name
			protected:
				// enable copy and move semantics (callable only for derived classes)
				GeoObj(const GeoObj&) = default;
				GeoObj(GeoObj&&) = default;
				// disable assignment operators (due to problem of slicing)
				GeoObj& operator= (const GeoObj&) = delete;
				GeoObj& operator= (GeoObj&&) = delete;
			};

			// http://www.cppmove.com/code/poly/coord.hpp.html
			class Coord {
			private:
				int m_x{ 0 };
				int m_y{ 0 };
			public:
				// default constructor
				Coord() = default;
				Coord(int xarg, int yarg)
					: m_x{ xarg }, m_y{ yarg }
				{}
				Coord(const Coord&) = default;
				Coord(Coord&&) = default;
				Coord& operator= (const Coord&) = default;
				Coord& operator= (Coord&&) = default;

				friend Coord operator+ (Coord c1, Coord c2) { // plus
					return Coord{ c1.m_x + c2.m_x, c1.m_y + c2.m_y };
				}
				friend Coord operator- (Coord c1, Coord c2) { // diff
					return Coord{ c1.m_x - c2.m_x, c1.m_y - c2.m_y };
				}
				Coord operator- () const { // negate
					return Coord{ -m_x, -m_y };
				}
				void operator+= (Coord c) { // +=
					*this = *this + c;	// or: m_x+=c.m_x; m_y+=c.m_y
				}
				void operator-= (Coord c) { // -=
					operator+=(-c);	// or as above
				}
				int getX() const {
					return m_x;
				}
				int getY() const {
					return m_y;
				}
				friend std::ostream& operator<<(std::ostream& strm, Coord c) {
					return strm << '(' << c.m_x << ',' << c.m_y << ')';
				}
			};
			// http://www.cppmove.com/code/poly/polygon.hpp.html
			class Polygon : public GeoObj {
			protected:
				std::vector<Coord> m_points;
			public:
				Polygon(std::string s, std::initializer_list<Coord> pl = {})
					: GeoObj{ std::move(s) }, m_points{ std::move(pl) }
				{}
				virtual void draw() const override {
					std::cout << "polygon '" << m_name << "' over";
					for (auto& p : m_points) {
						std::cout << " " << p;
					}
					std::cout << "\n";
				}
				//virtual ~Polygon() = default; // move semantics enabled
			};
			void run()
			{
				Polygon p0{ "Poly1", {Coord{1,1}, Coord{1,9}, Coord{9,9}, Coord{9,1}} };
				Polygon p1{ p0 };				// copy
				Polygon p2{ std::move(p0) };	// move

				p0.draw();
				p1.draw();
				p2.draw();
			}
		}
	}
}
