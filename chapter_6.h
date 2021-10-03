#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

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
			process2(c);	// use c after move semantics
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
}
