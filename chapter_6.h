#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <memory>

// Moved From States
namespace chapter_6
{
	// Required and Guaranteed States of Moved-From Objects
	namespace sec_6_1
	{
	}
	// Required States of Moved-From Objects
	namespace sec_6_1_1
	{
	}
	// Guaranteed States of Moved-From Objects
	namespace sec_6_1_2
	{
		void run()
		{
			std::string s;

		}
	}
	// Broken Invariants
	namespace sec_6_1_3
	{
	}
	// Destructible and Assignable
	namespace sec_6_2
	{
	}
	// Assignable and Destructible Moved-From Objects
	namespace sec_6_2_1
	{
		class Customer {
		private:
			std::string			m_name;		// name of the customer
			std::vector<int>	m_values;	// some values of the customer
		public:
			Customer(const std::string& n)
				: m_name(n)
			{
				assert(!m_name.empty());
			}

			std::string get_name() const
			{
				return m_name;
			}

			void add_value(int val)
			{
				m_values.push_back(val);
			}

			friend std::ostream& operator<< (std::ostream& strm, const Customer& cust)
			{
				strm << '[' << cust.m_name << ": ";
				for (int val : cust.m_values) {
					strm << val << ' ';
				}
				strm << ']';
				return strm;
			}
		};
		void process(Customer&& cust)
		{
			std::cout << cust << std::endl;
		}
		void process2(const Customer& cust)
		{
			std::cout << cust << std::endl;
		}
		void run()
		{
			Customer c{"Michael Spencer"};

			process(std::move(c)); // now both m_names and m_values have valid but unspecified values			
			process2(c);	// use c after move semantics is a potential bug
		}	// destructor of c will clean up m_name and m_values (whatever their state is)
	}
}

#include <array>
#include <thread>
namespace chapter_6
{
	// Non-Destructible Moved-From Objects
	namespace sec_6_2_2
	{
		class Tasks {		
		private:
			std::array<std::thread, 10> m_threads;	// array of threads for up to 10 tasks
			int m_num_threads{ 0 };					// current number of threads/tasks
		public:
			Tasks() = default;

			// pass a new thread
			template <typename T>
			void start(T op)
			{
				m_threads[m_num_threads] = std::thread{ std::move(op) };
				++m_num_threads;
			}

			// at the end wait for all started threads
			~Tasks()
			{
				for (int i = 0; i < m_num_threads; ++i) {
					m_threads[i].join();
				}
			}
		};

		void run()
		{
			Tasks ts;
			ts.start([]() {
				std::this_thread::sleep_for(std::chrono::seconds{ 2 });
				std::cout << "\nt1 done" << std::endl;
			});
			ts.start([]() {				
				std::cout << "\nt2 done" << std::endl;
				});
		}
	}
}

// Now consider enabling move semantics by generating default move operations
namespace chapter_6
{
	// Non-Destructible Moved-From Objects
	namespace sec_6_2_2
	{
		class Tasks2 {
		private:
			std::array<std::thread, 10> m_threads;	// array of threads for up to 10 tasks
			int m_num_threads{ 0 };					// current number of threads/tasks
		public:
			Tasks2() = default;

			// OOPS: enable default move semantics
			Tasks2(Tasks2&&) = default;
			Tasks2& operator=(Tasks2&&) = default;

			// pass a new thread
			template <typename T>
			void start(T op)
			{
				m_threads[m_num_threads] = std::thread{ std::move(op) };
				++m_num_threads;
			}

			// at the end wait for all started threads
			~Tasks2()
			{
				for (int i = 0; i < m_num_threads; ++i) {
					m_threads[i].join();
				}
			}
		};

		void run2()
		{			
			try {
				Tasks2 ts;
				ts.start([]() {
					std::this_thread::sleep_for(std::chrono::seconds{ 2 });
					std::cout << "\nt1 done" << std::endl;
					});
				ts.start([]() {
					std::cout << "\nt2 done" << std::endl;
					});

				// OPPS: move tasks:
				Tasks2 other{ std::move(ts) };
			}
			catch (const std::exception& e) {
				std::cerr << "EXCEPTION: " << e.what() << std::endl;
			}
		}
	}
}

namespace chapter_6
{
	// Dealing with Broken Invariants
	namespace sec_6_3 {}

	// Broken Invariants Due to a Moved Value Member
	namespace sec_6_3_1
	{
		// http://www.cppmove.com/code/basics/card.hpp.html
		void assert_valid_card(const std::string& val)
		{
			assert(val.find("seven") != std::string::npos ||
				val.find("eight") != std::string::npos ||
				val.find("nine") != std::string::npos ||
				val.find("ten") != std::string::npos ||
				val.find("jack") != std::string::npos ||
				val.find("queen") != std::string::npos ||
				val.find("king") != std::string::npos ||
				val.find("ace") != std::string::npos);
			assert(val.find("clubs") != std::string::npos ||
				val.find("spades") != std::string::npos ||
				val.find("hearts") != std::string::npos ||
				val.find("diamonds") != std::string::npos);
		}

		class Card {
		private:
			std::string m_value;			// rank + "-of-" + suit
		public:
			Card(const std::string& v)
				: m_value{ v } {
				assert_valid_card(m_value);	// ensure that the value is always valid.
			}
			// ...

			std::string get_value() const {
				return m_value;
			}

			friend std::ostream& operator<< (std::ostream& strm, const Card& c) {
				return strm << c.m_value;
			}
		};

		void print(const Card& c)
		{
			std::string val{c.get_value()};
			auto pos = val.find("-of-");			// find position of substring (no check)
			std::cout << val.substr(0, pos) << ' '
				<< val.substr(pos + 4) << '\n';		// OOPS: possible core dump
		}
		void run()
		{}
	}

	// Breaking Invariants Due to a Moved Consistent Value Members
	namespace sec_6_3_2
	{
		class IntString {
		private:
			int			m_val;	// value
			std::string	m_sval;	// cached string representation of the value
		public:
			IntString(int i = 0)
				: m_val{i}, m_sval{std::to_string(i)}
			{}

			void set_value(int i) {
				m_val = i;
				m_sval = std::to_string(i);
			}

			void dump() const {
				std::cout << " [" << m_val << "/'" << m_sval << "']\n";
			}
		};

		void run()
		{
			IntString is1{ 42 };
			IntString is2;

			std::cout << "is1 and is2 before move:\n";
			is1.dump();
			is2.dump();

			is2 = std::move(is1);

			std::cout << "is1 and is2 after move:\n";
			is1.dump();	// compiler warning C26800: use of a moved from object
			is2.dump();

		}
	}

	// Breaking Invariants Due to Moved Pointer-Like Members
	namespace sec_6_3_3
	{
		class SharedInt {
		private:
			std::shared_ptr<int> m_sp;
		public:
			explicit SharedInt(int val)
				: m_sp{std::make_shared<int>(val)}
			{}

			std::string asString() const {
				return std::to_string(*m_sp);	// OOPS: assume there is always an int value
			}
		};

		void run()
		{
			SharedInt si1{ 42 };
			SharedInt si2{ si1 };	// si1 and si2 share the value 42

			std::cout << si1.asString() << '\n';	// OK

			SharedInt si3{ 42 };
			SharedInt si4{ std::move(si3) };	// OOPS: moves away the allocted memory in si3

			std::cout << si3.asString() << '\n';	// undefined behaviour (probably core dump) -- Exception

		}
	}
}

// Implementing Move Semantics
namespace chapter_6
{
	namespace sec_6_3_3b
	{
		class SharedInt {
		private:
			std::shared_ptr<int> m_sp;
			
			// special "value" for moved-from objects
			inline static std::shared_ptr<int> moved_from_value{ std::make_shared<int>(0) };

		public:
			explicit SharedInt(int val)
				: m_sp{ std::make_shared<int>(val) }
			{}

			std::string asString() const
			{
				return std::to_string(*m_sp);	// OOPS: unconditional deref
			}

			// fix moving special member functions
			SharedInt(SharedInt&& si) noexcept
				:m_sp{ std::move(si.m_sp) } 
			{
				si.m_sp = moved_from_value;
			}

			SharedInt& operator=(SharedInt&& si) noexcept
			{
				if (this != &si) {
					m_sp = std::move(si.m_sp);
					si.m_sp = moved_from_value;
				}
				return *this;
			}

			// enable copying (deleted with user-declared move operations):
			SharedInt(const SharedInt&) = default;
			SharedInt& operator= (const SharedInt&) = default;
		};

		void run()
		{
			SharedInt si1{ 42 };
			SharedInt si2{ si1 };	// si1 and si2 share the value 42

			std::cout << si1.asString() << '\n';	// OK

			SharedInt si3{ 42 };
			SharedInt si4{ std::move(si3) };

			std::cout << si3.asString() << '\n';	
		}
	}
}